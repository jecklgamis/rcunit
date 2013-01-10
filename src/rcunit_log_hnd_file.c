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

#if RCU_ENABLE_FILE_LOG_HANDLER

/** @brief File log handler */
RCU_LOG_HANDLER g_rcu_file_log_hnd;

/** @brief Log file pointer */
FILE *log_fp=RCU_NULL;

/**
 *  @brief Log handler implementation
 *  @param[in] param  Log data (RCU_LOG_REC)
 */

RCU_DEF_GENERIC_FUNC(rcu_file_log_hnd_log_impl,param){
const RCU_CHAR *level_str;
RCU_LOG_RECORD *log_rec;
    if (log_fp == RCU_NULL){
        return;
    }
    log_rec = (RCU_LOG_RECORD*) param;
    level_str = rcu_get_log_level_str(log_rec->level);
#if RCU_ENABLE_EMIT_PLAINTEXT_LOG
    fprintf(log_fp,"[RCUNIT] %s : %s\n",level_str,log_rec->data);
#else
    switch(log_rec->level){
        case RCU_LOG_LEVEL_INFO:
            fprintf(log_fp,"[RCUNIT] %s : %s<br>\n",level_str,log_rec->data);
            break;
        case RCU_LOG_LEVEL_WARN:
            fprintf(log_fp,"<font style=\"color:red\">[RCUNIT] %s: %s</font><br>\n",level_str,log_rec->data);
            break;
        case RCU_LOG_LEVEL_FATAL:
            fprintf(log_fp,"<font style=\"color:blue\">[RCUNIT] %s: %s</font><br>\n",level_str,log_rec->data);
            break;
    }
#endif
    fflush(log_fp);
}

/**
 *  @brief Destroys the log handler
 *  @param[in] param  Set to RCU_NULL value
 */

RCU_DEF_GENERIC_FUNC(rcu_file_log_hnd_destroy,param){
char ts_buff[RCU_TSTAMP_BUFF_SIZE];
    if (log_fp != RCU_NULL){
        rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
#if RCU_ENABLE_EMIT_PLAINTEXT_LOG
    fprintf(log_fp,"Generated on %s by RCUNIT version %s\n",ts_buff,RCU_VERSION_STRING);
#else
        fprintf(log_fp,"<hr>\n");
        fprintf(log_fp,"<i><font size=\"-1\" align=\"left\">Generated on %s by <a href=\"http://rcunit.sourceforge.net\"> RCUNIT</a> version %s</i>\n",ts_buff,RCU_VERSION_STRING);
#endif
        fflush(log_fp);
        fclose(log_fp);
        log_fp = RCU_NULL;
    }
}

/**
 *  @brief Initializes the log handler
 *  @param[in] param  Set to RCU_NULL value
 */

RCU_DEF_GENERIC_FUNC(rcu_file_log_hnd_init,param){
    if (log_fp != RCU_NULL){
        fflush(log_fp);
        fclose(log_fp);
        log_fp = RCU_NULL;
    }
#if RCU_ENABLE_DEBUG_FILE_LOG_HANDLER
    {
        static s_counter=0;
        RCU_CHAR ts_filename[64];
#if RCU_ENABLE_EMIT_PLAINTEXT_LOG
        sprintf(ts_filename,"debug_%02lu_%s",s_counter,RCU_LOG_FILENAME_PLAINTEXT);
#else
        sprintf(ts_filename,"debug_%02lu_%s",s_counter,RCU_LOG_FILENAME_HTML);
#endif
        s_counter++;
        if ((log_fp=fopen(ts_filename,"wt"))==RCU_NULL){
            return;
        }
    }
#else /** RCU_ENABLE_DEBUG_FILE_LOG_HANDLER */

#if RCU_ENABLE_EMIT_PLAINTEXT_LOG
    if ((log_fp=fopen(RCU_LOG_FILENAME_PLAINTEXT,"wt"))==RCU_NULL){
        return;
    }
#else
    if ((log_fp=fopen(RCU_LOG_FILENAME_HTML,"wt"))==RCU_NULL){
        return;
    }
#endif

#endif  /** RCU_ENABLE_DEBUG_FILE_LOG_HANDLER */

#if RCU_ENABLE_EMIT_PLAINTEXT_LOG
    fprintf(log_fp,"                               RCUNIT Log\n\n");
#else
    fprintf(log_fp,"<!doctype html public \"-//w3c//dtd html 3.2//en\">\n");
    fprintf(log_fp,"<html>");
    fprintf(log_fp,"<head><title>RCUNIT Log</title><head>");
    fprintf(log_fp,"<style><!-- h1, b { font-family: arial, helvetica, sans-serif }--></style>\n");
    fprintf(log_fp,"<body>\n");
    fprintf(log_fp,"<table width=\"100%% align=\"center\">\n");
    fprintf(log_fp,"<tr style=\"background-color:#7b68ee\"><td><br><center><font size=\"+2\"><b>RCUNIT Log</b></font></center></td></tr>\n");
    fprintf(log_fp,"</table>\n");
    fprintf(log_fp,"<hr>\n");
#endif
}

#endif /** RCU_ENABLE_FILE_LOG_HANDLER */

