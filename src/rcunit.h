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

#ifndef RCUNIT_H
#define RCUNIT_H

/* Tag to indicate an API function */
#define RCU_API

/* Tag to indicate a function parameter is nullable */
#define RCU_NULLABLE

/* Test machine name */
#define RCU_DEFAULT_MACHINE_NAME  "default-machine"

/* Default test registry name  */
#define RCU_DEFAULT_REGISTRY_NAME "default-reg"

/* Default test module name  */
#define RCU_DEFAULT_MODULE_NAME   "default-mod"

/* Timestamp buffer size */
#define RCU_TSTAMP_BUFF_SIZE 32

/** Standard C library header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <time.h>
#include <stdarg.h>
#include <signal.h>

#include "logmoko.h"
#include "rcunit_config.h"
#include "rcunit_types.h"
#include "rcunit_list.h"

/* Test function or module not tested */
#define RCU_RUN_STAT_NOTTESTED      0

/* Test function or module run is successful */
#define RCU_RUN_STAT_TEST_SUCC      1

/* Test function or module run failed status */
#define RCU_RUN_STAT_TEST_FAILED    2

/* Init handler failed status */
#define RCU_RUN_STAT_INIT_FAILED      4

/* Destroy handler failed status */
#define RCU_RUN_STAT_DESTROY_FAILED   8

/* Sets explicit run status */
#define RCU_SET_RUN_STAT(func_or_mod, stat) \
    (func_or_mod)->run_stat = (stat);

#define RCU_SET_INIT_FAILED(func_or_mod) \
    (func_or_mod)->init_failed = (RCU_TRUE);

#define RCU_SET_DESTROY_FAILED(func_or_mod) \
    (func_or_mod)->destroy_failed = (RCU_TRUE);

/* Returns true if the test was not performed */
#define RCU_IS_NOT_TESTED(func_or_mod) \
    ((func_or_mod)->run_stat == RCU_RUN_STAT_NOTTESTED)

/* Returns true if the test run succeeded */
#define RCU_IS_TEST_SUCCEDED(func_or_mod) \
    ((func_or_mod)->run_stat == RCU_RUN_STAT_TEST_SUCC)

/* Returns true if the test run failed */
#define RCU_IS_TEST_FAILED(func_or_mod) \
    ((func_or_mod)->run_stat == RCU_RUN_STAT_TEST_FAILED)

/* Returns true if the init function failed */
#define RCU_IS_INIT_FAILED(func_or_mod) \
    ((func_or_mod)->init_failed)

/* Returns true if the destroy function failed */
#define RCU_IS_DESTROY_FAILED(func_or_mod) \
    ((func_or_mod)->destroy_failed)

/* Sets the abort flag */
#define RCU_SET_ABORT(func_or_mod) \
    (func_or_mod)->aborted = RCU_TRUE;

/* Resets the abort flag */
#define RCU_CLEAR_ABORT(func_or_mod) \
    (func_or_mod)->aborted = RCU_FALSE;

/* Tests the abort flag */
#define RCU_ABORTED(func_or_mod) \
    ((func_or_mod)->aborted)

#define RCU_HAS_NO_ASSERTS(func) \
    ((func)->nr_succ_assert == 0 && (func)->nr_fail_assert == 0 && !RCU_ABORTED((func)))

/* WARNING:
 *  1. Do not change the order of the fields of the RCU_TEST_FUNCTION_ENTRY
 *     and RCU_TEST_MODULE_ENTRY structures. Some macros rely on the order
 *     of these fields.
 *  2. Do not move the position of the "link" fields.
 */

#define RCU_TEST_FUNCTION_NAME_LENGTH 255
#define RCU_TEST_MODULE_NAME_LENGTH 255
#define RCU_TEST_REGISTRY_NAME_LENGTH 255

/* Test function table entry */
typedef struct rcu_test_function_entry_tag {
    const char *name;
    rcu_generic_function entry;
    rcu_generic_function init;
    rcu_generic_function destroy;
} rcu_test_function_entry;

/* Test module table entry */
typedef struct rcu_module_entry_tag {
    const char *name;
    rcu_generic_function init;
    rcu_generic_function destroy;
    rcu_test_function_entry *func_tbl;
} rcu_module_entry;

/* Test registry runtime data structure */
typedef struct rcu_registry_tag {
    rcu_list link;
    rcu_list mod_list;
    char name[RCU_TEST_REGISTRY_NAME_LENGTH + 1];
    int nr_mod;
    int nr_failed_mod;
    int nr_succ_mod;
    int nr_succ_test;
    int nr_failed_test;
} rcu_registry;

/* Test module runtime data structure */
typedef struct rcu_module_tag {
    rcu_list link;
    rcu_generic_function init;
    rcu_generic_function destroy;
    rcu_list func_list;
    char name[RCU_TEST_MODULE_NAME_LENGTH + 1];
    int nr_test;
    int nr_failed_test;
    int nr_succ_test;
    int run_stat;
    int init_failed;
    int destroy_failed;
    rcu_list fail_rec_list;
} rcu_module;

/*  Failure record entry */
typedef struct rcu_failure_record {
    rcu_list link;
    char *info;
} rcu_failure_record;

