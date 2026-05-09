/*
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rcunit.h"

extern struct rcu_test *rcu_alloc_test_func(int nr_func);

struct rcu_test *rcu_search_test_func_entry(struct rcu_module *module, rcu_generic_function entry);

RCU_API int rcu_add_test(rcu_generic_function test) {
    return rcu_add_test_func(rcu_get_default_module(), test, NULL, NULL, NULL);
}

RCU_API int rcu_add_test_to_module(struct rcu_module *module, rcu_generic_function test) {
    return rcu_add_test_func(module, test, NULL, NULL, NULL);
}

RCU_API int rcu_add_test_fixture(rcu_generic_function test, rcu_generic_function setup, rcu_generic_function teardown) {
    return rcu_add_test_func(rcu_get_default_module(), test, setup, teardown, NULL);
}

RCU_API int rcu_add_test_fixture_to_module(struct rcu_module *module, rcu_generic_function test,
                                    rcu_generic_function setup, rcu_generic_function teardown) {
    return rcu_add_test_func(module, test, setup, teardown, NULL);
}

RCU_API int rcu_add_test_func(struct rcu_module *module, rcu_generic_function entry, rcu_generic_function init,
                              rcu_generic_function destroy, const char *name) {
    struct rcu_test *func = NULL;
    struct rcu_test_engine *engine = NULL;
    int name_len = 0;

    rcu_init();
    engine = &the_test_engine;
    if (!entry) {
        RCU_SET_ERROR_CODE(RCU_E_INVFUNCENTRY);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    module = (!module) ? rcu_get_default_module() : module;
    func = rcu_search_test_func_entry(module, entry);
    if (func) {
        RCU_LOG_WARN("Similar test %s exists in %s", func->name, module->name);
    }
    if ((!(func = rcu_alloc_test_func(1)))) {
        RCU_SET_ERROR_CODE(RCU_E_NOMEM);
        RCU_LOG_WARN("%s", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    rcu_init_list(&func->link);
    char test_name[RCU_TEST_FUNCTION_NAME_LENGTH];
    if (!name) {
        memset(test_name, 0, RCU_TEST_FUNCTION_NAME_LENGTH);
        sprintf(test_name, "%p", entry);
        strncpy(func->name, test_name, strlen(test_name));
    } else {
        name_len = strlen(name) > RCU_TEST_FUNCTION_NAME_LENGTH ?
                   RCU_TEST_FUNCTION_NAME_LENGTH : strlen(name);
        strncpy(func->name, name, name_len);
    }
    func->entry = entry;
    func->module = module;

    rcu_init_list(&func->fail_rec_list);
    rcu_insert_list(&module->func_list, &func->link);

    func->run_stat = RCU_RUN_STAT_NOTTESTED;
    RCU_INCR(module->nr_test);
    RCU_LOG_DEBUG("Test %s added to %s", func->name, module->name);
    return RCU_E_OK;
}

struct rcu_test *rcu_alloc_test_func(int nr_func) {
    struct rcu_test *func = NULL;
    if (nr_func <= 0) {
        return NULL;
    }
    func = (struct rcu_test *) rcu_malloc(sizeof(struct rcu_test) * nr_func);
    if (func) {
        memset(func, 0, sizeof(struct rcu_test));
    }
    return func;
}

int rcu_free_test_func(struct rcu_test *func) {
    if (!func) {
        RCU_LOG_WARN("%s", RCU_GET_ERR_MSG_OF(RCU_E_INVFUNC));
        return RCU_E_NG;
    }
    RCU_LOG_DEBUG("De-allocating test : %s", func->name);
    rcu_free(func);
    return RCU_E_OK;
}

int rcu_add_fail_rec_to_func(struct rcu_test *func, const char *info, const char *filepath, const int line_no) {
    return rcu_add_fail_rec_impl(&func->fail_rec_list, info, filepath, "", line_no);
}

int rcu_del_all_fail_rec_from_func(struct rcu_test *func) {
    return (rcu_del_all_fail_rec_impl(&func->fail_rec_list));
}

int rcu_run_test_func_impl(struct rcu_test_engine *engine, struct rcu_test *func) {
    func->run_stat = RCU_RUN_STAT_TEST_FAILED;
    RCU_SET_CURRENT_FUNC(engine, func);
    func->nr_fail_assert = 0;
    func->nr_succ_assert = 0;

    if (func->module->init) {
        RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_FUNC_INIT);
        RCU_TRY
                {
                    func->module->init(NULL);
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

    RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_FUNC)
    RCU_LOG_DEBUG("Running test function : %s", func->name);

    RCU_TRY
            {
                func->entry(NULL);
                func->run_stat = RCU_RUN_STAT_TEST_SUCC;
            }

        RCU_CATCH(e)
            {
                if (e->id != RCU_EXCP_ASSERTIONFAILURE) {
                    RCU_LOG_ERROR("Caught %s in %s", e->name, func->name);
                }
            }
    RCU_END_CATCH

    if (func->run_stat == RCU_RUN_STAT_TEST_SUCC) {
        if (func->nr_succ_assert == 0 && func->nr_fail_assert == 0) {
            RCU_LOG_WARN("No executed assertions in %s", func->name);
            func->run_stat = RCU_RUN_STAT_TEST_FAILED;
        } else {
            RCU_LOG_INFO("Test %s OK", func->name);
        }
    }

    if (func->module->destroy) {
        RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_FUNC_DESTROY);
        RCU_TRY
                {
                    func->module->destroy(NULL);
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

