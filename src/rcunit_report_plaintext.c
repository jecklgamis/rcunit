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

#define RCU_PLAINTEXT_LINE_WIDTH 80
#define RCU_LINE_MARKER_1 "========================================"

int rcu_print_char(FILE *fp, char ch, int nr_chars);

const char *rcu_get_stat_str(int stat);

#define RCU_DEFAULT_STRING_BUFFER_SIZE 1024

int rcu_indent(FILE *fp, int nr_spaces);

int rcu_gen_plaintext_report(rcu_test_machine *machine) {
    char ts_buff[RCU_TSTAMP_BUFF_SIZE];
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    FILE *fp = NULL;
    const char *title = "RCUNIT Test Run Report";
    const char *sub_title = "http://github.com/jecklgamis/rcunit";

    RCU_LOG_DEBUG("Generating test run report : %s", RCU_TEST_RUN_REPORT_FILENAME_PLAINTEXT);
    if ((fp = fopen(RCU_TEST_RUN_REPORT_FILENAME_PLAINTEXT, "wt")) == NULL) {
        return RCU_E_NG;
    }
    fprintf(fp, "%s%s\n", RCU_LINE_MARKER_1, RCU_LINE_MARKER_1);
    rcu_print_char(fp, ' ', (RCU_PLAINTEXT_LINE_WIDTH - strlen(title)) / 2);
    fprintf(fp, "%s\n", title);
    rcu_print_char(fp, ' ', (RCU_PLAINTEXT_LINE_WIDTH - strlen(sub_title)) / 2);
    fprintf(fp, "%s\n", sub_title);
    fprintf(fp, "%s%s\n", RCU_LINE_MARKER_1, RCU_LINE_MARKER_1);

    RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&machine->reg_list, reg_cursor, reg_no) {
        rcu_registry *reg = (rcu_registry *) reg_cursor;
        fprintf(fp, "%d. [registry = \"%s\", %d modules]\n", reg_no, reg->name, reg->nr_mod);

        RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&reg->mod_list, mod_cursor, mod_no) {
            rcu_module *mod = (rcu_module *) mod_cursor;
            rcu_indent(fp, 4);
            fprintf(fp, "%d.%d [module = \"%s\", %d tests]\n", reg_no, mod_no, mod->name, mod->nr_test);

            RCU_FOR_EACH_ENTRY_WITH_CURSOR(&mod->fail_rec_list, fail_rec_cursor) {
                rcu_failure_record *fail_rec = (rcu_failure_record *) fail_rec_cursor;
                rcu_indent(fp, 12);
                fprintf(fp, "%s\n", fail_rec->info);
            }

            RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&mod->func_list, func_cursor, func_no) {
                rcu_test *func = (rcu_test *) func_cursor;
                rcu_indent(fp, 8);
                fprintf(fp, "[test = \"%s\", result = %s]\n",
                        func->name,
                        rcu_get_stat_str(func->run_stat));
                if (func->nr_succ_assert == 0 && func->nr_fail_assert == 0
                    && RCU_IS_TEST_SUCCEDED(func)) {
                    rcu_indent(fp, 12);
                    fprintf(fp, "[failure = No assertions executed!]\n");
                }
                rcu_list *fail_rec_cursor;

                RCU_FOR_EACH_ENTRY(&func->fail_rec_list, fail_rec_cursor) {
                    rcu_failure_record *fail_rec = (rcu_failure_record *) fail_rec_cursor;
                    rcu_indent(fp, 12);
                    fprintf(fp, "[failure = %s]\n", fail_rec->info);
                }
            }
        }
    }

    fprintf(fp, "\nTest Run Results: Passed : %d  Failed : %d\n", machine->nr_succ_test, machine->nr_failed_test);
    if (machine->nr_failed_test == 0) {
        fprintf(fp, "Test successful!\n");
    } else {
        fprintf(fp, "Test failed!\n");
    }


    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    fprintf(fp, "\n%s%s\n", RCU_LINE_MARKER_1, RCU_LINE_MARKER_1);
    fprintf(fp, "Generated last %s by RCUNIT version %s.\n\n", ts_buff, RCU_VERSION_STRING);

    if (fp != NULL) {
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
