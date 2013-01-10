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

#include <rcunit.h>

/** @brief The RCUNIT logger */
RCU_LOGGER the_rcu_logger;

/** @brief RCUNIT logger init flag */
RCU_INT g_logger_init_done=RCU_FALSE;

/** @brief Log buffer */
RCU_CHAR g_log_buff[RCU_LOG_BUFF_SIZE];

/** @brief Redirection log buffer */
RCU_CHAR g_redir_buff[RCU_LOG_BUFF_SIZE];

/** @brief Stringified current log level */
const RCU_CHAR* g_log_level_str[]={
     "ALL"
    ,"INFO"
    ,"WARNING"
    ,"FATAL"
    ,"OFF"
};

#if RCU_ENABLE_FILE_LOG_HANDLER
extern RCU_LOG_HANDLER g_rcu_file_log_hnd;
extern RCU_VOID rcu_file_log_hnd_init(RCU_VOID *param);
extern RCU_VOID rcu_file_log_hnd_destroy(RCU_VOID *param);
extern RCU_VOID rcu_file_log_hnd_log_impl(RCU_VOID *param);
#endif

#if RCU_ENABLE_STDOUT_LOG_HANDLER
extern RCU_LOG_HANDLER g_rcu_stdout_log_hnd;
extern RCU_VOID rcu_stdout_log_hnd_init(RCU_VOID *param);
extern RCU_VOID rcu_stdout_log_hnd_destroy(RCU_VOID *param);
extern RCU_VOID rcu_stdout_log_hnd_log_impl(RCU_VOID *param);
#endif

/**
 *  @brief Sets the current log level
 *  @param[in] level Log level
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_set_log_level(RCU_INT level){
    if (level >= RCU_LOG_LEVEL_ALL && level <= RCU_LOG_LEVEL_OFF){
        the_rcu_logger.log_level = level;
        return(RCU_E_OK);
    }
    return(RCU_E_NG);
}

/**
 *  @brief Retrieves the current log level
 *  @return Current log level
 */

RCU_INT rcu_get_log_level(){
    return(the_rcu_logger.log_level);
}

/**
 *  @brief Retrieves the stringified log level
 *  @param[in] level Log level
 *  @return Stringified current log level
 */

const RCU_CHAR *rcu_get_log_level_str(RCU_INT level){
const RCU_CHAR *level_str;
    if (level >= RCU_LOG_LEVEL_ALL && level <= RCU_LOG_LEVEL_OFF){
        level_str = g_log_level_str[level];
    }else{
        level_str = RCU_NULL;
    }
    return(level_str);
}

/**
 *  @brief Main log implementation
 *  @param[in] level Log level
 *  @param[in] data Log data
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_log_impl(RCU_INT level,RCU_CHAR *data){
RCU_GENERIC_LIST *cursor;
RCU_LOG_HANDLER *log_hnd;
RCU_LOG_RECORD log_rec;
const RCU_CHAR *level_str;
    if (!g_logger_init_done){
        return(RCU_E_NG);
    }
    if (rcu_get_log_level() == RCU_LOG_LEVEL_OFF){
        return(RCU_E_OK);
    }
    if (data == RCU_NULL){
        return(RCU_E_NG);
    }
    level_str = rcu_get_log_level_str(level);
    RCU_FOR_EACH_ENTRY(&the_rcu_logger.log_hnd_list,cursor){
        log_hnd = (RCU_LOG_HANDLER*)cursor;
        if (level >= rcu_get_log_level()){
            log_rec.level = level;
            log_rec.data = data;
            log_hnd->log_impl(&log_rec);
        }
    }
#if RCU_ENABLE_REDIRECT_LOG
    memset(g_redir_buff,0x00,RCU_LOG_BUFF_SIZE);
    sprintf(g_redir_buff,"[RCUNIT] %s : %s\n",level_str,data);
    rcu_print(g_redir_buff);
#endif
    return(RCU_E_OK);
}

/** @brief Log handler banner */

#define RCU_LOG_HANDLER_BANNER(ts_buff,log_hnd,banner)         \
    {                                                          \
        RCU_LOG_RECORD log_rec;                                \
        memset(g_log_buff,0x00,RCU_LOG_BUFF_SIZE);             \
        sprintf(g_log_buff,(banner),(ts_buff));                \
        log_rec.level = RCU_LOG_LEVEL_INFO;                    \
        log_rec.data = g_log_buff;                             \
        (log_hnd)->log_impl(&log_rec);                         \
    }

/** @brief Log handler entry banner */
#define RCU_LOG_HANDLER_START_BANNER(ts_buff,log_hnd) \
    RCU_LOG_HANDLER_BANNER((ts_buff),(log_hnd),"Log handler initialized %s")

/** @brief Log handler exit banner */
#define RCU_LOG_HANDLER_STOP_BANNER(log_hnd) \
    RCU_LOG_HANDLER_BANNER((ts_buff),(log_hnd),"Log handler destroyed %s")

