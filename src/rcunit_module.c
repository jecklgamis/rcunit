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

extern struct rcu_module *rcu_search_module_by_name(struct rcu_registry *reg, const char *mod_name);

RCU_API struct rcu_module *rcu_get_default_module() {
    rcu_init();
    return &the_test_engine.def_module;
}

RCU_API struct rcu_module *rcu_get_module(const char *name) {
    struct rcu_module *module;
    rcu_init();
    if (name) {
        if ((module = rcu_search_module_by_name(&the_test_engine.def_reg, name))) {
            return module;
        }
        module = rcu_create_test_module(name, NULL, NULL);
        rcu_add_test_module(module);
        return module;
    }
    return NULL;
}

RCU_API void rcu_set_module_fixture(struct rcu_module *module, rcu_generic_function setup,
                             rcu_generic_function teardown) {
    rcu_init();
    module->init = setup;
    module->destroy = teardown;
}

RCU_API void rcu_set_module_fixture_all(struct rcu_module *module, rcu_generic_function setup,
    rcu_generic_function teardown) {
    rcu_init();
    module->init_all = setup;
    module->destroy_all = teardown;
}

struct rcu_module *rcu_alloc_test_module(unsigned int nr_module) {
    struct rcu_module *module = NULL;

    if (nr_module == 0) {
        return NULL;
    }
    module = (struct rcu_module *) rcu_malloc(sizeof(struct rcu_module) * nr_module);
    if (module) {
        memset(module, 0, sizeof(struct rcu_module));
    }
    return module;
}

int rcu_free_test_module(struct rcu_module *module) {
    if (!module) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMOD));
        return RCU_E_NG;
    }
    rcu_free(module);
    return RCU_E_OK;
}

int rcu_init_module(struct rcu_module *module, rcu_generic_function init, rcu_generic_function destroy, const char *name) {
    int name_len = 0;
    if (!module) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMOD));
        return RCU_E_NG;
    }
    if (!name) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMODNAME));
        return RCU_E_NG;
    }
    memset(module, 0, sizeof(struct rcu_module));
    rcu_init_list(&module->link);
    rcu_init_list(&module->func_list);
    module->init = init;
    module->destroy = destroy;
    name_len = strlen(name) > RCU_TEST_MODULE_NAME_LENGTH ? RCU_TEST_MODULE_NAME_LENGTH
                                                          : strlen(name);
    strncpy(module->name, name, name_len);
    return RCU_E_OK;
}

struct rcu_module *rcu_create_test_module(const char *name, rcu_generic_function init,
                             rcu_generic_function destroy) {
    struct rcu_module *module;
    struct rcu_test_engine *engine = &the_test_engine;
    rcu_init();
    if ((!(module = rcu_alloc_test_module(1)))) {
        RCU_SET_ERROR_CODE(RCU_E_NOMEM);
        RCU_LOG_WARN("%s", RCU_GET_ERR_MSG());
        return NULL;
    }
    if (name && !strcmp(name, RCU_DEFAULT_MODULE_NAME)) {
        RCU_SET_ERROR_CODE(RCU_E_INVMODNAME);
        RCU_LOG_WARN("%s (%s is reserved)", RCU_GET_ERR_MSG(), RCU_DEFAULT_MODULE_NAME);
        rcu_free_test_module(module);
        return NULL;
    }
    if ((rcu_init_module(module, init, destroy, name)) == RCU_E_NG) {
        rcu_free_test_module(module);
        return NULL;
    }
    RCU_LOG_DEBUG("Test module created : %s", module->name);
    return module;
}

int rcu_destroy_test_module(struct rcu_module *module) {
    struct rcu_list *cursor;
    struct rcu_test *func;
    struct rcu_test_engine *engine;
    rcu_init();
    engine = &the_test_engine;
    if (!module) {
        RCU_SET_ERROR_CODE(RCU_E_INVMOD);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    RCU_LOG_DEBUG("Destroying test module : %s", module->name);

    RCU_FOR_EACH_ENTRY(&module->func_list, cursor) {
        func = (struct rcu_test *) cursor;
        RCU_SAVE_CURSOR(cursor)
            rcu_remove_list(cursor);
            rcu_free_test_func(func);
        RCU_RESTORE_CURSOR(cursor)
    }
    rcu_remove_list(&module->link);
    if (strcmp(module->name, RCU_DEFAULT_MODULE_NAME)) {
        RCU_LOG_DEBUG("Test module destroyed : %s", module->name);
        rcu_free_test_module(module);
    }
    return RCU_E_OK;
}

int rcu_add_fail_rec_to_module(struct rcu_module *module, const char *info, const char *filepath, const int line_no, int fatal) {
    return rcu_add_fail_rec_impl(&module->fail_rec_list, info, filepath, "", line_no);
}

int rcu_del_all_fail_rec_from_module(struct rcu_module *module) {
    return (rcu_del_all_fail_rec_impl(&module->fail_rec_list));
}

struct rcu_test *rcu_search_test_func_by_name(struct rcu_module *module,
                                     const char *name) {
    struct rcu_list *cursor;
    struct rcu_test *func;

    RCU_FOR_EACH_ENTRY(&module->func_list, cursor) {
        func = (struct rcu_test *) cursor;
        if (!strcmp(func->name, name)) {
            return (func);
        }
    }
    return NULL;
}

int rcu_run_test_module(struct rcu_module *module) {
    struct rcu_test_engine *engine = NULL;
    char ts_buff[RCU_TSTAMP_BUFF_SIZE];

    rcu_init();
    engine = &the_test_engine;
    module = (!module) ? &engine->def_module : module;

    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO("Test run started %s", ts_buff);
    rcu_restart_engine(engine);
    engine->run_level = RCU_RUN_LEVEL_MODULE;
    RCU_SET_CURRENT_MODULE(engine, module);
    rcu_run_test_module_impl(engine, module);
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO("Test run finished %s", ts_buff);
    rcu_stop_engine(engine);
    rcu_gen_test_run_report(engine);
    return RCU_E_OK;
}

int rcu_run_test_module_by_name(const char *name) {
    struct rcu_module *module = NULL;

    rcu_init();
    if (!name) {
        module = rcu_get_default_module();
    } else {
        module = rcu_search_module_by_name(&the_test_engine.def_reg, name);
    }
    if (!module) {
        RCU_SET_ERROR_CODE(RCU_E_INVMOD);
        RCU_LOG_WARN("%s (%s)", RCU_GET_ERR_MSG(), name);
        return RCU_E_NG;
    }
    return rcu_run_test_module(module);
}

struct rcu_test *rcu_search_test_func_entry(struct rcu_module *module,
                                   rcu_generic_function entry) {

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&module->func_list, cursor) {
        struct rcu_test *func = (struct rcu_test *) cursor;
        if (func->entry == entry) {
            return func;
        }
    }
    return NULL;
}

