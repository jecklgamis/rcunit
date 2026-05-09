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

#include "testmoko.h"

unsigned int g_nr_failed_test;
unsigned int g_nr_succ_test;
static int g_tmk_initialized = 0;

#define TMK_LOG_BUFF_SIZE 2048
static char local_log_buff[TMK_LOG_BUFF_SIZE];
tmk_test_function *tmk_test_function_table;
char assert_msg_buff[TMK_LOG_BUFF_SIZE];

void tmk_log_impl(const char *filename, const int line_no, const char *format, ...) {
    va_list ap;
    memset(local_log_buff, 0, TMK_LOG_BUFF_SIZE);
    va_start(ap, format);
    vsprintf(local_log_buff, format, ap);
    va_end(ap);
    fprintf(stdout, "[TEST] %s\n", local_log_buff);
}

void tmk_assert_impl(int cond, const char *filename, const char *func_name,
                     int line, const char *format, ...) {
    va_list ap;
    memset(&assert_msg_buff[0], 0, TMK_LOG_BUFF_SIZE);
    va_start(ap, format);
    vsprintf(assert_msg_buff, format, ap);
    va_end(ap);
    if (!cond) {
        TMK_LOG("Assertion failure in %s(%s:%d) : %s ", func_name,
                filename, line, assert_msg_buff);
        TMK_THROW(TMK_GET_EXCP(TMK_EXCP_ASSERTIONFAILURE));
    }
}

TMK_API int tmk_run_tests(tmk_test_function_entry *tbl, TMK_NULLABLE void (*setup)(), TMK_NULLABLE void (*teardown)()) {
    tmk_test_function_entry *test;
    g_nr_failed_test = 0;
    TMK_LOG("Test started");
    int abort_on_first_failure = 1;

    for (test = tbl; test->name != NULL; test++) {
        test->result = TEST_RESULT_SUCCESSFUL;
        TMK_TRY
                {
                    if (setup != NULL) {
                        setup();
                    }
                    if (test->setup != NULL) {
                        test->setup();
                    }
                    if (test->test != NULL) {

                        test->test();
                    }
                    if (test->teardown != NULL) {
                        test->teardown();
                    }
                    if (teardown != NULL) {
                        teardown();
                    }
                    g_nr_succ_test++;
                    TMK_LOG("Test : %s OK", test->name);
                }

            TMK_CATCH(e)
                {
                    TMK_LOG("Test : %s NG (%s)", test->name, e->name);
                    if (abort_on_first_failure) {
                        return EXIT_FAILURE;
                    }
                    g_nr_failed_test++;
                    continue;
                    test->result = TEST_RESULT_FAILED;
                }
        TMK_END_CATCH
    }

    if (teardown != NULL) {
        TMK_TRY
                {
                    teardown();
                }

            TMK_CATCH(e)
                {
                    TMK_LOG("Exception encountered in teardown : %s", e->name);
                }
        TMK_END_CATCH
    }

    TMK_LOG("Test Results : Passed = %lu, Failed = %lu ", g_nr_succ_test, g_nr_failed_test);
    if (g_nr_failed_test != 0) {
        TMK_LOG("Test failed!");
    } else {
        TMK_LOG("Test successful");
    }
    TMK_LOG("Test finished");
    return g_nr_failed_test != 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}

void tmk_init() {
    if (!g_tmk_initialized) {
        tmk_init_exception();
        g_tmk_initialized = 1;
    }
}

void tmk_destroy() {
    if (g_tmk_initialized) {
        tmk_destroy_exception();
    }
}