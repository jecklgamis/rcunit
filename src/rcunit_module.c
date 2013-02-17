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


RCU_API rcu_module *rcu_get_default_mod() {
    rcu_init();
    return &the_test_machine.def_mod;
}

RCU_API rcu_module *rcu_get_mod(const char *name) {
    rcu_registry *which_reg;
    rcu_module *mod;
    rcu_init();
    if (name != NULL) {
        if ((mod = rcu_srch_mod_by_name_global(name, &which_reg)) != NULL) {
            return mod;
        }
        mod = rcu_cre_test_mod(name, NULL, NULL);
        rcu_add_test_mod(mod);
        return mod;
    }
    return NULL;
}

RCU_API void rcu_set_mod_fxt(rcu_module *mod, rcu_generic_function setup,
        rcu_generic_function teardown) {
    mod->init = setup;
    mod->destroy = teardown;
}

rcu_module *rcu_alloc_test_mod(unsigned int nr_mod) {
    rcu_module *mod = NULL;

    if (nr_mod == 0) {
        return NULL;
    }
    mod = (rcu_module *) rcu_malloc(sizeof (rcu_module) * nr_mod);
    if (mod != NULL) {
        memset(mod, 0x00, sizeof (rcu_module));
    }
    return mod;
}

int rcu_free_test_mod(rcu_module *mod) {
    if (mod == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMOD));
        return RCU_E_NG;
    }
    rcu_free(mod);
    return RCU_E_OK;
}

int rcu_init_mod(rcu_module *mod, rcu_generic_function init,
        rcu_generic_function destroy, const char *name) {
    int name_len = 0;
    if (mod == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMOD));
        return RCU_E_NG;
    }
    if (name == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMODNAME));
        return RCU_E_NG;
    }
    memset(mod, 0x00, sizeof (rcu_module));
    rcu_init_list(&mod->link);
    rcu_init_list(&mod->func_list);
    mod->init = init;
    mod->destroy = destroy;
    name_len = strlen(name) > RCU_TEST_MODULE_NAME_LENGTH ? RCU_TEST_MODULE_NAME_LENGTH
            : strlen(name);
    strncpy(mod->name, name, name_len);
    return RCU_E_OK;
}

rcu_module *rcu_cre_test_mod(const char *name, rcu_generic_function init,
        rcu_generic_function destroy) {
    rcu_module *mod;
    rcu_test_machine *machine = &the_test_machine;
    rcu_init();
    if ((mod = rcu_alloc_test_mod(1)) == NULL) {
        RCU_SET_ERCD(RCU_E_NOMEM);
        RCU_LOG_WARN("%s", RCU_GET_ERR_MSG());
        return NULL;
    }
    if (name != NULL && !strcmp(name, RCU_DEFAULT_MODULE_NAME)) {
        RCU_SET_ERCD(RCU_E_INVMODNAME);
        RCU_LOG_WARN("%s (%s is reserved)", RCU_GET_ERR_MSG(), RCU_DEFAULT_MODULE_NAME);
        rcu_free_test_mod(mod);
        return NULL;
    }
    if ((rcu_init_mod(mod, init, destroy, name)) == RCU_E_NG) {
        rcu_free_test_mod(mod);
        return NULL;
    }
    RCU_LOG_DEBUG("Test module created : %s", mod->name);
    return mod;
}

RCU_API int rcu_destroy_test_mod(rcu_module *mod) {
    rcu_list *cursor;
    rcu_test *func;
    rcu_test_machine *machine;
    rcu_init();
    machine = &the_test_machine;
    if (mod == NULL) {
        RCU_SET_ERCD(RCU_E_INVMOD);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    RCU_LOG_DEBUG("Destroying test module : %s", mod->name);

    RCU_FOR_EACH_ENTRY(&mod->func_list, cursor) {
        func = (rcu_test*) cursor;
        RCU_SAVE_CURSOR(cursor)
        rcu_remove_list(cursor);
        rcu_free_test_func(func);
        RCU_RESTORE_CURSOR(cursor)
    }
    /* Unlink this test module */
    rcu_remove_list(&mod->link);
    /* Do not deallocate default test module! */
    if (strcmp(mod->name, RCU_DEFAULT_MODULE_NAME)) {
        RCU_LOG_DEBUG("Test module destroyed : %s", mod->name);
        rcu_free_test_mod(mod);
    }
    return RCU_E_OK;
}

int rcu_add_fail_rec_to_mod(rcu_module *mod, const char *info,
        const char *filepath, const int line_no, int fatal) {
    return (rcu_add_fail_rec_impl(&mod->fail_rec_list, info, filepath, line_no,
            fatal));
}

int rcu_del_all_fail_rec_from_mod(rcu_module *mod) {
    return (rcu_del_all_fail_rec_impl(&mod->fail_rec_list));
}

rcu_test *rcu_srch_test_func_by_name(rcu_module *mod,
        const char *name) {
    rcu_list *cursor;
    rcu_test *func;

    RCU_FOR_EACH_ENTRY(&mod->func_list, cursor) {
        func = (rcu_test*) cursor;
        if (!strcmp(func->name, name)) {
            return (func);
        }
    }
    return (NULL);
}

int rcu_run_test_mod(rcu_module *module) {
    rcu_test_machine *machine = NULL;
    char ts_buff[RCU_TSTAMP_BUFF_SIZE];
    rcu_module *mod;

    rcu_init();
    machine = &the_test_machine;
    mod = (module == NULL) ? &machine->def_mod : module;

    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO("Test run started %s", ts_buff);
    rcu_restart_mach(machine);
    RCU_SET_RUN_LEVEL(machine, RCU_RUN_LEVEL_MOD);
    RCU_SET_CURR_MOD(machine, mod);
    rcu_reset_all_run_stat();
    rcu_run_test_mod_impl(machine, mod);
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO("Test run finished %s", ts_buff);
    rcu_stop_mach(machine);
    rcu_gen_test_run_report(machine);
    return RCU_E_OK;
}

RCU_API int rcu_run_test_mod_by_name(const char *name) {
    rcu_module *mod = NULL;
    rcu_registry *which_reg = NULL;

    rcu_init();
    if (name == NULL) {
        mod = rcu_get_default_mod();
    } else {
        mod = rcu_srch_mod_by_name_global(name, &which_reg);
    }
    if (mod == NULL) {
        RCU_SET_ERCD(RCU_E_INVMOD);
        RCU_LOG_WARN("%s (%s)", RCU_GET_ERR_MSG(), name);
        return RCU_E_NG;
    }
    return rcu_run_test_mod(mod);
}

rcu_test *rcu_srch_test_func_entry(rcu_module *mod,
        rcu_generic_function entry) {

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&mod->func_list, cursor) {
        rcu_test *func = (rcu_test*) cursor;
        if (func->entry == entry) {
            return func;
        }
    }
    return NULL;
}

