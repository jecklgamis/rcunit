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

extern struct rcu_test *rcu_search_test_func_entry(struct rcu_module *module, rcu_generic_function entry);

extern struct rcu_test *rcu_search_test_func_by_name(struct rcu_module *module, const char *name);

int rcu_get_timestamp(char *ts_buff, int ts_buff_len) {
    time_t t;
    int len;

    if (!ts_buff || ts_buff_len < RCU_TSTAMP_BUFF_SIZE) {
        RCU_LOG_WARN("Null timestamp buffer or insufficient buffer length");
        return RCU_E_NG;
    }
    memset(ts_buff, 0, ts_buff_len);
    time(&t);
    sprintf(ts_buff, "%s", ctime(&t));
    len = strlen(ts_buff);
    memset(&ts_buff[len - 1], 0, 1);
    return RCU_E_OK;
}

void rcu_dump_test_registry_impl(struct rcu_test_engine *engine) {
    if (engine) {
        if (!rcu_is_engine_initialized(engine)) {
            RCU_SET_ERROR_CODE(RCU_E_ENGNOINIT);
            RCU_LOG_WARN("%s", RCU_GET_ERR_MSG());
        }
        RCU_LOG_INFO("+- Test Entity Database");
        rcu_dump_test_reg(&engine->def_reg);
    }
}

void rcu_dump_test_registry() {
    rcu_init();
    rcu_dump_test_registry_impl(&the_test_engine);
}

int rcu_get_nr_tests() {
    int nr_tests = 0;
    struct rcu_test_engine *engine = &the_test_engine;

    rcu_init();
    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&engine->def_reg.mod_list, mod_cursor) {
        struct rcu_module *module = (struct rcu_module *) mod_cursor;
        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&module->func_list, test_cursor) {
            nr_tests++;
        }
    }
    return nr_tests;
}

int rcu_get_nr_mods() {
    int nr_mods = 0;
    struct rcu_test_engine *engine = &the_test_engine;

    rcu_init();
    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&engine->def_reg.mod_list, mod_cursor) {
        nr_mods++;
    }
    return nr_mods;
}
