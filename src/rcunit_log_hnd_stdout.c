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

#if RCU_ENABLE_STDOUT_LOG_HANDLER

RCU_LOG_HANDLER g_rcu_stdout_log_hnd;


/**
 *  @brief Log handler implementation
 *  @param[in] param  Log data (RCU_LOG_REC)
 */

RCU_DEF_GENERIC_FUNC(rcu_stdout_log_hnd_log_impl,param){
RCU_LOG_RECORD *log_rec;
const RCU_CHAR *level_str;
    log_rec = (RCU_LOG_RECORD*) param;
    level_str = rcu_get_log_level_str(log_rec->level);
    fprintf(stdout,"[RCUNIT] %s : %s\n",level_str,log_rec->data);
}

/**
 *  @brief Destroys the log handler
 *  @param[in] param  Set to RCU_NULL value
 */

RCU_DEF_GENERIC_FUNC(rcu_stdout_log_hnd_destroy,param){
}

/**
 *  @brief Initializes log handler
 *  @param[in] param  Set to RCU_NULL value
 */

RCU_DEF_GENERIC_FUNC(rcu_stdout_log_hnd_init,param){
}

#endif


