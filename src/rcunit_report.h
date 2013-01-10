/*
 * RCUNIT - A unit testing framework for C.
 * Copyright (C) 2006 Jerrico L. Gamis
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 *
 */

#ifndef RCUNIT_REPORT_H
#define RCUNIT_REPORT_H

/** Run report file names */
#define RCU_TEST_RUN_REPORT_FILENAME_HTML      "rcunit_test_run_report.html"
#define RCU_TEST_RUN_REPORT_FILENAME_PLAINTEXT     "rcunit_test_run_report.txt"

/** @brief Temporary report buffer size */
#define RCU_REPORT_BUFF_SIZE 1024

/** @brief Common report macro (no parameter) */
#define RCU_REPORT(format)                \
    memset(g_report_buff,0x00,RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff,format);             \
    rcu_report_iface_impl(g_report_buff);

/** @brief Common report macro (1 parameter) */
#define RCU_REPORT_P1(format,p1)          \
    memset(g_report_buff,0x00,RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff,format,p1);          \
    rcu_report_iface_impl(g_report_buff);

/** @brief Common report macro (2 parameters) */
#define RCU_REPORT_P2(format,p1,p2)       \
    memset(g_report_buff,0x00,RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff,format,p1,p2);       \
    rcu_report_iface_impl(g_report_buff);

/** @brief Common report macro (3 parameters) */
#define RCU_REPORT_P3(format,p1,p2,p3)    \
    memset(g_report_buff,0x00,RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff,format,p1,p2,p3);    \
    rcu_report_iface_impl(g_report_buff);

/** @brief Common report macro (4 parameters) */
#define RCU_REPORT_P4(format,p1,p2,p3,p4) \
    memset(g_report_buff,0x00,RCU_REPORT_BUFF_SIZE);  \
    sprintf(g_report_buff,format,p1,p2,p3,p4); \
    rcu_report_iface_impl(g_report_buff);

RCU_INT rcu_gen_test_run_report(RCU_TEST_MACHINE *mach);

#endif /* RCUNIT_REPORT_H */



