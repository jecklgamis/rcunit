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

extern rcu_test *
rcu_srch_test_func_entry_global(rcu_generic_function entry, rcu_module **which_mod, rcu_registry **which_reg);

extern rcu_test *rcu_srch_test_func_by_name_global(const char *name, rcu_module **which_mod, rcu_registry **which_reg);

extern rcu_test *rcu_alloc_test_func(int nr_func);

extern rcu_module *rcu_srch_mod_by_name_global(const char *mod_name, rcu_registry **which_reg);

rcu_test *rcu_srch_test_func_entry(rcu_module *mod, rcu_generic_function entry);

RCU_API int rcu_add_test(rcu_generic_function test) {
    return rcu_add_test_func(rcu_get_default_mod(), test, NULL, NULL, NULL);
}

RCU_API int rcu_add_test_to_mod(rcu_module *mod, rcu_generic_function test) {
    return rcu_add_test_func(mod, test, NULL, NULL);
}

RCU_API int rcu_add_test_fxt(rcu_generic_function test,
                             rcu_generic_function setup, rcu_generic_function teardown) {
    return rcu_add_test_func(rcu_get_default_mod(), test, setup, teardown);
}

RCU_API int rcu_add_test_fxt_to_mod(rcu_module *mod, rcu_generic_function test,
                                    rcu_generic_function setup, rcu_generic_function teardown) {
    return rcu_add_test_func(rcu_get_default_mod(), test, setup, teardown);
}

