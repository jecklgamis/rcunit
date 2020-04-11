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

extern rcu_module *rcu_srch_mod_by_name_global(const char *mod_name, rcu_registry **which_reg);

rcu_registry *rcu_srch_reg_by_name(rcu_test_machine *machine, const char *reg_name);

int rcu_mod_exists(rcu_registry *reg, rcu_module *srch_mod);

int rcu_reg_exists(rcu_test_machine *machine, rcu_registry *reg);

rcu_registry *rcu_cre_test_reg(const char *name);

RCU_API rcu_registry *rcu_get_reg(const char *name) {
    rcu_registry *reg = NULL;
    if (name == NULL) {
        return NULL;
    }
    rcu_init();
    reg = rcu_srch_reg_by_name(&the_test_machine, name);
    if (reg == NULL) {
        reg = rcu_cre_test_reg(name);
        rcu_add_test_reg(reg);
    }
    return reg;
}

RCU_API rcu_registry *rcu_get_default_reg() {
    rcu_init();
    return &the_test_machine.def_reg;
}

rcu_registry *rcu_alloc_reg(int nr_reg) {
    rcu_registry *reg = NULL;
    if (nr_reg == 0) {
        return NULL;
    }
    reg = (rcu_registry *) rcu_malloc(sizeof(rcu_registry) * nr_reg);
    if (reg != NULL) {
        memset(reg, 0, sizeof(rcu_registry));
    }
    return reg;
}

void rcu_free_reg(rcu_registry *reg) {
    if (reg != NULL) {
        rcu_free(reg);
    }
}

int rcu_init_reg(rcu_registry *reg, const char *name) {
    int name_len = 0;
    if (reg == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVREG));
        return RCU_E_NG;
    }
    if (name == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVREGNAME));
        return RCU_E_NG;
    }
    memset(reg, 0, sizeof(rcu_registry));
    rcu_init_list(&reg->link);
    rcu_init_list(&reg->mod_list);
    name_len = strlen(name) > RCU_TEST_REGISTRY_NAME_LENGTH ?
               RCU_TEST_REGISTRY_NAME_LENGTH : strlen(name);
    strncpy(reg->name, name, name_len);
    RCU_LOG_DEBUG("Initialized registry : %s", reg->name);
    return RCU_E_OK;
}

rcu_registry *rcu_cre_test_reg(const char *name) {
    rcu_registry *reg = NULL;
    rcu_test_machine *machine = &the_test_machine;
    rcu_init();
    if (!(reg = rcu_alloc_reg(1))) {
        RCU_SET_ERCD(RCU_E_NOMEM);
        RCU_LOG_WARN("%s", RCU_GET_ERR_MSG());
        return NULL;
    }
    if (name != NULL && !strcmp(name, RCU_DEFAULT_REGISTRY_NAME)) {
        RCU_SET_ERCD(RCU_E_INVREGNAME);
        RCU_LOG_WARN("%s (%s is reserved)", RCU_GET_ERR_MSG(),
                     RCU_DEFAULT_REGISTRY_NAME);
        rcu_free_reg(reg);
        return NULL;
    }
    if (rcu_init_reg(reg, name) == RCU_E_NG) {
        rcu_free_reg(reg);
        return NULL;
    }
    RCU_LOG_DEBUG("Test registry created : %s", reg->name);
    return reg;
}

RCU_API int rcu_destroy_test_reg(rcu_registry *reg) {
    rcu_list *cursor = NULL;
    rcu_module *mod = NULL;
    rcu_test_machine *machine = &the_test_machine;

    rcu_init();
    if (reg == NULL) {
        RCU_SET_ERCD(RCU_E_INVREG);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    RCU_LOG_DEBUG("Destroying test registry: %s", reg->name);

    RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor) {
        mod = (rcu_module *) cursor;
        RCU_SAVE_CURSOR(cursor)
            rcu_remove_list(cursor);
            rcu_destroy_test_mod(mod);
        RCU_RESTORE_CURSOR(cursor)
    }
    rcu_remove_list(&reg->link);
    if (strcmp(reg->name, RCU_DEFAULT_REGISTRY_NAME)) {
        rcu_free_reg(reg);
    }
    return RCU_E_OK;
}

RCU_API int rcu_add_test_mod(rcu_module *mod) {
    rcu_init();
    return rcu_add_mod_to_reg(rcu_get_default_reg(), mod);
}

