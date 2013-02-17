    /*
 * The MIT License (MIT)
 *
 * RCUNIT - A unit testing framework for C
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "rcunit.h"

extern rcu_registry *rcu_srch_reg_by_name(rcu_test_machine *machine, const char *reg_name);

int rcu_add_test_reg(rcu_registry *reg) {
    rcu_test_machine *machine = NULL;
    rcu_registry *srch_reg = NULL;

    rcu_init();
    machine = &the_test_machine;
    if (!rcu_is_mach_initialized(machine)) {
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        return RCU_E_NG;
    }
    if (reg == NULL) {
        RCU_SET_ERCD(RCU_E_INVREG);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    if (rcu_reg_exists(machine, reg)) {
        RCU_SET_ERCD(RCU_E_REGEXISTS);
        RCU_LOG_WARN("%s (%s)", RCU_GET_ERR_MSG(), reg->name);
        return RCU_E_NG;
    }
    srch_reg = rcu_srch_reg_by_name(machine, reg->name);
    if (srch_reg != NULL) {
        RCU_SET_ERCD(RCU_E_REGEXISTS);
        RCU_LOG_WARN("%s (%s)", RCU_GET_ERR_MSG(), srch_reg->name);
        return RCU_E_NG;
    }
    rcu_init_list(&reg->link);
    rcu_insert_list(&machine->reg_list, &reg->link);
    RCU_INCR(machine->nr_reg);
    RCU_LOG_DEBUG("Test registry added : %s", reg->name);
    return RCU_E_OK;
}

int rcu_init_test_mach(rcu_test_machine *machine) {
    if (machine != NULL) {
        RCU_LOG_DEBUG("Initializing test machine");
        memset(machine, 0x00, sizeof (rcu_test_machine));
        rcu_init_list(&machine->reg_list);
        rcu_init_list(&machine->ae.assert_list);
        machine->name = RCU_DEFAULT_MACHINE_NAME;
        machine->init_done = 1;
        RCU_SET_RUN_CTX(machine, RCU_RUN_CTX_UNKNOWN);
#if RCU_ENABLE_MTRACE
        rcu_init_mtrace();
#endif
        rcu_init_exception();
        rcu_init_reg(&machine->def_reg, RCU_DEFAULT_REGISTRY_NAME);
        rcu_add_test_reg(&machine->def_reg);
        rcu_init_mod(&machine->def_mod, NULL, NULL, RCU_DEFAULT_MODULE_NAME);
        rcu_add_mod_to_reg(&machine->def_reg, &machine->def_mod);
    }
    return RCU_E_OK;
}

int rcu_destroy_test_mach(rcu_test_machine *machine) {
    RCU_LOG_DEBUG("Destroying test machine");
    rcu_destroy_exception();
    rcu_del_all_fail_rec(machine);
    rcu_destroy_test_dbase(machine);
#if RCU_ENABLE_MTRACE
    rcu_destroy_mtrace();
#endif
    machine->init_done = 0;
    return RCU_E_OK;
}

int rcu_destroy_test_dbase(rcu_test_machine *machine) {
    rcu_list *cursor = NULL;
    rcu_registry *reg = NULL;
    RCU_LOG_DEBUG("Destroying test database");
    if (machine != NULL) {
        if (!rcu_is_mach_initialized(machine)) {
            RCU_LOG_WARN("%s", RCU_GET_ERR_MSG_OF(RCU_E_MACHNOINIT));
            return RCU_E_NG;
        }

        RCU_FOR_EACH_ENTRY(&machine->reg_list, cursor) {
            reg = (rcu_registry*) cursor;
            RCU_SAVE_CURSOR(cursor)
            rcu_remove_list(cursor);
            rcu_destroy_test_reg(reg);
            RCU_RESTORE_CURSOR(cursor)
        }
    }
    return RCU_E_OK;
}

int rcu_is_mach_initialized(rcu_test_machine *machine) {
    return machine != NULL && machine->init_done;
}

int rcu_restart_mach(rcu_test_machine *machine) {
    RCU_LOG_DEBUG("Restarting machine");
    return RCU_E_OK;
}

int rcu_stop_mach(rcu_test_machine *machine) {
    RCU_LOG_DEBUG("Stopping machine");
    rcu_stop_assert_engine(machine);
    return RCU_E_OK;
}

int rcu_run_tests_impl(rcu_test_machine *machine) {
    rcu_list *cursor1 = NULL;
    rcu_registry *reg = NULL;
    int run_event;

    if (machine != NULL) {
        if (machine->run_hook != NULL) {
            run_event = RCU_TEST_RUN_STARTED;
            machine->run_hook(&run_event);
        }
        RCU_LOG_DEBUG("Running test machine : %s", machine->name);

        RCU_FOR_EACH_ENTRY(&machine->reg_list, cursor1) {
            reg = (rcu_registry*) cursor1;
            RCU_SET_CURR_REG(machine, reg);
            rcu_run_test_reg_impl(machine, reg);
        }
        if (machine->run_hook != NULL) {
            run_event = RCU_TEST_RUN_FINISHED;
            machine->run_hook(&run_event);
        }
    }
    return RCU_E_OK;
}

void rcu_reset_all_run_stat() {
    rcu_test_machine *machine = &the_test_machine;
    rcu_list *cursor1 = NULL;
    rcu_list *cursor2 = NULL;
    rcu_list *cursor3 = NULL;
    rcu_module *mod;
    rcu_registry *reg;
    rcu_test *func;

    RCU_FOR_EACH_ENTRY(&machine->reg_list, cursor1) {
        reg = (rcu_registry*) cursor1;

        RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor2) {
            mod = (rcu_module*) cursor2;

            RCU_FOR_EACH_ENTRY(&mod->func_list, cursor3) {
                func = (rcu_test*) cursor3;
                func->run_stat = RCU_RUN_STAT_NOTTESTED;
            }

        }
    }
}

rcu_registry *rcu_srch_reg_by_name(rcu_test_machine *machine, const char *reg_name) {
    rcu_list *cursor = NULL;
    rcu_registry *srch_reg = NULL;
    if (machine != NULL && reg_name != NULL) {

        RCU_FOR_EACH_ENTRY(&machine->reg_list, cursor) {
            srch_reg = (rcu_registry*) cursor;
            if (!strcmp(reg_name, srch_reg->name)) {
                return srch_reg;
            }
        }
    }
    return NULL;
}
