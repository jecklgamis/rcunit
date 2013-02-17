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