/* TODO clean this up */
RCU_API int rcu_add_mod_to_reg(rcu_registry *reg, rcu_module *mod) {
    rcu_registry *which_reg = NULL;
    rcu_module *srch_mod = NULL;
    rcu_test_machine *machine = NULL;
    rcu_init();
    machine = &the_test_machine;
    if (mod == NULL) {
        RCU_SET_ERCD(RCU_E_INVMOD);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
#if 0
    /* Check if a module by that name exists in any registry */
    srch_mod = rcu_srch_mod_by_name_global(mod->name, &which_reg);
    if (srch_mod != NULL) {
        RCU_SET_ERCD(RCU_E_MODEXISTS);
        RCU_LOG_WARN("%s (module = %s, registry = %s)", RCU_GET_ERR_MSG(), srch_mod->name, which_reg->name);
        return RCU_E_NG;
    }

    /* Check if a module exists in any registry */
    if (rcu_srch_mod_by_ref_global(mod, &which_reg)) {
        RCU_SET_ERCD(RCU_E_MODEXISTS);
        RCU_LOG_WARN("%s (module = %s, registry = %s)", RCU_GET_ERR_MSG(),
                srch_mod->name, which_reg->name);
        return RCU_E_NG;
    }
#endif
    reg = (reg == NULL) ? rcu_get_default_reg() : reg;
#if 0
    reg = (reg == NULL) ? RCU_GET_DEF_REG() : reg;
    if (!rcu_srch_reg_by_ref(RCU_GET_CURR_MACH(), reg)) {
        RCU_SET_ERCD(RCU_E_UNKNOWNREG);
        RCU_LOG_WARN("%s", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
#endif

#if 0
    if (rcu_mod_exists(reg, mod)) {
        RCU_SET_ERCD(RCU_E_MODEXISTS);
        RCU_LOG_WARN("%s (module = %s, registry = %s)", RCU_GET_ERR_MSG(),
                mod->name, reg->name);
        return RCU_E_NG;
    }
#endif
    /* Initialize essential fields */
    rcu_init_list(&mod->link);
    rcu_insert_list(&reg->mod_list, &mod->link);
    RCU_INCR(reg->nr_mod);
    rcu_init_list(&mod->fail_rec_list);
    RCU_LOG_DEBUG("%s added to %s", mod->name, reg->name);
    return RCU_E_OK;
}

RCU_API int rcu_add_test_mod_tbl(rcu_registry *reg, rcu_module_entry *mod_tbl) {
    rcu_module_entry *cursor = NULL;
    rcu_module *mod = NULL;
    int mod_index;
    int ercd;
    rcu_test_machine *machine = NULL;

    rcu_init();
    RCU_LOG_DEBUG("Adding test module table");
    machine = &the_test_machine;
    if (mod_tbl == NULL) {
        RCU_SET_ERCD(RCU_E_INVMODTABLE);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }

    RCU_FOR_EACH_MOD_ENTRY(mod_tbl, cursor, mod_index) {
        if (cursor->func_tbl == NULL) {
            RCU_LOG_WARN("Invalid test function table (index = %d)", mod_index);
            continue;
        }
        mod = rcu_cre_test_mod(cursor->name, cursor->init, cursor->destroy);
        if (mod == NULL) {
            RCU_LOG_WARN("Unable to create test module. (index = %d)", mod_index);
            continue;
        }
        ercd = rcu_add_mod_to_reg(reg, mod);
        if (ercd == RCU_E_NG) {
            RCU_LOG_WARN("Unable to add test module. (index = %d)", mod_index);
            rcu_destroy_test_mod(mod);
            continue;
        }
        ercd = rcu_add_test_func_tbl(mod, cursor->func_tbl);
        if (ercd == RCU_E_NG) {
            RCU_LOG_WARN("Unable to add test function table. (index = %d)", mod_index);
        }
    }
    RCU_LOG_DEBUG("Test module table added");
    return RCU_E_OK;
}

rcu_module *rcu_srch_mod_by_name(rcu_registry *reg, const char *mod_name) {
    rcu_list *cursor = NULL;
    rcu_module *srch_mod = NULL;

    if (mod_name == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMODNAME));
        return NULL;
    }

    RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor) {
        srch_mod = (rcu_module *) cursor;
        if (!strcmp(srch_mod->name, mod_name)) {
            return (srch_mod);
        }
    }
    return NULL;
}

int rcu_mod_exists(rcu_registry *reg, rcu_module *srch_mod) {
    rcu_list *cursor = NULL;
    rcu_module *mod = NULL;

    if (reg == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVREG));
        return RCU_FALSE;
    }
    if (srch_mod == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMOD));
        return RCU_FALSE;
    }

    RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor) {
        mod = (rcu_module *) cursor;
        if (mod == srch_mod) {
            return RCU_TRUE;
        }
    }
    return RCU_FALSE;
}

void rcu_dump_test_reg(rcu_registry *reg) {
    rcu_list *cursor1 = NULL;
    rcu_list *cursor2 = NULL;
    rcu_test *func = NULL;
    rcu_module *mod = NULL;

    reg = reg == NULL ? rcu_get_default_reg() : reg;
    RCU_LOG_INFO("|   +- registry [name = %s]", reg->name);

    RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor1) {
        mod = (rcu_module *) cursor1;
        RCU_LOG_INFO("|   |   +- module [name = %s]", mod->name);

        RCU_FOR_EACH_ENTRY(&mod->func_list, cursor2) {
            func = (rcu_test *) cursor2;
            RCU_LOG_INFO("|   |   |   +- test [name = %s]", func->name);
        }
    }
}

