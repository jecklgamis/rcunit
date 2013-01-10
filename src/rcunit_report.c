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

/** @brief Temporary report buffer */
RCU_CHAR g_report_buff[RCU_REPORT_BUFF_SIZE];

/** @brief Line marker */
#define RCU_LINE_MARKER_1 "========================================================================"
/** @brief Line marker */
#define RCU_LINE_MARKER_2 "************************************************************************"
/** @brief Line marker */
#define RCU_LINE_MARKER_3 "------------------------------------------------------------------------"

#if RCU_ENABLE_REDIRECT_REPORT
#undef RCU_ENABLE_EMIT_PLAINTEXT_REPORT
#define RCU_ENABLE_EMIT_PLAINTEXT_REPORT 1
#endif


/** current test registry run statistics */
RCU_U4 reg_nr_fin_mod;              /**< no. of completed test modules */
RCU_U4 reg_nr_fail_mod;             /**< no. of failed test modules */
RCU_U4 reg_nr_succ_mod;             /**< no. of successful modules */
RCU_U4 reg_nr_abort_mod;            /**< no. of aborted modules */
RCU_U4 reg_nr_test;                 /**< no. of registered test functions */
RCU_U4 reg_nr_fin_test;             /**< no. of completed test functions */
RCU_U4 reg_nr_fail_test;            /**< no. of failed test functions */
RCU_U4 reg_nr_succ_test;            /**< no. of successful test functions */
RCU_U4 reg_nr_abort_test;           /**< no. of aborted test functions */
RCU_U4 reg_nr_succ_assert;          /**< no. of successful assertions */
RCU_U4 reg_nr_fail_assert;          /**< no. of failed assertions */
RCU_U4 reg_nr_fatal_assert;         /**< no. of fatal assertions */

/** current test module run statistics */
RCU_U4 mod_nr_fin_test;             /**< no. of completed test functions */
RCU_U4 mod_nr_fail_test;            /**< no. of failed test functions */
RCU_U4 mod_nr_succ_test;            /**< no. of successful test functions */
RCU_U4 mod_nr_abort_test;           /**< no. of aborted test functions */
RCU_U4 mod_nr_succ_assert;          /**< no. of successful assertions */
RCU_U4 mod_nr_fail_assert;          /**< no. of failed assertions */
RCU_U4 mod_nr_fatal_assert;         /**< no. of fatal assertions */

/**
 *  @brief Generates an HTML test run report
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

/** Reset current registry statistics */
#define RCU_RESET_CURR_REG_STATS()      \
    RCU_RESET(reg_nr_fin_mod);          \
    RCU_RESET(reg_nr_fail_mod);         \
    RCU_RESET(reg_nr_succ_mod);         \
    RCU_RESET(reg_nr_abort_mod);        \
    RCU_RESET(reg_nr_test);             \
    RCU_RESET(reg_nr_fin_test);         \
    RCU_RESET(reg_nr_fail_test);        \
    RCU_RESET(reg_nr_succ_test);        \
    RCU_RESET(reg_nr_abort_test);       \
    RCU_RESET(reg_nr_fail_assert);      \
    RCU_RESET(reg_nr_succ_assert);      \
    RCU_RESET(reg_nr_fatal_assert);


#define RCU_RESET_CURR_MOD_STATS()      \
    RCU_RESET(mod_nr_fin_test);         \
    RCU_RESET(mod_nr_fail_test);        \
    RCU_RESET(mod_nr_succ_test);        \
    RCU_RESET(mod_nr_abort_test);       \
    RCU_RESET(mod_nr_fail_assert);      \
    RCU_RESET(mod_nr_succ_assert);      \
    RCU_RESET(mod_nr_fatal_assert);

#define RCU_HAS_NO_ASSERTS(func) \
    ((func)->nr_succ_assert == 0 && (func)->nr_fail_assert == 0 && (func)->nr_fatal_assert == 0 && !RCU_ABORTED((func)))

#if RCU_ENABLE_REDIRECT_REPORT
#else
FILE *g_report_fp=RCU_NULL;
#endif

