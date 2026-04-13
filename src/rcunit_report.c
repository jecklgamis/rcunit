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

extern int rcu_gen_html_report(rcu_test_machine *machine);

extern int rcu_gen_plaintext_report(rcu_test_machine *machine);

extern int rcu_gen_xml_report(rcu_test_machine *machine);

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
void rcu_prepare_report(rcu_test_machine *machine) {

    RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&machine->reg_list, reg_cursor, reg_no) {
        rcu_registry *reg = (rcu_registry *) reg_cursor;

        RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&reg->mod_list, mod_cursor, mod_no) {
            rcu_module *mod = (rcu_module *) mod_cursor;

            RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&mod->func_list, func_cursor, func_no) {
                rcu_test *func = (rcu_test *) func_cursor;
                if (RCU_IS_TEST_SUCCEDED(func)) {
                    RCU_INCR(mod->nr_succ_test);
                } else if (RCU_IS_TEST_FAILED(func)) {
                    RCU_INCR(mod->nr_failed_test);
                } else {
                    RCU_LOG_WARN("Test with unknown test result : %s", func->name);
                }
            }
            if (RCU_IS_TEST_SUCCEDED(mod)) {
                RCU_INCR(reg->nr_succ_mod);
            }
            if (RCU_IS_TEST_FAILED(mod)) {
                RCU_INCR(reg->nr_failed_mod);
            } else {
                RCU_LOG_WARN("Module with unknown test result : %s", mod->name);
            }
            RCU_INCR_BY(reg->nr_succ_test, mod->nr_succ_test);
            RCU_INCR_BY(reg->nr_failed_test, mod->nr_failed_test);
        }
        if (reg->nr_succ_mod > 0) {
            RCU_INCR(machine->nr_succ_reg);
        } else if (reg->nr_failed_mod > 0) {
            RCU_INCR(machine->nr_failed_reg);
        } else {
            RCU_LOG_WARN("Registry with unknown test result : %s", reg->name);
        }
        RCU_INCR_BY(machine->nr_succ_test, reg->nr_succ_test);
        RCU_INCR_BY(machine->nr_failed_test, reg->nr_failed_test);
    }
}

void rcu_gen_test_run_report(rcu_test_machine *machine) {
    rcu_gen_plaintext_report(machine);
}


