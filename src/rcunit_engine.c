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

int rcu_init_test_engine(struct rcu_test_engine *engine) {
    if (engine) {
        RCU_LOG_DEBUG("Initializing test engine");
        memset(engine, 0, sizeof(struct rcu_test_engine));
        rcu_init_list(&engine->ae.assert_list);
        engine->name = RCU_DEFAULT_ENGINE_NAME;
        engine->init_done = 1;
        RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_UNKNOWN);
#if RCU_ENABLE_MTRACE
        rcu_init_mtrace();
#endif
        rcu_init_exception();
        rcu_init_reg(&engine->def_reg, RCU_DEFAULT_REGISTRY_NAME);
        rcu_init_module(&engine->def_module, NULL, NULL, RCU_DEFAULT_MODULE_NAME);
        rcu_add_module_to_reg(&engine->def_reg, &engine->def_module);
    }
    return RCU_E_OK;
}

int rcu_destroy_test_engine(struct rcu_test_engine *engine) {
    RCU_LOG_DEBUG("Destroying test engine");
    rcu_destroy_exception();
    rcu_del_all_fail_rec(engine);
    rcu_destroy_test_dbase(engine);
#if RCU_ENABLE_MTRACE
    rcu_destroy_mtrace();
#endif
    engine->init_done = 0;
    return RCU_E_OK;
}

int rcu_destroy_test_dbase(struct rcu_test_engine *engine) {
    struct rcu_list *cursor = NULL;
    struct rcu_module *module = NULL;
    RCU_LOG_DEBUG("Destroying test database");
    if (engine) {
        if (!rcu_is_engine_initialized(engine)) {
            RCU_LOG_WARN("%s", RCU_GET_ERR_MSG_OF(RCU_E_ENGNOINIT));
            return RCU_E_NG;
        }
        RCU_FOR_EACH_ENTRY(&engine->def_reg.mod_list, cursor) {
            module = (struct rcu_module *) cursor;
            RCU_SAVE_CURSOR(cursor)
                rcu_remove_list(cursor);
                rcu_destroy_test_module(module);
            RCU_RESTORE_CURSOR(cursor)
        }
    }
    return RCU_E_OK;
}

int rcu_is_engine_initialized(struct rcu_test_engine *engine) {
    return engine && engine->init_done;
}

int rcu_restart_engine(struct rcu_test_engine *engine) {
    RCU_LOG_DEBUG("Restarting engine");
    return RCU_E_OK;
}

int rcu_stop_engine(struct rcu_test_engine *engine) {
    RCU_LOG_DEBUG("Stopping engine");
    rcu_stop_assert_engine(engine);
    return RCU_E_OK;
}

int rcu_run_tests_impl(struct rcu_test_engine *engine) {
    int run_event;

    if (engine) {
        if (engine->run_hook) {
            run_event = RCU_TEST_RUN_STARTED;
            engine->run_hook(&run_event);
        }
        RCU_LOG_DEBUG("Running test engine : %s", engine->name);
        RCU_SET_CURRENT_REG(engine, &engine->def_reg);
        rcu_run_test_reg_impl(engine, &engine->def_reg);
        if (engine->run_hook) {
            run_event = RCU_TEST_RUN_FINISHED;
            engine->run_hook(&run_event);
        }
    }
    return RCU_E_OK;
}