/**
 *  @brief Initializes the report interface
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_init_report_iface(){
#if RCU_ENABLE_REDIRECT_REPORT
    RCU_LOG_INFO("[ RCUNIT TEST RUN REPORT REDIRECT START ]");
#else
#if RCU_ENABLE_DEBUG_REPORT
    static s_counter=0;
    RCU_CHAR ts_filename[64];
    memset(ts_filename,0x00,64);
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
    sprintf(ts_filename,"debug_%02lu_%s",s_counter,RCU_TEST_RUN_REPORT_FILENAME_PLAINTEXT);
#else
    sprintf(ts_filename,"debug_%02lu_%s",s_counter,RCU_TEST_RUN_REPORT_FILENAME_HTML);
#endif
    s_counter++;
    if ((g_report_fp=fopen(ts_filename,"wt"))==RCU_NULL){
        return(RCU_E_NG);
    }
#else
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
    if ((g_report_fp=fopen(RCU_TEST_RUN_REPORT_FILENAME_PLAINTEXT,"wt"))==RCU_NULL){
        return(RCU_E_NG);
    }
#else
    if ((g_report_fp=fopen(RCU_TEST_RUN_REPORT_FILENAME_HTML,"wt"))==RCU_NULL){
        return(RCU_E_NG);
    }
#endif
#endif
#endif /** RCU_ENABLE_REDIRECT_REPORT */
    return(RCU_E_OK);
}