/* Test function runtime data structure */
typedef struct rcu_test_tag {
    rcu_list link;
    char name[RCU_TEST_FUNCTION_NAME_LENGTH];
    rcu_generic_function entry;
    rcu_generic_function init;
    rcu_generic_function destroy;
    int run_stat;
    int init_failed;
    int destroy_failed;
    int nr_fail_assert;
    int nr_succ_assert;
    rcu_list fail_rec_list;
} rcu_test;

/*  Sets the currently executing test function */
#define RCU_SET_CURR_FUNC(machine, func) \
    (machine)->ae.curr_func = func;

/* Sets the currently executing test module */
#define RCU_SET_CURR_MOD(machine, mod) \
    (machine)->ae.curr_mod = mod;

/* Sets the currently executing test registry */
#define RCU_SET_CURR_REG(machine, reg)   \
    (machine)->ae.curr_reg = reg;

/* Gets the currently executing test function */
#define RCU_GET_CURR_FUNC(machine)  \
    (machine->ae.curr_func)

/* Gets the currently executing test module */
#define RCU_GET_CURR_MOD(machine)  \
    (machine->ae.curr_mod)

/* Gets the currently executing test registry  */
#define RCU_GET_CURR_REG(machine)  \
    (machine->ae.curr_reg)

/* Gets the currently executing test machine */
#define RCU_THE_TEST_MACHINE (&the_test_machine)

/* Test run levels (i.e. when a test module is run through its reference or
 *  name, it is said to run in test module level)
 */
/* Test machine run level */
#define RCU_RUN_LEVEL_MACH   0

/* Test registry run level */
#define RCU_RUN_LEVEL_REG    1

/* Test module run level */
#define RCU_RUN_LEVEL_MOD    2

/* Sets the run level */
#define RCU_SET_RUN_LEVEL(machine, level) \
    (machine)->run_level = level;

/* Gets the run level */
#define RCU_GET_RUN_LEVEL(machine) \
    ((machine)->run_level)

/* Assertion engine runtime data structure */
typedef struct rcu_assertion_engine {
    rcu_registry *curr_reg;
    rcu_module *curr_mod;
    rcu_test *curr_func;
    rcu_list assert_list;
    rcu_generic_function assert_hook;
} rcu_assertion_engine;

/* Test run started */
#define RCU_TEST_RUN_STARTED    0

/* Test run finished */
#define RCU_TEST_RUN_FINISHED   1

/* Returns the run event type from the test run hook parameter */
#define RCU_GET_RUN_EVT_TYPE(param) \
    (*((int*)param))

/* Test function run context */
#define RCU_RUN_CTX_UNKNOWN         0

/* Test function run context */
#define RCU_RUN_CTX_FUNC            1

/* Test function init run context */
#define RCU_RUN_CTX_FUNC_INIT       2

/* Test function destroy run context */
#define RCU_RUN_CTX_FUNC_DESTROY    3

/* Test module init run context */
#define RCU_RUN_CTX_MOD_INIT        4

/* Test module destroy run context */
#define RCU_RUN_CTX_MOD_DESTROY     5

/** Set test machine run context */
#define RCU_SET_RUN_CTX(machine, ctx)   (machine)->run_ctx = (ctx);

/*  Get test machine run context */
#define RCU_GET_RUN_CTX(machine)   ((machine)->run_ctx)

/* HTML log file name */
#define RCU_LOG_FILENAME_HTML  "rcunit_log.html"

/* Plain text log file name */
#define RCU_LOG_FILENAME_PLAINTEXT  "rcunit_log.txt"

/* Test machine runtime data structure */
typedef struct rcu_test_machine_tag {
    rcu_module def_mod;
    rcu_registry def_reg;
    rcu_list reg_list;
    const char *name;
    int init_done;
    rcu_assertion_engine ae;
    int run_level;
    int run_ctx;
    rcu_generic_function run_hook;
    int nr_reg;
    int nr_failed_reg;
    int nr_succ_reg;
    int nr_failed_test;
    int nr_succ_test;
    int terminate_on_first_failure;
} rcu_test_machine;

void rcu_assert_impl(int cond, const char *filename, const char *func_name, int line, const char *format, ...);

int rcu_get_timestamp(char *ts_buff, const int ts_buff_len);

int rcu_is_mach_initialized(rcu_test_machine *machine);

int rcu_del_all_fail_rec(rcu_test_machine *machine);

int rcu_del_all_fail_rec_from_func(rcu_test *func);

int rcu_del_all_fail_rec_from_mod(rcu_module *mod);

void rcu_print(const char *str);

void rcu_print_rcunit_info();

rcu_module *rcu_cre_test_mod(const char *name, rcu_generic_function init,
                             rcu_generic_function destroy);

/* Other RCUNIT header files */
#include "rcunit_helpers.h"
#include "rcunit_error.h"
#include "rcunit_assert.h"
#include "rcunit_report.h"
#include "rcunit_exception.h"
#include "rcunit_mem.h"
#include "rcunit_mtrace.h"
#include "rcunit_random.h"
#include "rcunit_api.h"

/* External variable declarations (used internally) */
extern const char *g_error_msg_tbl[];
extern int g_ercd;
extern rcu_test_machine the_test_machine;
extern lmk_logger *the_rcu_logger;


#endif /* RCUNIT_H */

