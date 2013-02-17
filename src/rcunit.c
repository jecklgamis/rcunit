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

/* The test machine */
rcu_test_machine the_test_machine;

/* The RCUNIT logger */
lmk_logger *the_rcu_logger;

/* The RCUNIT log file handler */
lmk_log_handler *the_rcu_file_log_hnd;

/* The RCUNIT console file handler */
lmk_log_handler *the_rcu_console_log_hnd;

extern rcu_test *rcu_srch_test_func_by_name(rcu_module *mod,
        const char *name);
extern rcu_test *rcu_srch_test_func_entry(rcu_module *mod,
        rcu_generic_function entry);

RCU_API int rcu_run_test_mach() {
    rcu_test_machine *machine = &the_test_machine;
    char ts_buff[RCU_TSTAMP_BUFF_SIZE];
    rcu_init();
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO("Test run started %s", ts_buff);
    RCU_SET_RUN_LEVEL(machine, RCU_RUN_LEVEL_MACH);
    rcu_run_tests_impl(machine);
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    rcu_stop_mach(machine);
    rcu_gen_test_run_report(machine);
    RCU_LOG_INFO("Test run finished %s", ts_buff);
    RCU_LOG_INFO("Test Run Results: Passed : %d  Failed : %d", machine->nr_succ_test, machine->nr_failed_test);
    if (machine->nr_failed_reg == 0 && rcu_get_nr_tests() > 0 ) {
        RCU_LOG_INFO("Test successful!");
    } else {
        RCU_LOG_INFO("Test failed!");
    }
    rcu_destroy();
    return machine->nr_failed_reg > 0 ? RCU_E_NG : RCU_E_OK;
}

RCU_API int rcu_run_tests() {
    return rcu_run_test_mach();
}

void rcu_print_rcunit_info() {
    RCU_LOG_DEBUG("[ RCUNIT INFORMATION ]");
    RCU_LOG_DEBUG("RCUNIT version is %s (Built last %s %s)", RCU_VERSION_STRING, __DATE__, __TIME__);
#ifdef RCU_DEBUG
    RCU_LOG_DEBUG("RCUNIT is running in debug mode");
#else
    RCU_LOG_DEBUG("RCUNIT is not running in debug mode");
#endif

#if RCU_ENABLE_MTRACE
    RCU_LOG_DEBUG("Memory leak checking is enabled");
#else
    RCU_LOG_DEBUG("Memory leak checking  is disabled");
#endif

#if RCU_ENABLE_MALLOC_ALIGNMENT
    RCU_LOG_DEBUG("Memory allocation aligned to %u bytes", RCU_DEFAULT_MALLOC_ALIGNMENT);
#endif
    RCU_LOG_DEBUG("sizeof(char) is %u bytes", sizeof (char));
    RCU_LOG_DEBUG("sizeof(rcu_short) is %u bytes", sizeof (rcu_short));
    RCU_LOG_DEBUG("sizeof(int) is %u bytes", sizeof (int));
    RCU_LOG_DEBUG("sizeof(rcu_long) is %u bytes", sizeof (rcu_long));
    RCU_LOG_DEBUG("sizeof(rcu_float) is %u bytes", sizeof (rcu_float));
    RCU_LOG_DEBUG("sizeof(rcu_double) is %u bytes", sizeof (rcu_double));
    RCU_LOG_DEBUG("sizeof(rcu_long_long) is %u bytes", sizeof (rcu_long_long));
    RCU_LOG_DEBUG("sizeof(rcu_long_double) is %u bytes", sizeof (rcu_long_double));
    RCU_LOG_DEBUG("sizeof(void*) is %lu bytes", sizeof (void*));
    RCU_LOG_DEBUG("[ RCUNIT INFORMATION END ]");
}

RCU_API int rcu_init() {
    rcu_test_machine *machine = &the_test_machine;
    if (!rcu_is_mach_initialized(machine)) {
        rcu_init_log();
        RCU_LOG_DEBUG("Initializing RCUNIT");
        rcu_print_rcunit_info();
        rcu_init_test_mach(machine);
    }
    return RCU_E_OK;
}

RCU_API int rcu_destroy() {
    rcu_test_machine *machine = &the_test_machine;
    RCU_LOG_DEBUG("Destroying RCUNIT");
    if (rcu_is_mach_initialized(machine)) {
        rcu_destroy_test_mach(machine);
        rcu_destroy_log();
    }
    return RCU_E_OK;
}

int rcu_exit_hook(void *param) {
    RCU_LOG_FATAL("Terminating");
    rcu_destroy();
    return RCU_E_OK;
}

RCU_API int rcu_set_run_hook(rcu_generic_function run_hook) {
    rcu_test_machine *machine = &the_test_machine;
    rcu_init();
    machine->run_hook = run_hook;
    RCU_LOG_DEBUG("Test run hook set");
    return RCU_E_OK;
}

int rcu_destroy_log() {
    if (the_rcu_logger != NULL) {
        return lmk_destroy_logger(&the_rcu_logger);
    }
    lmk_destroy();
    return RCU_E_OK;
}

int rcu_init_log() {
    lmk_init();
    if ((the_rcu_logger = lmk_get_logger("rcunit")) == NULL) {
        fprintf(stdout, "Unable to get logger");
        return RCU_E_NG;
    }
#if RCU_DEBUG
    int log_level = LMK_LOG_LEVEL_DEBUG;
#else
    int log_level = LMK_LOG_LEVEL_INFO;
#endif    
    lmk_set_log_level(the_rcu_logger, log_level);
    the_rcu_file_log_hnd = lmk_get_file_log_handler("rcunit-flh", "rcunit.log");
    if (the_rcu_file_log_hnd != NULL) {
        the_rcu_file_log_hnd->log_level = LMK_LOG_LEVEL_TRACE;
        lmk_attach_log_handler(the_rcu_logger, the_rcu_file_log_hnd);
    }

    the_rcu_console_log_hnd = lmk_get_console_log_handler();
    if (the_rcu_console_log_hnd != NULL) {
        the_rcu_console_log_hnd->log_level = log_level;
        lmk_attach_log_handler(the_rcu_logger, the_rcu_console_log_hnd);
    }
    return RCU_E_OK;
}


