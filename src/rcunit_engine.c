/*
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rcunit.h"

extern rcu_registry *rcu_srch_reg_by_name(rcu_test_engine *engine, const char *reg_name);

int rcu_add_test_reg(rcu_registry *reg) {
    rcu_test_engine *engine = NULL;
    rcu_registry *srch_reg = NULL;

    rcu_init();
    engine = &the_test_engine;
    if (!rcu_is_mach_initialized(engine)) {
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        return RCU_E_NG;
    }
    if (reg == NULL) {
        RCU_SET_ERCD(RCU_E_INVREG);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    if (rcu_reg_exists(engine, reg)) {
        RCU_SET_ERCD(RCU_E_REGEXISTS);
        RCU_LOG_WARN("%s (%s)", RCU_GET_ERR_MSG(), reg->name);
        return RCU_E_NG;
    }
    srch_reg = rcu_srch_reg_by_name(engine, reg->name);
    if (srch_reg != NULL) {
        RCU_SET_ERCD(RCU_E_REGEXISTS);
        RCU_LOG_WARN("%s (%s)", RCU_GET_ERR_MSG(), srch_reg->name);
        return RCU_E_NG;
    }
    rcu_init_list(&reg->link);
    rcu_insert_list(&engine->reg_list, &reg->link);
    RCU_INCR(engine->nr_reg);
    RCU_LOG_DEBUG("Test registry added : %s", reg->name);
    return RCU_E_OK;
}

int rcu_init_test_mach(rcu_test_engine *engine) {
    if (engine != NULL) {
        RCU_LOG_DEBUG("Initializing test engine");
        memset(engine, 0x00, sizeof(rcu_test_engine));
        rcu_init_list(&engine->reg_list);
        rcu_init_list(&engine->ae.assert_list);
        engine->name = RCU_DEFAULT_MACHINE_NAME;
        engine->init_done = 1;
        RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_UNKNOWN);
#if RCU_ENABLE_MTRACE
        rcu_init_mtrace();
#endif
        rcu_init_exception();
        rcu_init_reg(&engine->def_reg, RCU_DEFAULT_REGISTRY_NAME);
        rcu_add_test_reg(&engine->def_reg);
        rcu_init_mod(&engine->def_mod, NULL, NULL, RCU_DEFAULT_MODULE_NAME);
        rcu_add_mod_to_reg(&engine->def_reg, &engine->def_mod);
    }
    return RCU_E_OK;
}

int rcu_destroy_test_mach(rcu_test_engine *engine) {
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

int rcu_destroy_test_dbase(rcu_test_engine *engine) {
    rcu_list *cursor = NULL;
    rcu_registry *reg = NULL;
    RCU_LOG_DEBUG("Destroying test database");
    if (engine != NULL) {
        if (!rcu_is_mach_initialized(engine)) {
            RCU_LOG_WARN("%s", RCU_GET_ERR_MSG_OF(RCU_E_MACHNOINIT));
            return RCU_E_NG;
        }

        RCU_FOR_EACH_ENTRY(&engine->reg_list, cursor) {
            reg = (rcu_registry *) cursor;
            RCU_SAVE_CURSOR(cursor)
                rcu_remove_list(cursor);
                rcu_destroy_test_reg(reg);
            RCU_RESTORE_CURSOR(cursor)
        }
    }
    return RCU_E_OK;
}

int rcu_is_mach_initialized(rcu_test_engine *engine) {
    return engine != NULL && engine->init_done;
}

int rcu_restart_mach(rcu_test_engine *engine) {
    RCU_LOG_DEBUG("Restarting engine");
    return RCU_E_OK;
}

int rcu_stop_mach(rcu_test_engine *engine) {
    RCU_LOG_DEBUG("Stopping engine");
    rcu_stop_assert_engine(engine);
    return RCU_E_OK;
}

int rcu_run_tests_impl(rcu_test_engine *engine) {
    rcu_list *cursor1 = NULL;
    rcu_registry *reg = NULL;
    int run_event;

    if (engine != NULL) {
        if (engine->run_hook != NULL) {
            run_event = RCU_TEST_RUN_STARTED;
            engine->run_hook(&run_event);
        }
        RCU_LOG_DEBUG("Running test engine : %s", engine->name);

        RCU_FOR_EACH_ENTRY(&engine->reg_list, cursor1) {
            reg = (rcu_registry *) cursor1;
            RCU_SET_CURR_REG(engine, reg);
            rcu_run_test_reg_impl(engine, reg);
        }
        if (engine->run_hook != NULL) {
            run_event = RCU_TEST_RUN_FINISHED;
            engine->run_hook(&run_event);
        }
    }
    return RCU_E_OK;
}

void rcu_reset_all_run_stat() {
    rcu_test_engine *engine = &the_test_engine;
    rcu_list *cursor1 = NULL;
    rcu_list *cursor2 = NULL;
    rcu_list *cursor3 = NULL;
    rcu_module *mod;
    rcu_registry *reg;
    rcu_test *func;

    RCU_FOR_EACH_ENTRY(&engine->reg_list, cursor1) {
        reg = (rcu_registry *) cursor1;

        RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor2) {
            mod = (rcu_module *) cursor2;

            RCU_FOR_EACH_ENTRY(&mod->func_list, cursor3) {
                func = (rcu_test *) cursor3;
                func->run_stat = RCU_RUN_STAT_NOTTESTED;
            }

        }
    }
}

rcu_registry *rcu_srch_reg_by_name(rcu_test_engine *engine, const char *reg_name) {
    rcu_list *cursor = NULL;
    rcu_registry *srch_reg = NULL;
    if (engine != NULL && reg_name != NULL) {

        RCU_FOR_EACH_ENTRY(&engine->reg_list, cursor) {
            srch_reg = (rcu_registry *) cursor;
            if (!strcmp(reg_name, srch_reg->name)) {
                return srch_reg;
            }
        }
    }
    return NULL;
}