/**
 *  @brief Destroys the report interface
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_report_iface(){
#if RCU_ENABLE_REDIRECT_REPORT
    RCU_LOG_INFO("[ RCUNIT TEST RUN REPORT REDIRECT END ]");
#else
    if (g_report_fp != RCU_NULL){
        fflush(g_report_fp);
        fclose(g_report_fp);
    }
#endif
    return(RCU_E_OK);
}

/**
 *  @brief Report interface implementation
 *  @param[in] info Report information
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_report_iface_impl(const RCU_CHAR *info){
#if RCU_ENABLE_REDIRECT_REPORT
    rcu_print(info);
#else
    if (g_report_fp != RCU_NULL){
        fprintf(g_report_fp,info);
    }
#endif
    return(RCU_E_OK);
}

RCU_INT rcu_gen_test_run_report_impl(RCU_TEST_MACHINE *mach){
RCU_GENERIC_LIST *cursor1=RCU_NULL,*cursor2=RCU_NULL,*cursor3=RCU_NULL,*cursor4=RCU_NULL;
RCU_TEST_FUNCTION *func=RCU_NULL;
RCU_TEST_MODULE *mod=RCU_NULL;
RCU_TEST_REGISTRY *reg=RCU_NULL;
RCU_FAILURE_RECORD *fail_rec=RCU_NULL;
RCU_INT reg_counter;
RCU_INT mod_counter;
RCU_INT func_counter;
RCU_CHAR ts_buff[RCU_TSTAMP_BUFF_SIZE];
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);

    if (mach == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVMACH);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }

    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    rcu_init_report_iface();
    RCU_LOG_INFO("Generating test run report");
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
    RCU_REPORT_P1("%s\n",RCU_LINE_MARKER_1);
    RCU_REPORT("                         RCUNIT Test Run Report\n");
    RCU_REPORT_P1("%s\n",RCU_LINE_MARKER_1);
#else
    RCU_REPORT("<!doctype html public \"-//w3c//dtd html 3.2//en\">\n");
    RCU_REPORT("<html>\n");
    RCU_REPORT("<head><title>RCUNIT Test Run Report</title>\n");
    RCU_REPORT("<style><!-- h1, b { font-family: arial, helvetica, sans-serif }--></style>\n");
    RCU_REPORT("</head>\n");
    RCU_REPORT("<body>\n");

    RCU_REPORT("<br>\n");
    RCU_REPORT_P1("<table width=\"%s align=\"center\">\n","100%%");
    RCU_REPORT("<tr style=\"background-color:#7b68ee\"><td><br><center><font size=\"+2\"><b>RCUNIT Test Run Report</b></font></center></td></tr>\n");
    RCU_REPORT("</table>\n");
#endif
    /** Reset test machine run statistics */
    memset(&mach->run_stats,0x00,sizeof(RCU_RUN_STATISTICS));
    reg_counter =1;
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor1){
        reg = (RCU_TEST_REGISTRY*)cursor1;
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
        RCU_REPORT_P2("%lu. Test registry run results (%s)\n",reg_counter,reg->name);
#else
        RCU_REPORT("<hr>\n");
        RCU_REPORT_P2("<font size=\"+1\"><b>%lu. Test registry run results (%s)</b></font><br><br>\n",reg_counter,reg->name);
#endif
        if (!reg->enable){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
            RCU_REPORT("Test registry is disabled!\n\n");
#else
            RCU_REPORT("<font>Test registry is disabled!</font><br>\n");
#endif
            RCU_INCR(reg_counter);
            continue;
        }
        if (rcu_is_list_empty(&reg->mod_list)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
            RCU_REPORT("Test registry has no test modules!\n\n");
#else
            RCU_REPORT("<font>Test registry has no test modules!</font><br><br>\n");
#endif
            RCU_INCR(reg_counter)
            continue;
        }
        /** Reset current test registry run statistics */
        RCU_RESET_CURR_REG_STATS();
        mod_counter=1;
        RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor2){
            mod = (RCU_TEST_MODULE*)cursor2;
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
            RCU_REPORT_P3("\n%lu.%lu. Test module run results (%s)\n\n",reg_counter,mod_counter,mod->name);
#else
            RCU_REPORT_P3("<h3>%lu.%lu. Test module run results (%s)</h3>\n",reg_counter,mod_counter,mod->name);
#endif
            if (!mod->enable){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                RCU_REPORT("Test module is disabled!\n");
#else
                RCU_REPORT("<font>Test module is disabled!</font><br>\n");
#endif
                RCU_INCR(mod_counter);
                continue;
            }
            if (rcu_is_list_empty(&mod->func_list)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                RCU_REPORT("Test module has no test functions!\n");
#else
                RCU_REPORT("<font>Test module has no test functions!</font>\n");
#endif
                RCU_INCR(mod_counter);
                continue;
            }
            if (RCU_RUN_STAT_INIT_FAILED(mod) || RCU_RUN_STAT_DESTROY_FAILED(mod)){
                if (RCU_ABORTED(mod)){
                    RCU_INCR(reg_nr_abort_mod);
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT("Test module aborted!\n\n");
#else
                    RCU_REPORT("<font>Test module aborted!</font><br><br>\n");
#endif
                }
                if (!rcu_is_list_empty(&mod->fail_rec_list)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT("Test module warnings:\n");
#else
                    RCU_REPORT("<h3><li><i>Test module warnings</i></li></h3>\n");
                    RCU_REPORT("<ol>");
#endif
                    RCU_FOR_EACH_ENTRY(&mod->fail_rec_list,cursor4){
                        fail_rec = (RCU_FAILURE_RECORD*)cursor4;
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                        RCU_REPORT_P1("    %s\n",fail_rec->info);
#else
                        RCU_REPORT_P1("<li>%s</li>\n",fail_rec->info);
#endif
                    }
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
#else
                    RCU_REPORT("</ol>");
#endif
                }
                if (RCU_RUN_STAT_INIT_FAILED(mod)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT("    Oopss : Init function failed!\n");
#else
                    RCU_REPORT("<font style=\"color:red\"> Oopss : Init function failed! </font><br>\n");
#endif
                }else if (RCU_RUN_STAT_DESTROY_FAILED(mod)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT("    Oopss : Destroy function failed!\n");
#else
                    RCU_REPORT("<font style=\"color:red\"> Oopss : Destroy function failed! </font><br>\n");
#endif
                }
                RCU_INCR(mod_counter);
                continue;
            }
            /** Reset current test module run statistics */
            RCU_RESET_CURR_MOD_STATS();
            func_counter=1;
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
            RCU_REPORT("Test Function Result Total Asserts Failed Asserts\n");
#else
            RCU_REPORT_P1("<table width=\"%s\" border=\"1\">\n","50%%");
            RCU_REPORT("<tr align=\"center\" style=\"background-color:#7b68ee\">\n");
            RCU_REPORT("<td>Test Function</td>\n");
            RCU_REPORT("<td>Result</td>\n");
            RCU_REPORT("<td>Total Asserts</td>\n");
            RCU_REPORT("<td>Failed Asserts</td>\n");
            RCU_REPORT("</tr>\n");
#endif
            RCU_FOR_EACH_ENTRY(&mod->func_list,cursor3){
                func = (RCU_TEST_FUNCTION*)cursor3;
                if (!func->enable){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT_P1("  %s    Disabled\n",func->name);
#else
                    RCU_REPORT_P1("<tr><td >%s</td><td align=\"center\"><font style=\"color:blue\">Disabled</font></td><td align=\"center\">0</td><td align=\"center\">0</td></tr>\n",func->name);
#endif
                    RCU_INCR(func_counter);
                    continue;
                }
                /** Collect current test module run statistics */
                if (RCU_RUN_STAT_SUCCEEDED(func)){
                    RCU_INCR(mod_nr_succ_test);
                }else {
                    RCU_INCR(mod_nr_fail_test);
                }
                if (RCU_ABORTED(func)){
                    RCU_INCR(mod_nr_abort_test);
                }
                if (RCU_RUN_STAT_SUCCEEDED(func) && !RCU_ABORTED(func)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT_P3("  %s    Passed    %lu    %lu\n",
                    func->name,
                    func->nr_succ_assert+func->nr_fail_assert+func->nr_fatal_assert,
                    func->nr_fail_assert+func->nr_fatal_assert);
#else
                    RCU_REPORT_P3("<tr><td >%s</td><td align=\"center\" style=\"color:green\">Passed</td><td align=\"center\">%lu</td><td align=\"center\">%lu</td></tr>\n",
                    func->name,
                    func->nr_succ_assert+func->nr_fail_assert+func->nr_fatal_assert,
                    func->nr_fail_assert+func->nr_fatal_assert);
#endif
                }else {
                    if (RCU_ABORTED(func)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                        RCU_REPORT_P3("  %s    Aborted   %lu    %lu\n",
                        func->name,
                        func->nr_succ_assert+func->nr_fail_assert+func->nr_fatal_assert,
                        func->nr_fail_assert+func->nr_fatal_assert);
#else
                        RCU_REPORT_P4("<tr><td ><a href=#%s_fail_list>%s</a></td><td align=\"center\" style=\"color:red\">Aborted</td><td align=\"center\">%lu</td><td align=\"center\">%lu</td></tr>\n",
                        func->name,func->name,
                        func->nr_succ_assert+func->nr_fail_assert+func->nr_fatal_assert,
                        func->nr_fail_assert+func->nr_fatal_assert);
#endif
                    }else{
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                        RCU_REPORT_P3("  %s    Failed    %lu    %lu\n",
                        func->name,
                        func->nr_succ_assert+func->nr_fail_assert+func->nr_fatal_assert,
                        func->nr_fail_assert+func->nr_fatal_assert);
#else
                        RCU_REPORT_P4("<tr><td ><a href=#%s_fail_list>%s</a></td><td align=\"center\" style=\"color:red\">Failed</td><td align=\"center\">%lu</td><td align=\"center\">%lu</td></tr>\n",
                        func->name,func->name,
                        func->nr_succ_assert+func->nr_fail_assert+func->nr_fatal_assert,
                        func->nr_fail_assert+func->nr_fatal_assert);
#endif
                    }
                }
                /** Update current test module run statistics */
                RCU_INCR(mod_nr_fin_test);
                RCU_INCR_BY(mod_nr_fail_assert,func->nr_fail_assert);
                RCU_INCR_BY(mod_nr_succ_assert,func->nr_succ_assert);
                RCU_INCR_BY(mod_nr_fatal_assert,func->nr_fatal_assert);
                RCU_INCR(func_counter);
            }/**  mod->func_list */
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
#else
            RCU_REPORT("</table>\n");
#endif

            /** Dump the failure records */
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
            RCU_REPORT("\nTest function warnings:\n\n");
#else
            RCU_REPORT("<h3><li><i>Test function warnings</i></li></h3>\n");
            RCU_REPORT("<ol>");
#endif
            RCU_FOR_EACH_ENTRY(&mod->func_list,cursor3){
                func = (RCU_TEST_FUNCTION*)cursor3;
                if (!func->enable){
                    continue;
                }
                if (rcu_is_list_empty(&func->fail_rec_list) && !RCU_HAS_NO_ASSERTS(func)){
                    continue;
                }
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                RCU_REPORT_P1("%s\n",func->name);
#else
                RCU_REPORT_P2("<a name=\"%s_fail_list\"><li>%s</li></a>\n",func->name,func->name);
#endif

                if (func->nr_succ_assert == 0 && func->nr_fail_assert == 0
                    && func->nr_fatal_assert == 0 && !RCU_ABORTED(func)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT("    Oopss : No assertion statements found!\n");
#else
                    RCU_REPORT("<br><font style=\"color:red\"> Oopss : No assertion statements found!</font>\n");
#endif
                }
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
#else
                RCU_REPORT("<ul>");
#endif
                RCU_FOR_EACH_ENTRY(&func->fail_rec_list,cursor4){
                    fail_rec = (RCU_FAILURE_RECORD*)cursor4;
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT_P1("    %s\n",fail_rec->info);
#else
                    RCU_REPORT_P1("<li>%s</li>\n",fail_rec->info);
#endif
                }
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
#else
                RCU_REPORT("</ul>");
#endif
                if (RCU_RUN_STAT_INIT_FAILED(func)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT("    Oopss : Init function failed!\n");
#else
                    RCU_REPORT("<font style=\"color:red\"> Oopss : Init function failed!  </font><br>\n");
#endif
                }
                if (RCU_RUN_STAT_DESTROY_FAILED(func)){
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
                    RCU_REPORT("    Oopss : Destroy function failed!\n");
#else
                    RCU_REPORT("<font style=\"color:red\"> Oopss : Destroy function failed! </font><br>\n");
#endif
                }
            } /** mod->func_list */
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
#else
            RCU_REPORT("</ol>");
#endif
            /** Dump current test module run statistics */
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
            RCU_REPORT("\nTest module run statistics\n");
            RCU_REPORT_P1("Test module                  : %s\n",mod->name);
            RCU_REPORT_P1("Registered test functions    : %lu\n",mod->nr_test);
            RCU_REPORT_P1("Completed test functions     : %lu\n",mod_nr_fin_test);
            RCU_REPORT_P1("*Passed                      : %lu\n",mod_nr_succ_test);
            RCU_REPORT_P1("*Failed                      : %lu\n",mod_nr_fail_test);
            RCU_REPORT_P1("*Aborted                     : %lu\n",mod_nr_abort_test);
#else
            RCU_REPORT("<h3><li><i>Test module run statistics</i></li></h3>\n");
            RCU_REPORT_P1("<table width=\"%s\" border=\"1\">\n","50%%");
            RCU_REPORT_P1("<tr style=\"background-color:#7b68ee\"><td >Test module</td><td align=\"center\">%s</td>\n",mod->name);
            RCU_REPORT_P1("<tr><td>Registered test functions</td><td align=\"center\">%lu</td>\n",mod->nr_test);
            RCU_REPORT_P1("<tr><td>Completed test functions</td><td align=\"center\">%lu</td>\n",mod_nr_fin_test);
            RCU_REPORT_P1("<tr><td><li>Passed</li></td><td align=\"center\">%lu</td>\n",mod_nr_succ_test);
            RCU_REPORT_P1("<tr><td><li>Failed</li></td><td align=\"center\">%lu</td>\n",mod_nr_fail_test);
            RCU_REPORT_P1("<tr><td><li>Aborted</li></td><td align=\"center\">%lu</td>\n",mod_nr_abort_test);
            RCU_REPORT("</table>\n");
#endif
            /** Update current test registry statistics */
            if (mod_nr_fail_test == 0){
                RCU_INCR(reg_nr_succ_mod);
            }else {
                RCU_INCR(reg_nr_fail_mod);
            }
            RCU_INCR(reg_nr_fin_mod);
            RCU_INCR_BY(reg_nr_test,mod->nr_test);
            RCU_INCR_BY(reg_nr_fin_test,mod_nr_fin_test);
            RCU_INCR_BY(reg_nr_fail_test,mod_nr_fail_test);
            RCU_INCR_BY(reg_nr_succ_test,mod_nr_succ_test);
            RCU_INCR_BY(reg_nr_abort_test,mod_nr_abort_test);
            RCU_INCR_BY(reg_nr_fail_assert,mod_nr_fail_assert);
            RCU_INCR_BY(reg_nr_succ_assert,mod_nr_succ_assert);
            RCU_INCR_BY(reg_nr_fatal_assert,mod_nr_fatal_assert);
            RCU_INCR(mod_counter);
        }/**  reg->mod_list */
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
        RCU_REPORT("\nTest registry run statistics\n");
        RCU_REPORT_P1("Test registry            : %s\n",reg->name);
        RCU_REPORT_P1("Registered test modules  : %lu\n",reg->nr_mod);
        RCU_REPORT_P1("Completed test modules   : %lu\n",reg_nr_fin_mod);
        RCU_REPORT_P1("*Passed                  : %lu\n",reg_nr_succ_mod);
        RCU_REPORT_P1("*Failed                  : %lu\n",reg_nr_fail_mod);
        RCU_REPORT_P1("*Aborted                 : %lu\n",reg_nr_abort_mod);