RCU_API int rcu_run_test_reg_unsupported(rcu_registry *reg) {
    rcu_test_machine *machine = NULL;
    char ts_buff[RCU_TSTAMP_BUFF_SIZE];
    rcu_init();
    machine = &the_test_machine;
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO("Test run started %s", ts_buff);
    rcu_restart_mach(machine);
    RCU_SET_RUN_LEVEL(machine, RCU_RUN_LEVEL_REG);
    reg = (reg == NULL) ? &machine->def_reg : reg;

    if (rcu_reg_exists(machine, reg) == 0) {
        RCU_SET_ERCD(RCU_E_UNKNOWNREG);
        RCU_LOG_WARN("%s (%s)", RCU_GET_ERR_MSG(), reg->name);
        return RCU_E_NG;
    }
    RCU_SET_CURR_REG(machine, reg);
    if ((rcu_run_test_reg_impl(machine, reg)) == RCU_E_NG) {
        return RCU_E_NG;
    }
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO("Test run finished %s", ts_buff);
    rcu_stop_mach(machine);
    rcu_gen_test_run_report(machine);
    return RCU_E_OK;
}

RCU_API int rcu_run_test_reg_by_name(const char *name) {
    rcu_registry *reg = NULL;
    rcu_test_machine *machine = NULL;

    rcu_init();
    machine = &the_test_machine;

    if (name == NULL) {
        reg = rcu_get_default_reg();;
    } else {
        reg = rcu_srch_reg_by_name(machine, name);
    }

    if (reg == NULL) {
        RCU_SET_ERCD(RCU_E_INVREG);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    return (rcu_run_test_reg_unsupported(reg));
}

int rcu_reg_exists(rcu_test_machine *machine, rcu_registry *reg) {
    rcu_registry *srch_reg = NULL;
    rcu_list *cursor = NULL;
    if (machine != NULL && reg != NULL) {

        RCU_FOR_EACH_ENTRY(&machine->reg_list, cursor) {
            srch_reg = (rcu_registry *) cursor;
            if (srch_reg == reg) {
                return 1;
            }
        }
    }
    return 0;
}

int rcu_run_test_reg_impl(rcu_test_machine *machine, rcu_registry *reg) {
    rcu_list *cursor2, *cursor3;
    rcu_module *mod;
    int run_event;
    rcu_test *func;

    reg = (reg == NULL) ? &machine->def_reg : reg;
    if (RCU_GET_RUN_LEVEL(machine) == RCU_RUN_LEVEL_REG) {
        if (machine->run_hook != NULL) {
            run_event = RCU_TEST_RUN_STARTED;
            machine->run_hook(&run_event);
        }
        machine->nr_failed_reg = 0;
        machine->nr_succ_reg = 0;
        machine->nr_failed_test = 0;
        machine->nr_succ_test = 0;
        reg->nr_failed_mod = 0;
        reg->nr_succ_mod = 0;
    }
    RCU_LOG_DEBUG("Running tests from registry : %s", reg->name);

    RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor2) {
        mod = (rcu_module *) cursor2;
        RCU_SET_CURR_MOD(machine, mod);
        rcu_run_test_mod_impl(machine, mod);
        /*
         * If the module's init or destroy function failed, override the
         * module's status to failed, even if all its tests completed
         * successfully.
         */
        if (RCU_IS_INIT_FAILED(mod) || RCU_IS_DESTROY_FAILED(mod)) {
            RCU_SET_RUN_STAT(mod, RCU_RUN_STAT_TEST_FAILED);

            /* Override all tests results */
            RCU_FOR_EACH_ENTRY(&mod->func_list, cursor3) {
                func = (rcu_test *) cursor3;
                RCU_SET_RUN_STAT(func, RCU_RUN_STAT_TEST_FAILED);
                RCU_RESET(func->nr_fail_assert);
                RCU_RESET(func->nr_succ_assert);
                rcu_del_all_fail_rec_from_func(func);
            }
        } else {
            if (RCU_IS_TEST_FAILED(mod)) {
                RCU_INCR(reg->nr_failed_mod);
            } else {
                RCU_INCR(reg->nr_succ_mod);
            }
        }
    }
    if (RCU_GET_RUN_LEVEL(machine) == RCU_RUN_LEVEL_REG) {
        if (machine->run_hook != NULL) {
            run_event = RCU_TEST_RUN_FINISHED;
            machine->run_hook(&run_event);
        }
    }
    if (reg->nr_failed_mod > 0) {
        RCU_INCR(machine->nr_failed_reg);
    } else {
        RCU_INCR(machine->nr_succ_reg);
    }
    return RCU_E_OK;
}