int rcu_run_test_mod_impl(rcu_test_machine *machine, rcu_module *mod) {
    rcu_list *cursor3 = NULL;
    rcu_test *func = NULL;
    rcu_registry *reg = NULL;
    int run_event;
    mod = (mod == NULL) ? rcu_get_default_mod() : mod;
    /** Execute test run hook if executed at module level */
    if (RCU_GET_RUN_LEVEL(machine) == RCU_RUN_LEVEL_MOD) {
        if (machine->run_hook != NULL) {
            run_event = RCU_TEST_RUN_STARTED;
            machine->run_hook(&run_event);
        }
        machine->nr_failed_reg = 0;
        machine->nr_succ_reg = 0;
        machine->nr_failed_test = 0;
        machine->nr_succ_test = 0;
        mod->nr_failed_test = 0;
        mod->nr_succ_test = 0;
    }
    reg = RCU_GET_CURR_REG(machine);
    RCU_LOG_DEBUG("Running tests from  module : %s", mod->name);
    if (mod->init != NULL) {
        RCU_TRY
        {
            RCU_SET_RUN_CTX(machine, RCU_RUN_CTX_MOD_INIT);
            mod->init(NULL);
        }

        RCU_CATCH(e) {
            RCU_LOG_WARN("Test module %s setup function failed", mod->name);
            /** Invoke the test run hook here */
            if (RCU_GET_RUN_LEVEL(machine) == RCU_RUN_LEVEL_MOD) {
                if (machine->run_hook != NULL) {
                    run_event = RCU_TEST_RUN_FINISHED;
                    machine->run_hook(&run_event);
                }
            }
            return RCU_E_NG;
        }
        RCU_END_CATCH
    }

    RCU_FOR_EACH_ENTRY(&mod->func_list, cursor3) {
        func = (rcu_test*) cursor3;
        rcu_run_test_func_impl(machine, func);
        /** If the function's init or destroy function failed, override the test
         *  status to failed even if the test actually succeeded.
         */
        if (RCU_IS_INIT_FAILED(func) || RCU_IS_DESTROY_FAILED(func)) {
            RCU_SET_RUN_STAT(func, RCU_RUN_STAT_TEST_FAILED);
            RCU_INCR(mod->nr_failed_test);
        } else {
            if (RCU_IS_TEST_FAILED(func)) {
                RCU_INCR(mod->nr_failed_test);
            } else {
                RCU_INCR(mod->nr_succ_test);
            }
        }
    }
    if (mod->destroy != NULL) {
        RCU_SET_RUN_CTX(machine, RCU_RUN_CTX_MOD_DESTROY);
        RCU_TRY
        {
            mod->destroy(NULL);
        }

        RCU_CATCH(e) {
            RCU_LOG_WARN("Test module teardown function failed : %s", mod->name);
            /** Invoke test run hook before we return */
            if (RCU_GET_RUN_LEVEL(machine) == RCU_RUN_LEVEL_MOD) {
                if (machine->run_hook != NULL) {
                    run_event = RCU_TEST_RUN_FINISHED;
                    machine->run_hook(&run_event);
                }
            }
        }
        return RCU_E_NG;

        RCU_END_CATCH
    }

    /**
     * If we arrive here, then the module's init and destroy functions
     * completed successfully.
     */

    if (RCU_GET_RUN_LEVEL(machine) == RCU_RUN_LEVEL_MOD) {
        if (machine->run_hook != NULL) {
            run_event = RCU_TEST_RUN_FINISHED;
            machine->run_hook(&run_event);
        }
    }

    /* the module will be marked failed if at least a test failed */
    if (mod->nr_failed_test == 0) {
        RCU_SET_RUN_STAT(mod, RCU_RUN_STAT_TEST_SUCC);
    } else {
        RCU_SET_RUN_STAT(mod, RCU_RUN_STAT_TEST_FAILED);
    }

    /* Increment the total number of failed test */
    RCU_INCR_BY(machine->nr_failed_test, mod->nr_failed_test)
    RCU_INCR_BY(machine->nr_succ_test, mod->nr_succ_test)

    return RCU_E_OK;
}

