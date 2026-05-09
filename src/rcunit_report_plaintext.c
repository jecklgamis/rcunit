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

#define RCU_PLAINTEXT_LINE_WIDTH 80
#define RCU_LINE_MARKER_1 "========================================"

int rcu_print_char(FILE *fp, char ch, int nr_chars);

const char *rcu_get_stat_str(int stat);

#define RCU_DEFAULT_STRING_BUFFER_SIZE 1024

int rcu_indent(FILE *fp, int nr_spaces);

int rcu_gen_plaintext_report(struct rcu_test_engine *engine) {
    char ts_buff[RCU_TSTAMP_BUFF_SIZE];
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    FILE *fp = NULL;
    const char *title = "rcunit Test Run Report";
    const char *sub_title = "https://github.com/jecklgamis/rcunit";

    RCU_LOG_DEBUG("Generating test run report : %s", RCU_TEST_RUN_REPORT_FILENAME_PLAINTEXT);
    if ((!(fp = fopen(RCU_TEST_RUN_REPORT_FILENAME_PLAINTEXT, "wt")))) {
        return RCU_E_NG;
    }
    fprintf(fp, "%s%s\n", RCU_LINE_MARKER_1, RCU_LINE_MARKER_1);
    rcu_print_char(fp, ' ', (RCU_PLAINTEXT_LINE_WIDTH - strlen(title)) / 2);
    fprintf(fp, "%s\n", title);
    rcu_print_char(fp, ' ', (RCU_PLAINTEXT_LINE_WIDTH - strlen(sub_title)) / 2);
    fprintf(fp, "%s\n", sub_title);
    fprintf(fp, "%s%s\n", RCU_LINE_MARKER_1, RCU_LINE_MARKER_1);

    struct rcu_registry *reg = &engine->def_reg;
    fprintf(fp, "[registry = \"%s\", %d modules]\n", reg->name, reg->nr_module);

    RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&reg->mod_list, mod_cursor, mod_no) {
        struct rcu_module *module = (struct rcu_module *) mod_cursor;
        rcu_indent(fp, 4);
        fprintf(fp, "%d. [module = \"%s\", %d tests]\n", mod_no, module->name, module->nr_test);

        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&module->fail_rec_list, fail_rec_cursor) {
            struct rcu_failure_record *fail_rec = (struct rcu_failure_record *) fail_rec_cursor;
            rcu_indent(fp, 12);
            fprintf(fp, "%s\n", fail_rec->info);
        }

        RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&module->func_list, func_cursor, func_no) {
            struct rcu_test *func = (struct rcu_test *) func_cursor;
            rcu_indent(fp, 8);
            fprintf(fp, "[test = \"%s\", result = %s]\n",
                    func->name,
                    rcu_get_stat_str(func->run_stat));
            if (func->nr_succ_assert == 0 && func->nr_fail_assert == 0
                && func->run_stat == RCU_RUN_STAT_TEST_SUCC) {
                rcu_indent(fp, 12);
                fprintf(fp, "[failure = No assertions executed!]\n");
            }
            struct rcu_list *fail_rec_cursor;

            RCU_FOR_EACH_ENTRY(&func->fail_rec_list, fail_rec_cursor) {
                struct rcu_failure_record *fail_rec = (struct rcu_failure_record *) fail_rec_cursor;
                rcu_indent(fp, 12);
                fprintf(fp, "[failure = %s]\n", fail_rec->info);
            }
        }
    }

    fprintf(fp, "\nTest Run Results: Passed : %d  Failed : %d\n", engine->nr_succ_test, engine->nr_failed_test);
    if (engine->nr_failed_test == 0) {
        fprintf(fp, "Test successful!\n");
    } else {
        fprintf(fp, "Test failed!\n");
    }


    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    fprintf(fp, "\n%s%s\n", RCU_LINE_MARKER_1, RCU_LINE_MARKER_1);
    fprintf(fp, "Generated last %s by rcunit version %s.\n\n", ts_buff, RCU_VERSION_STRING);

    if (fp) {
        fclose(fp);
    }
    RCU_LOG_INFO("Test run report generated : %s", RCU_TEST_RUN_REPORT_FILENAME_PLAINTEXT);
    return RCU_E_OK;
}

int rcu_print_char(FILE *fp, char ch, int nr_chars) {
    int a = 0;
    for (a = 0; a < nr_chars; a++) {
        fprintf(fp, "%c", ch);
    }
    return RCU_E_OK;
}

int rcu_indent(FILE *fp, int nr_spaces) {
    int a = 0;
    for (a = 0; a < nr_spaces; a++) {
        fprintf(fp, " ");
    }
    return RCU_E_OK;
}