#else
        RCU_REPORT("<h3><li><i>Test registry run statistics</i></li></h3>\n");
        RCU_REPORT_P1("<table width=\"%s\" border=\"1\">\n","50%%");
        RCU_REPORT_P1("<tr style=\"background-color:#7b68ee\"><td >Test registry</td><td align=\"center\">%s</td>\n",reg->name);
        RCU_REPORT_P1("<tr><td>Registered test modules</td><td align=\"center\">%lu</td>\n",reg->nr_mod);
        RCU_REPORT_P1("<tr><td>Completed test modules </td><td align=\"center\">%lu</td>\n",reg_nr_fin_mod);
        RCU_REPORT_P1("<tr><td><li>Passed</li></td><td align=\"center\">%lu</td >\n",reg_nr_succ_mod);
        RCU_REPORT_P1("<tr><td><li>Failed</li></td><td align=\"center\">%lu</td >\n",reg_nr_fail_mod);
        RCU_REPORT_P1("<tr><td><li>Aborted</li></td><td align=\"center\">%lu</td>\n",reg_nr_abort_mod);
        RCU_REPORT("</table>\n");

#endif
        //RCU_REPORT("<hr>\n");
        if (reg_nr_fail_mod == 0){
            RCU_INCR(mach->run_stats.nr_succ_reg);
        }else{
            RCU_INCR(mach->run_stats.nr_fail_reg);
        }
        if (reg_nr_abort_mod!=0){
            RCU_INCR(mach->run_stats.nr_abort_reg);
        }

        /** Update overall run statistics */
        RCU_INCR(mach->run_stats.nr_fin_reg);

        RCU_INCR_BY(mach->run_stats.nr_mod,reg->nr_mod);
        RCU_INCR_BY(mach->run_stats.nr_fin_mod,reg_nr_fin_mod);
        RCU_INCR_BY(mach->run_stats.nr_fail_mod,reg_nr_fail_mod);
        RCU_INCR_BY(mach->run_stats.nr_succ_mod,reg_nr_succ_mod);
        RCU_INCR_BY(mach->run_stats.nr_abort_mod,reg_nr_abort_mod);

        RCU_INCR_BY(mach->run_stats.nr_test,reg_nr_test);
        RCU_INCR_BY(mach->run_stats.nr_fin_test,reg_nr_fin_test);
        RCU_INCR_BY(mach->run_stats.nr_fail_test,reg_nr_fail_test);
        RCU_INCR_BY(mach->run_stats.nr_succ_test,reg_nr_succ_test);
        RCU_INCR_BY(mach->run_stats.nr_abort_test,reg_nr_abort_test);

        RCU_INCR_BY(mach->run_stats.nr_fail_assert,reg_nr_fail_assert);
        RCU_INCR_BY(mach->run_stats.nr_succ_assert,reg_nr_succ_assert);
        RCU_INCR_BY(mach->run_stats.nr_fatal_assert,reg_nr_fatal_assert);
        RCU_INCR(reg_counter);
        RCU_REPORT_P1("%s\n",RCU_LINE_MARKER_3);
    }
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT

    RCU_REPORT_P1("Test machine                 : %s\n",mach->name);
    RCU_REPORT_P1("Registered test registries   : %lu\n",mach->nr_reg);
    RCU_REPORT_P1("Completed test registries    : %lu\n",mach->run_stats.nr_fin_reg);
    RCU_REPORT_P1("*Passed                      : %lu\n",mach->run_stats.nr_succ_reg);
    RCU_REPORT_P1("*Failed                      : %lu\n",mach->run_stats.nr_fail_reg);
    RCU_REPORT_P1("Registered test modules      : %lu\n",mach->run_stats.nr_mod);
    RCU_REPORT_P1("Completed test modules       : %lu\n",mach->run_stats.nr_fin_mod);
    RCU_REPORT_P1("*Passed                      : %lu\n",mach->run_stats.nr_succ_mod);
    RCU_REPORT_P1("*Failed                      : %lu\n",mach->run_stats.nr_fail_mod);
    RCU_REPORT_P1("*Aborted                     : %lu\n",mach->run_stats.nr_abort_mod);
    RCU_REPORT_P1("Registered test functions    : %lu\n",mach->run_stats.nr_test);
    RCU_REPORT_P1("Completed test functions     : %lu\n",mach->run_stats.nr_fin_test);
    RCU_REPORT_P1("*Passed                      : %lu\n",mach->run_stats.nr_succ_test);
    RCU_REPORT_P1("*Failed                      : %lu\n",mach->run_stats.nr_fail_test);
    RCU_REPORT_P1("*Aborted                     : %lu\n",mach->run_stats.nr_abort_test);
    RCU_REPORT_P1("Assertions                   : %lu\n",mach->run_stats.nr_fail_assert + mach->run_stats.nr_succ_assert + mach->run_stats.nr_fatal_assert);
    RCU_REPORT_P1("*Passed                      : %lu\n",mach->run_stats.nr_succ_assert);
    RCU_REPORT_P1("*Failed(non-fatal)           : %lu\n",mach->run_stats.nr_fail_assert);
    RCU_REPORT_P1("*Failed(fatal)               : %lu\n",mach->run_stats.nr_fatal_assert);
    RCU_REPORT("\n");
