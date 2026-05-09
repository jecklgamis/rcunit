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

extern int rcu_gen_plaintext_report(struct rcu_test_engine *engine);

const char *rcu_get_stat_str(int stat) {
    switch (stat) {
        case RCU_RUN_STAT_NOTTESTED:
            return "Ignored";
        case RCU_RUN_STAT_TEST_SUCC:
            return "Passed";
        case RCU_RUN_STAT_TEST_FAILED:
            return "Failed";
        default:
            return "Unknown";
    }
}

/* This function prepares the test report statistics. No report generators
 * should modify the statistics */
void rcu_prepare_report(struct rcu_test_engine *engine) {
    struct rcu_registry *reg = &engine->def_reg;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&reg->mod_list, mod_cursor, mod_no) {
        struct rcu_module *module = (struct rcu_module *) mod_cursor;

        RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&module->func_list, func_cursor, func_no) {
            struct rcu_test *func = (struct rcu_test *) func_cursor;
            if (func->run_stat == RCU_RUN_STAT_TEST_SUCC) {
                RCU_INCR(module->nr_succ_test);
            } else if (func->run_stat == RCU_RUN_STAT_TEST_FAILED) {
                RCU_INCR(module->nr_failed_test);
            } else {
                RCU_LOG_WARN("Test with unknown test result : %s", func->name);
            }
        }
        if (module->run_stat == RCU_RUN_STAT_TEST_SUCC) {
            RCU_INCR(reg->nr_succ_module);
        }
        if (module->run_stat == RCU_RUN_STAT_TEST_FAILED) {
            RCU_INCR(reg->nr_failed_module);
        }
        RCU_INCR_BY(reg->nr_succ_test, module->nr_succ_test);
        RCU_INCR_BY(reg->nr_failed_test, module->nr_failed_test);
    }
    if (reg->nr_succ_module > 0) {
        RCU_INCR(engine->nr_succ_reg);
    } else if (reg->nr_failed_module > 0) {
        RCU_INCR(engine->nr_failed_reg);
    }
    RCU_INCR_BY(engine->nr_succ_test, reg->nr_succ_test);
    RCU_INCR_BY(engine->nr_failed_test, reg->nr_failed_test);
}

void rcu_gen_test_run_report(struct rcu_test_engine *engine) {
    rcu_gen_plaintext_report(engine);
}