RCU_API int rcu_add_test_func(rcu_module *mod,
                              rcu_generic_function entry, rcu_generic_function init,
                              rcu_generic_function destroy, const char *name) {
    rcu_test *func = NULL;
    rcu_registry *which_reg = NULL;
    rcu_test_machine *machine = NULL;
    int name_len = 0;

    rcu_init();
    machine = &the_test_machine;
    if (entry == NULL) {
        RCU_SET_ERCD(RCU_E_INVFUNCENTRY);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    mod = (mod == NULL) ? rcu_get_default_mod() : mod;
    func = rcu_srch_test_func_entry(mod, entry);
    if (func != NULL) {
        RCU_LOG_WARN("Similar test %s exists in %s", func->name, mod->name);
    }
    if ((func = rcu_alloc_test_func(1)) == NULL) {
        RCU_SET_ERCD(RCU_E_NOMEM);
        RCU_LOG_WARN("%s", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    rcu_init_list(&func->link);
    char test_name[RCU_TEST_FUNCTION_NAME_LENGTH];
    if (name == NULL) {
        memset(test_name, 0, RCU_TEST_FUNCTION_NAME_LENGTH);
        sprintf(test_name, "%p", entry);
        strncpy(func->name, test_name, strlen(test_name));
    } else {
        name_len = strlen(name) > RCU_TEST_FUNCTION_NAME_LENGTH ?
                   RCU_TEST_FUNCTION_NAME_LENGTH : strlen(name);
        strncpy(func->name, name, name_len);
    }
    func->entry = entry;
    func->init = init;
    func->destroy = destroy;

    rcu_init_list(&func->fail_rec_list);
    rcu_insert_list(&mod->func_list, &func->link);

    RCU_SET_RUN_STAT(func, RCU_RUN_STAT_NOTTESTED);
    RCU_INCR(mod->nr_test);
    RCU_LOG_DEBUG("Test %s added to %s", func->name, mod->name);
    return RCU_E_OK;
}

RCU_API int rcu_add_test_func_tbl(rcu_module *mod,
                                  rcu_test_function_entry *func_tbl) {
    rcu_test_function_entry *cursor = NULL;
    int index;
    rcu_test_machine *machine = &the_test_machine;

    rcu_init();
    if (!rcu_is_mach_initialized(machine)) {
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        return RCU_E_NG;
    }
    if (func_tbl == NULL) {
        RCU_SET_ERCD(RCU_E_INVFUNCTABLE);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }

    mod = (mod == NULL) ? rcu_get_default_mod() : mod;

    RCU_FOR_EACH_FUNC_ENTRY(func_tbl, cursor, index) {
        if (cursor->entry != NULL) {
            rcu_add_test_func(mod, cursor->entry, cursor->init,
                              cursor->destroy, cursor->name);
        } else {
            RCU_LOG_WARN("Invalid test function entry. (index = %d)", index);
        }
    }
    return RCU_E_OK;
}

rcu_test *rcu_alloc_test_func(int nr_func) {
    rcu_test *func = NULL;
    if (nr_func <= 0) {
        return NULL;
    }
    func = (rcu_test *) rcu_malloc(sizeof(rcu_test) * nr_func);
    if (func != NULL) {
        memset(func, 0, sizeof(rcu_test));
    }
    return func;
}

int rcu_free_test_func(rcu_test *func) {
    if (func == NULL) {
        RCU_LOG_WARN("%s", RCU_GET_ERR_MSG_OF(RCU_E_INVFUNC));
        return RCU_E_NG;
    }
    RCU_LOG_DEBUG("De-allocating test : %s", func->name);
    rcu_free(func);
    return RCU_E_OK;
}

int rcu_add_fail_rec_to_func(rcu_test *func, const char *info,
                             const char *filepath, const int line_no) {
    return (rcu_add_fail_rec_impl(&func->fail_rec_list, info, filepath, line_no));
}

int rcu_del_all_fail_rec_from_func(rcu_test *func) {
    return (rcu_del_all_fail_rec_impl(&func->fail_rec_list));
}

int rcu_run_test_func_impl(rcu_test_machine *machine, rcu_test *func) {
    RCU_SET_RUN_STAT(func, RCU_RUN_STAT_TEST_FAILED);
    RCU_SET_CURR_FUNC(machine, func);
    RCU_RESET(func->nr_fail_assert);
    RCU_RESET(func->nr_succ_assert);

    if (func->init != NULL) {
        RCU_SET_RUN_CTX(machine, RCU_RUN_CTX_FUNC_INIT);
        RCU_TRY
                {
                    func->init(NULL);
                }

            RCU_CATCH(e)
                {
                    if (e->id != RCU_EXCP_ASSERTIONFAILURE) {
                        RCU_LOG_ERROR("Caught %s in %s setup", e->name, func->name);
                    }
                    return RCU_E_NG;
                }
        RCU_END_CATCH
    }

    RCU_SET_RUN_CTX(machine, RCU_RUN_CTX_FUNC)
    RCU_LOG_DEBUG("Running test function : %s", func->name);

    RCU_TRY
            {
                func->entry(NULL);
                RCU_SET_RUN_STAT(func, RCU_RUN_STAT_TEST_SUCC);
            }

        RCU_CATCH(e)
            {
                if (e->id != RCU_EXCP_ASSERTIONFAILURE) {
                    RCU_LOG_ERROR("Caught %s in %s", e->name, func->name);
                }
            }
    RCU_END_CATCH

    if (RCU_IS_TEST_SUCCEDED(func)) {
        if (func->nr_succ_assert == 0 && func->nr_fail_assert == 0) {
            RCU_LOG_WARN("No executed assertions", func->name);
            RCU_SET_RUN_STAT(func, RCU_RUN_STAT_TEST_FAILED);
        } else {
            RCU_LOG_INFO("Test %s OK", func->name);
        }
    }

    if (func->destroy != NULL) {
        RCU_SET_RUN_CTX(machine, RCU_RUN_CTX_FUNC_DESTROY);
        RCU_TRY
                {
                    func->destroy(NULL);
                }

            RCU_CATCH(e)
                {
                    if (e->id != RCU_EXCP_ASSERTIONFAILURE) {
                        RCU_LOG_ERROR("Caught %s in %s", e->name, func->name);
                    }
                    return RCU_E_NG;
                }
        RCU_END_CATCH
    }

    /* If we arrived here then that means, the test's init and destroy
     *  functions completed successfully and the test's  status
     *  is set properly in assertion function impl.
     */
    return RCU_E_OK;
}