#else
    RCU_REPORT("<hr>\n");
    RCU_REPORT("<font size=\"+1\"><b>Test run summary</b></font><br><br>\n");
    RCU_REPORT_P1("<table width=\"%s\" border=\"1\">\n","50%%");
    RCU_REPORT_P1("<tr style=\"background-color:#7b68ee\"><td>Test machine</td><td align=\"center\">%s</td></tr>\n",mach->name);
    RCU_REPORT_P1("<tr><td style=\"background-color:skyblue\">Registered test registries</td><td align=\"center\">%lu</td></tr>\n",mach->nr_reg);
    RCU_REPORT_P1("<tr><td>Completed test registries</td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_fin_reg);
    RCU_REPORT_P1("<tr><td><li>Passed</li></td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_succ_reg);
    RCU_REPORT_P1("<tr><td><li>Failed</li></td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_fail_reg);
    RCU_REPORT_P1("<tr><td style=\"background-color:skyblue\">Registered test modules</td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_mod);
    RCU_REPORT_P1("<tr><td>Completed test modules</td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_fin_mod);
    RCU_REPORT_P1("<tr><td><li>Passed</li></td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_succ_mod);
    RCU_REPORT_P1("<tr><td><li>Failed</li></td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_fail_mod);
    RCU_REPORT_P1("<tr><td><li>Aborted</li></td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_abort_mod);
    RCU_REPORT_P1("<tr><td style=\"background-color:skyblue\">Registered test functions</td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_test);
    RCU_REPORT_P1("<tr><td>Completed test functions</td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_fin_test);
    RCU_REPORT_P1("<tr><td><li>Passed</li></td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_succ_test);
    RCU_REPORT_P1("<tr><td><li>Failed</li></td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_fail_test);
    RCU_REPORT_P1("<tr><td><li>Aborted</li></td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_abort_test);
    RCU_REPORT_P1("<tr><td style=\"background-color:skyblue\">Assertions</td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_fail_assert + mach->run_stats.nr_succ_assert + mach->run_stats.nr_fatal_assert);
    RCU_REPORT_P1("<tr><td><li>Passed</li></td><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_succ_assert);
    RCU_REPORT_P1("<tr><td><li>Failed(non-fatal)</li></td align=\"center\"><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_fail_assert);
    RCU_REPORT_P1("<tr><td><li>Failed(fatal)</li></li></td align=\"center\"><td align=\"center\">%lu</td></tr>\n",mach->run_stats.nr_fatal_assert);
    RCU_REPORT("</table>\n");
    RCU_REPORT("<hr>\n");
#endif
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
#if RCU_ENABLE_EMIT_PLAINTEXT_REPORT
    RCU_REPORT_P1("%s\n",RCU_LINE_MARKER_1);
    RCU_REPORT_P2("Generated on %s by RCUNIT version %s\n",ts_buff,RCU_VERSION_STRING);
#else
    RCU_REPORT_P2("<i><font size=\"-1\" align=\"left\">Generated on %s by <a href=\"http://rcunit.sourceforge.net\"> RCUNIT </a> version %s</i>\n",ts_buff,RCU_VERSION_STRING);
#endif
    rcu_destroy_report_iface();
    RCU_LOG_INFO_P1("Run report generated : %s",RCU_TEST_RUN_REPORT_FILENAME_HTML);
    return(RCU_E_OK);
}

/**
 *  @brief Generates test run report
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_gen_test_run_report(RCU_TEST_MACHINE *mach){
    rcu_gen_test_run_report_impl(mach);
    rcu_del_all_fail_rec(mach);
    return(RCU_E_OK);
}