int rcu_run_test_module_impl(struct rcu_test_engine *engine, struct rcu_module *module) {
    struct rcu_list *cursor3 = NULL;
    struct rcu_test *func = NULL;
    struct rcu_registry *reg = NULL;
    int run_event;
    module = (!module) ? rcu_get_default_module() : module;
    /** Execute test run hook if executed at module level */
    if (engine->run_level == RCU_RUN_LEVEL_MODULE) {
        if (engine->run_hook) {
            run_event = RCU_TEST_RUN_STARTED;
            engine->run_hook(&run_event);
        }
        engine->nr_failed_reg = 0;
        engine->nr_succ_reg = 0;
        engine->nr_failed_test = 0;
        engine->nr_succ_test = 0;
        module->nr_failed_test = 0;
        module->nr_succ_test = 0;
    }
    reg = RCU_GET_CURRENT_REG(engine);
    RCU_LOG_DEBUG("Running tests from  module : %s", module->name);
    if (module->init_all) {
        RCU_TRY
                {
                    RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_MODULE_INIT);
                    module->init_all(NULL);
                }

            RCU_CATCH(e)
                {
                    RCU_LOG_WARN("Test module %s setup function failed", module->name);
                    /** Invoke the test run hook here */
                    if (engine->run_level == RCU_RUN_LEVEL_MODULE) {
                        if (engine->run_hook) {
                            run_event = RCU_TEST_RUN_FINISHED;
                            engine->run_hook(&run_event);
                        }
                    }
                    return RCU_E_NG;
                }
        RCU_END_CATCH
    }

    RCU_FOR_EACH_ENTRY(&module->func_list, cursor3) {
        func = (struct rcu_test *) cursor3;
        rcu_run_test_func_impl(engine, func);
        /** If the function's init or destroy function failed, override the test
         *  status to failed even if the test actually succeeded.
         */
        if (RCU_IS_INIT_FAILED(func) || RCU_IS_DESTROY_FAILED(func)) {
            func->run_stat = RCU_RUN_STAT_TEST_FAILED;
            RCU_INCR(module->nr_failed_test);
        } else {
            if (func->run_stat == RCU_RUN_STAT_TEST_FAILED) {
                RCU_INCR(module->nr_failed_test);
            } else {
                RCU_INCR(module->nr_succ_test);
            }
        }
    }

    if (module->destroy_all) {
        RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_MODULE_DESTROY);
        RCU_TRY
                {
                    module->destroy_all(NULL);
                }

            RCU_CATCH(e)
                {
                    RCU_LOG_WARN("Test module teardown function failed : %s", module->name);
                    /** Invoke test run hook before we return */
                    if (engine->run_level == RCU_RUN_LEVEL_MODULE) {
                        if (engine->run_hook) {
                            run_event = RCU_TEST_RUN_FINISHED;
                            engine->run_hook(&run_event);
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

    if (engine->run_level == RCU_RUN_LEVEL_MODULE) {
        if (engine->run_hook) {
            run_event = RCU_TEST_RUN_FINISHED;
            engine->run_hook(&run_event);
        }
    }

    /* the module will be marked failed if at least a test failed */
    if (module->nr_failed_test == 0) {
        module->run_stat = RCU_RUN_STAT_TEST_SUCC;
    } else {
        module->run_stat = RCU_RUN_STAT_TEST_FAILED;
    }

    /* Increment the total number of failed test */
    RCU_INCR_BY(engine->nr_failed_test, module->nr_failed_test)
    RCU_INCR_BY(engine->nr_succ_test, module->nr_succ_test)

    return RCU_E_OK;
}