/** @brief Log handler exit banner
 *  @param[in] log_hnd Log handler
 */

RCU_INT rcu_init_log_hnd(RCU_LOG_HANDLER *log_hnd){
RCU_CHAR ts_buff[RCU_TSTAMP_BUFF_SIZE];
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
    if (log_hnd->init != RCU_NULL){
        log_hnd->init(RCU_NULL);
        RCU_LOG_HANDLER_START_BANNER(ts_buff,log_hnd);
    }
    return(RCU_E_OK);
}

/**
 *  @brief Stops a log handler
 *  @param[in] log_hnd Log handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_log_hnd(RCU_LOG_HANDLER *log_hnd){
RCU_CHAR ts_buff[RCU_TSTAMP_BUFF_SIZE];
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
    if (log_hnd->destroy!=RCU_NULL){
        RCU_LOG_HANDLER_STOP_BANNER(log_hnd);
        log_hnd->destroy(RCU_NULL);
    }
    return(RCU_E_OK);
}

/**
 *  @brief Returns true if the given log handler is registered
 *  @param[in] log_hnd Log handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */
RCU_INT rcu_is_log_hnd_found(RCU_LOG_HANDLER *log_hnd){
RCU_GENERIC_LIST *cursor;
RCU_LOG_HANDLER *srch_log_hnd;
    if (log_hnd == RCU_NULL){
        return(RCU_FALSE);
    }
    RCU_FOR_EACH_ENTRY(&the_rcu_logger.log_hnd_list,cursor){
        srch_log_hnd = (RCU_LOG_HANDLER*)cursor;
        if (srch_log_hnd == log_hnd){
            return(RCU_TRUE);
        }
    }
    return(RCU_FALSE);
}

/**
 *  @brief Registers a log handler
 *  @param[in] log_hnd Log handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_add_log_hnd(RCU_LOG_HANDLER *log_hnd){
    if (log_hnd == RCU_NULL){
        return(RCU_E_NG);
    }
    if (rcu_is_log_hnd_found(log_hnd)){
        return(RCU_E_NG);
    }
    if (log_hnd->init == RCU_NULL || log_hnd->destroy == RCU_NULL ||
        log_hnd->log_impl == RCU_NULL){
        return(RCU_E_NG);
    }
    rcu_init_list(&log_hnd->link);
    rcu_insert_list(&the_rcu_logger.log_hnd_list,&log_hnd->link);
    rcu_init_log_hnd(log_hnd);
    return(RCU_E_OK);
}

/**
 *  @brief Deletes a log handler
 *  @param[in] log_hnd Log handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */
RCU_INT rcu_del_log_hnd(RCU_LOG_HANDLER *log_hnd){
    if (log_hnd == RCU_NULL){
        return(RCU_E_NG);
    }
    if (!rcu_is_log_hnd_found(log_hnd)){
        return(RCU_E_NG);
    }
    rcu_remove_list(&log_hnd->link);
    return(RCU_E_OK);
}

/**
 *  @brief Destroys the logging facility
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_log(){
RCU_GENERIC_LIST *cursor;
RCU_LOG_HANDLER *log_hnd;
    if (!g_logger_init_done){
        return(RCU_E_NG);
    }
    RCU_FOR_EACH_ENTRY(&the_rcu_logger.log_hnd_list,cursor){
        log_hnd = (RCU_LOG_HANDLER*)cursor;
        rcu_destroy_log_hnd(log_hnd);
    }
    return(RCU_E_OK);
}

/**
 *  @brief Initializes the logging facility
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_init_log(){
    if (g_logger_init_done){
        rcu_destroy_log();
    }
    g_logger_init_done = RCU_TRUE;
    memset(&the_rcu_logger,0x00,sizeof(RCU_LOGGER));
    rcu_init_list(&the_rcu_logger.log_hnd_list);
    the_rcu_logger.log_level = RCU_LOG_LEVEL_ALL;
#if RCU_ENABLE_FILE_LOG_HANDLER
    g_rcu_file_log_hnd.init = rcu_file_log_hnd_init;
    g_rcu_file_log_hnd.destroy = rcu_file_log_hnd_destroy;
    g_rcu_file_log_hnd.log_impl = rcu_file_log_hnd_log_impl;
    rcu_add_log_hnd(&g_rcu_file_log_hnd);
#endif
#if RCU_ENABLE_STDOUT_LOG_HANDLER
    g_rcu_stdout_log_hnd.init = rcu_stdout_log_hnd_init;
    g_rcu_stdout_log_hnd.destroy = rcu_stdout_log_hnd_destroy;
    g_rcu_stdout_log_hnd.log_impl = rcu_stdout_log_hnd_log_impl;
    rcu_add_log_hnd(&g_rcu_stdout_log_hnd);
#endif
    return(RCU_E_OK);
}
