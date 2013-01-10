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

#ifndef RCUNIT_LOG_H
#define RCUNIT_LOG_H

/** @brief HTML log file name */
#define RCU_LOG_FILENAME_HTML  "rcunit_log.html"

/** @brief Plain text log file name */
#define RCU_LOG_FILENAME_PLAINTEXT  "rcunit_log.txt"

/** @brief Log record */
typedef struct rcu_log_record {
RCU_INT level;                      /**< Log level */
RCU_CHAR *data;                     /**< Log data */
}RCU_LOG_RECORD;

/** @brief Log handler data structure */
typedef struct rcu_log_handler{
RCU_GENERIC_LIST link;              /**< Link to other handlers (Used by the framework) */
RCU_GENERIC_FUNCTION init;          /**< Initialization function */
RCU_GENERIC_FUNCTION destroy;       /**< Destruction function */
RCU_GENERIC_FUNCTION log_impl;      /**< Log implementation */
}RCU_LOG_HANDLER;

/** @brief Logger data structure */
typedef struct rcu_logger {
RCU_INT log_level;                  /**< Current log level */
RCU_GENERIC_LIST log_hnd_list;      /**< List of log handlers */
}RCU_LOGGER;

/** @brief Enables all log message types */
#define RCU_LOG_LEVEL_ALL    0
/** @brief Enables info, warning, and fatal logs (Lowest level) */
#define RCU_LOG_LEVEL_INFO   1
/** @brief Enables only warning and fatal logs (Middle level) */
#define RCU_LOG_LEVEL_WARN   2
/** @brief Enables only fatal logs (Highest level)*/
#define RCU_LOG_LEVEL_FATAL  3
/** @brief Turns off logging */
#define RCU_LOG_LEVEL_OFF    4

/** @brief Error message buffer size */
#define RCU_LOG_BUFF_SIZE   2048

/** @brief Common log macro (no parameter) */
#define RCU_LOG(type,format)                \
    memset(g_log_buff,0x00,RCU_LOG_BUFF_SIZE);  \
    sprintf(g_log_buff,format);             \
    rcu_log_impl(type,g_log_buff);

/** @brief Common log macro (1 parameter) */
#define RCU_LOG_P1(type,format,p1)          \
    memset(g_log_buff,0x00,RCU_LOG_BUFF_SIZE);  \
    sprintf(g_log_buff,format,p1);          \
    rcu_log_impl(type,g_log_buff);

/** @brief Common log macro (2 parameters) */
#define RCU_LOG_P2(type,format,p1,p2)       \
    memset(g_log_buff,0x00,RCU_LOG_BUFF_SIZE);  \
    sprintf(g_log_buff,format,p1,p2);       \
    rcu_log_impl(type,g_log_buff);

/** @brief Common log macro (3 parameters) */
#define RCU_LOG_P3(type,format,p1,p2,p3)    \
    memset(g_log_buff,0x00,RCU_LOG_BUFF_SIZE);  \
    sprintf(g_log_buff,format,p1,p2,p3);    \
    rcu_log_impl(type,g_log_buff);

/** @brief Common log macro (4 parameters) */
#define RCU_LOG_P4(type,format,p1,p2,p3,p4) \
    memset(g_log_buff,0x00,RCU_LOG_BUFF_SIZE);  \
    sprintf(g_log_buff,format,p1,p2,p3,p4); \
    rcu_log_impl(type,g_log_buff);

/** @brief Info log macro (no parameter) */
#define RCU_LOG_INFO(format)                \
    RCU_LOG(RCU_LOG_LEVEL_INFO,format);

/** @brief Info log macro (1 parameter) */
#define RCU_LOG_INFO_P1(format,p1)          \
    RCU_LOG_P1(RCU_LOG_LEVEL_INFO,format,p1);

/** @brief Info log macro (2 parameters) */
#define RCU_LOG_INFO_P2(format,p1,p2)    \
    RCU_LOG_P2(RCU_LOG_LEVEL_INFO,format,p1,p2);

/** @brief Info log macro (3 parameters) */
#define RCU_LOG_INFO_P3(format,p1,p2,p3) \
    RCU_LOG_P3(RCU_LOG_LEVEL_INFO,format,p1,p2,p3);

/** @brief Info log macro (4 parameters) */
#define RCU_LOG_INFO_P4(format,p1,p2,p3,p4)  \
    RCU_LOG_P4(RCU_LOG_LEVEL_INFO,format,p1,p2,p3,p4);

/** @brief Warning log macro (no parameter) */
#define RCU_LOG_WARN(format)                \
    RCU_LOG(RCU_LOG_LEVEL_WARN,format);

/** @brief Warning log macro (1 parameter) */
#define RCU_LOG_WARN_P1(format,p1)          \
    RCU_LOG_P1(RCU_LOG_LEVEL_WARN,format,p1);

/** @brief Warning log macro (2 parameters) */
#define RCU_LOG_WARN_P2(format,p1,p2) \
    RCU_LOG_P2(RCU_LOG_LEVEL_WARN,format,p1,p2);

/** @brief Warning log macro (3 parameters) */
#define RCU_LOG_WARN_P3(format,p1,p2,p3) \
    RCU_LOG_P3(RCU_LOG_LEVEL_WARN,format,p1,p2,p3);

/** @brief Warning log macro (4 parameters) */
#define RCU_LOG_WARN_P4(format,p1,p2,p3,p4) \
    RCU_LOG_P4(RCU_LOG_LEVEL_WARN,format,p1,p2,p3,p4);

/** @brief Fatal log macro (no parameter) */
#define RCU_LOG_FATAL(format)                \
    RCU_LOG(RCU_LOG_LEVEL_FATAL,format);

/** @brief Fatal log macro (1 parameter) */
#define RCU_LOG_FATAL_P1(format,p1)          \
    RCU_LOG_P1(RCU_LOG_LEVEL_FATAL,format,p1);

/** @brief Fatal log macro (2 parameters) */
#define RCU_LOG_FATAL_P2(format,p1,p2) \
    RCU_LOG_P2(RCU_LOG_LEVEL_FATAL,format,p1,p2);

/** @brief Fatal log macro (3 parameters) */
#define RCU_LOG_FATAL_P3(format,p1,p2,p3) \
    RCU_LOG_P3(RCU_LOG_LEVEL_FATAL,format,p1,p2,p3);

/** @brief Fatal log macro (4 parameters) */
#define RCU_LOG_FATAL_P4(format,p1,p2,p3,p4) \
    RCU_LOG_P4(RCU_LOG_LEVEL_FATAL,format,p1,p2,p3,p4);

/** @brief Retrieves the current log level */
#define RCU_GET_LOG_LEVEL() \
    (g_log_level)

/** Log management function prototypes */
RCU_INT rcu_log_impl(RCU_INT level,RCU_CHAR *data);
RCU_INT rcu_init_log();
RCU_INT rcu_destroy_log();

RCU_INT rcu_set_log_level(RCU_INT level);
RCU_INT rcu_get_log_level();

RCU_INT rcu_add_log_hnd(RCU_LOG_HANDLER *log_hnd);
RCU_INT rcu_del_log_hnd(RCU_LOG_HANDLER *log_hnd);

extern RCU_LOG_HANDLER g_rcu_file_log_hnd;

const RCU_CHAR *rcu_get_log_level_str(RCU_INT level);

#endif /* RCUNIT_LOG_H */



