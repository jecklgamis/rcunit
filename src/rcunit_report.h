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

#ifndef RCUNIT_REPORT_H
#define RCUNIT_REPORT_H

/* Run report file names */
#define RCU_TEST_RUN_REPORT_FILENAME_HTML      "rcunit_test_run_report.html"
#define RCU_TEST_RUN_REPORT_FILENAME_PLAINTEXT "rcunit_test_run_report.txt"
#define RCU_TEST_RUN_REPORT_FILENAME_XML       "rcunit_test_run_report.xml"
#define RCU_TEST_RUN_REPORT_FILENAME_JSON      "rcunit_test_run_report.json"

#if 0
/* Temporary report buffer size */
#define RCU_REPORT_BUFF_SIZE 1024

/* Common report macro (no parameter) */
#define RCU_REPORT(format)                \
    memset(g_report_buff, 0x00, RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff, format);             \
    rcu_report_iface_impl(g_report_buff);

/* Common report macro (1 parameter) */
#define RCU_REPORT_P1(format, p1)          \
    memset(g_report_buff, 0x00, RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff, format, p1);          \
    rcu_report_iface_impl(g_report_buff);

/* Common report macro (2 parameters) */
#define RCU_REPORT_P2(format, p1, p2)       \
    memset(g_report_buff,0x00,RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff, format, p1, p2);       \
    rcu_report_iface_impl(g_report_buff);

/* Common report macro (3 parameters) */
#define RCU_REPORT_P3(format, p1, p2, p3)    \
    memset(g_report_buff,0x00,RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff, format, p1, p2, p3);    \
    rcu_report_iface_impl(g_report_buff);

/* Common report macro (4 parameters) */
#define RCU_REPORT_P4(format, p1, p2, p3, p4) \
    memset(g_report_buff, 0x00, RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff, format, p1, p2, p3, p4); \
    rcu_report_iface_impl(g_report_buff);

void rcu_gen_test_run_report(rcu_test_machine *machine);

const char *rcu_get_stat_str(int stat);

#endif

#endif /* RCUNIT_REPORT_H */

