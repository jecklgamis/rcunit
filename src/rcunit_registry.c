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

int rcu_init_reg(struct rcu_registry *reg, const char *name) {
    int name_len = 0;
    if (!reg) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVREG));
        return RCU_E_NG;
    }
    if (!name) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVREGNAME));
        return RCU_E_NG;
    }
    memset(reg, 0, sizeof(struct rcu_registry));
    rcu_init_list(&reg->link);
    rcu_init_list(&reg->mod_list);
    name_len = strlen(name) > RCU_TEST_REGISTRY_NAME_LENGTH ?
               RCU_TEST_REGISTRY_NAME_LENGTH : strlen(name);
    strncpy(reg->name, name, name_len);
    RCU_LOG_DEBUG("Initialized registry : %s", reg->name);
    return RCU_E_OK;
}

RCU_API struct rcu_registry *rcu_get_default_reg() {
    rcu_init();
    return &the_test_engine.def_reg;
}

RCU_API int rcu_add_test_module(struct rcu_module *module) {
    rcu_init();
    return rcu_add_module_to_reg(rcu_get_default_reg(), module);
}

RCU_API int rcu_add_module_to_reg(struct rcu_registry *reg, struct rcu_module *module) {
    rcu_init();
    if (!module) {
        RCU_SET_ERROR_CODE(RCU_E_INVMOD);
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG());
        return RCU_E_NG;
    }
    reg = (!reg) ? rcu_get_default_reg() : reg;
    rcu_init_list(&module->link);
    rcu_insert_list(&reg->mod_list, &module->link);
    RCU_INCR(reg->nr_module);
    rcu_init_list(&module->fail_rec_list);
    RCU_LOG_DEBUG("%s added to %s", module->name, reg->name);
    return RCU_E_OK;
}

struct rcu_module *rcu_search_module_by_name(struct rcu_registry *reg, const char *mod_name) {
    struct rcu_list *cursor = NULL;
    struct rcu_module *search_module = NULL;

    if (!mod_name) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMODNAME));
        return NULL;
    }

    RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor) {
        search_module = (struct rcu_module *) cursor;
        if (!strcmp(search_module->name, mod_name)) {
            return (search_module);
        }
    }
    return NULL;
}

RCU_API void rcu_dump_test_reg(struct rcu_registry *reg) {
    struct rcu_list *cursor1 = NULL;
    struct rcu_list *cursor2 = NULL;
    struct rcu_test *func = NULL;
    struct rcu_module *module = NULL;

    reg = !reg ? rcu_get_default_reg() : reg;
    RCU_LOG_INFO("|   +- registry [name = %s]", reg->name);

    RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor1) {
        module = (struct rcu_module *) cursor1;
        RCU_LOG_INFO("|   |   +- module [name = %s]", module->name);

        RCU_FOR_EACH_ENTRY(&module->func_list, cursor2) {
            func = (struct rcu_test *) cursor2;
            RCU_LOG_INFO("|   |   |   +- test [name = %s]", func->name);
        }
    }
}

int rcu_run_test_reg_impl(struct rcu_test_engine *engine, struct rcu_registry *reg) {
    struct rcu_list *cursor2, *cursor3;
    struct rcu_module *module;
    int run_event;
    struct rcu_test *func;

    reg = (!reg) ? &engine->def_reg : reg;
    if (engine->run_level == RCU_RUN_LEVEL_REGISTRY) {
        if (engine->run_hook) {
            run_event = RCU_TEST_RUN_STARTED;
            engine->run_hook(&run_event);
        }
        engine->nr_failed_reg = 0;
        engine->nr_succ_reg = 0;
        engine->nr_failed_test = 0;
        engine->nr_succ_test = 0;
        reg->nr_failed_module = 0;
        reg->nr_succ_module = 0;
    }
    RCU_LOG_DEBUG("Running tests from registry : %s", reg->name);

    RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor2) {
        module = (struct rcu_module *) cursor2;
        RCU_SET_CURRENT_MODULE(engine, module);
        rcu_run_test_module_impl(engine, module);
        if (RCU_IS_INIT_FAILED(module) || RCU_IS_DESTROY_FAILED(module)) {
            module->run_stat = RCU_RUN_STAT_TEST_FAILED;
            RCU_FOR_EACH_ENTRY(&module->func_list, cursor3) {
                func = (struct rcu_test *) cursor3;
                func->run_stat = RCU_RUN_STAT_TEST_FAILED;
                func->nr_fail_assert = 0;
                func->nr_succ_assert = 0;
                rcu_del_all_fail_rec_from_func(func);
            }
        } else {
            if (module->run_stat == RCU_RUN_STAT_TEST_FAILED) {
                RCU_INCR(reg->nr_failed_module);
            } else {
                RCU_INCR(reg->nr_succ_module);
            }
        }
    }
    if (engine->run_level == RCU_RUN_LEVEL_REGISTRY) {
        if (engine->run_hook) {
            run_event = RCU_TEST_RUN_FINISHED;
            engine->run_hook(&run_event);
        }
    }
    if (reg->nr_failed_module > 0) {
        RCU_INCR(engine->nr_failed_reg);
    } else {
        RCU_INCR(engine->nr_succ_reg);
    }
    return RCU_E_OK;
}
