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

#ifndef RCUNIT_H
#define RCUNIT_H

/* Tag to indicate an API function */
#define RCU_API

/* Tag to indicate a function parameter is nullable */
#define RCU_NULLABLE

/* Test engine name */
#define RCU_DEFAULT_ENGINE_NAME  "default-engine"

/* Default test registry name  */
#define RCU_DEFAULT_REGISTRY_NAME "default-reg"

/* Default test module name  */
#define RCU_DEFAULT_MODULE_NAME   "default-module"

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
#define RCU_SET_RUN_STAT(func_or_module, stat) \
    (func_or_module)->run_stat = (stat);

#define RCU_SET_INIT_FAILED(func_or_module) \
    (func_or_module)->init_failed = (RCU_TRUE);

#define RCU_SET_DESTROY_FAILED(func_or_module) \
    (func_or_module)->destroy_failed = (RCU_TRUE);

/* Returns true if the test was not performed */
#define RCU_IS_NOT_TESTED(func_or_module) \
    ((func_or_module)->run_stat == RCU_RUN_STAT_NOTTESTED)

/* Returns true if the test run succeeded */
#define RCU_IS_TEST_SUCCEDED(func_or_module) \
    ((func_or_module)->run_stat == RCU_RUN_STAT_TEST_SUCC)

/* Returns true if the test run failed */
#define RCU_IS_TEST_FAILED(func_or_module) \
    ((func_or_module)->run_stat == RCU_RUN_STAT_TEST_FAILED)

/* Returns true if the init function failed */
#define RCU_IS_INIT_FAILED(func_or_module) \
    ((func_or_module)->init_failed)

/* Returns true if the destroy function failed */
#define RCU_IS_DESTROY_FAILED(func_or_module) \
    ((func_or_module)->destroy_failed)

/* Sets the abort flag */
#define RCU_SET_ABORT(func_or_module) \
    (func_or_module)->aborted = RCU_TRUE;

/* Resets the abort flag */
#define RCU_CLEAR_ABORT(func_or_module) \
    (func_or_module)->aborted = RCU_FALSE;

/* Tests the abort flag */
#define RCU_ABORTED(func_or_module) \
    ((func_or_module)->aborted)

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
struct rcu_test_function_entry {
    const char *name;
    rcu_generic_function entry;
    rcu_generic_function init;
    rcu_generic_function destroy;
};

/* Test module table entry */
struct rcu_module_entry {
    const char *name;
    rcu_generic_function init;
    rcu_generic_function destroy;
    struct rcu_test_function_entry *func_tbl;
};

/* Test registry runtime data structure */
struct rcu_registry {
    struct rcu_list link;
    struct rcu_list mod_list;
    char name[RCU_TEST_REGISTRY_NAME_LENGTH + 1];
    int nr_module;
    int nr_failed_module;
    int nr_succ_module;
    int nr_succ_test;
    int nr_failed_test;
};

/* Test module runtime data structure */
struct rcu_module {
    struct rcu_list link;
    rcu_generic_function init;
    rcu_generic_function destroy;
    struct rcu_list func_list;
    char name[RCU_TEST_MODULE_NAME_LENGTH + 1];
    int nr_test;
    int nr_failed_test;
    int nr_succ_test;
    int run_stat;
    int init_failed;
    int destroy_failed;
    struct rcu_list fail_rec_list;
};

/*  Failure record entry */
struct rcu_failure_record {
    struct rcu_list link;
    char *info;
};

/* Test function runtime data structure */
struct rcu_test {
    struct rcu_list link;
    char name[RCU_TEST_FUNCTION_NAME_LENGTH];
    rcu_generic_function entry;
    rcu_generic_function init;
    rcu_generic_function destroy;
    int run_stat;
    int init_failed;
    int destroy_failed;
    int nr_fail_assert;
    int nr_succ_assert;
    struct rcu_list fail_rec_list;
};

/*  Sets the currently executing test function */
#define RCU_SET_CURR_FUNC(engine, func) \
    (engine)->ae.curr_func = func;

/* Sets the currently executing test module */
#define RCU_SET_CURR_MODULE(engine, module) \
    (engine)->ae.curr_module = module;

/* Sets the currently executing test registry */
#define RCU_SET_CURR_REG(engine, reg)   \
    (engine)->ae.curr_reg = reg;

/* Gets the currently executing test function */
#define RCU_GET_CURR_FUNC(engine)  \
    (engine->ae.curr_func)

/* Gets the currently executing test module */
#define RCU_GET_CURR_MODULE(engine)  \
    (engine->ae.curr_module)

/* Gets the currently executing test registry  */
#define RCU_GET_CURR_REG(engine)  \
    (engine->ae.curr_reg)

/* Gets the currently executing test engine */
#define RCU_THE_TEST_ENGINE (&the_test_engine)

/* Test run levels (i.e. when a test module is run through its reference or
 *  name, it is said to run in test module level)
 */
/* Test engine run level */
#define RCU_RUN_LEVEL_ENGINE   0

/* Test registry run level */
#define RCU_RUN_LEVEL_REG    1

/* Test module run level */
#define RCU_RUN_LEVEL_MODULE    2


/* Assertion engine runtime data structure */
struct rcu_assertion_engine {
    struct rcu_registry *curr_reg;
    struct rcu_module *curr_module;
    struct rcu_test *curr_func;
    struct rcu_list assert_list;
    rcu_generic_function assert_hook;
};

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
#define RCU_RUN_CTX_MODULE_INIT        4

/* Test module destroy run context */
#define RCU_RUN_CTX_MODULE_DESTROY     5

/** Set test engine run context */
#define RCU_SET_RUN_CTX(engine, ctx)   (engine)->run_ctx = (ctx);

/*  Get test engine run context */
#define RCU_GET_RUN_CTX(engine)   ((engine)->run_ctx)

/* HTML log file name */
#define RCU_LOG_FILENAME_HTML  "rcunit_log.html"

/* Plain text log file name */
#define RCU_LOG_FILENAME_PLAINTEXT  "rcunit_log.txt"

/* Test engine runtime data structure */
struct rcu_test_engine {
    struct rcu_module def_module;
    struct rcu_registry def_reg;
    const char *name;
    int init_done;
    struct rcu_assertion_engine ae;
    int run_level;
    int run_ctx;
    rcu_generic_function run_hook;
    int nr_failed_reg;
    int nr_succ_reg;
    int nr_failed_test;
    int nr_succ_test;
    int terminate_on_first_failure;
};

void rcu_assert_impl(int cond, const char *filename, const char *func_name, int line, const char *format, ...);

int rcu_get_timestamp(char *ts_buff, const int ts_buff_len);

int rcu_is_engine_initialized(struct rcu_test_engine *engine);

int rcu_del_all_fail_rec(struct rcu_test_engine *engine);

int rcu_del_all_fail_rec_from_func(struct rcu_test *func);

int rcu_del_all_fail_rec_from_module(struct rcu_module *module);

void rcu_print(const char *str);

void rcu_print_rcunit_info();

struct rcu_module *rcu_create_test_module(const char *name, rcu_generic_function init,
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
#include "rcunit_thread.h"
#include "rcunit_api.h"

/* External variable declarations (used internally) */
extern const char *g_error_msg_tbl[];
extern int g_ercd;
extern struct rcu_test_engine the_test_engine;

extern int rcu_add_fail_rec_to_module(struct rcu_module *module, const char *info, const char *filepath, const int line_no,
                                   int fatal);

extern int rcu_run_test_reg_impl(struct rcu_test_engine *engine, struct rcu_registry *reg);

extern int rcu_add_fail_rec_to_func(struct rcu_test *func, const char *info, const char *filepath, const int line_no);

extern int rcu_add_test_func(struct rcu_module *module, rcu_generic_function entry, rcu_generic_function init,
                             rcu_generic_function destroy, const char *name);

extern int rcu_run_tests_impl(struct rcu_test_engine *engine);

extern int rcu_stop_engine(struct rcu_test_engine *engine);

extern void rcu_gen_test_run_report(struct rcu_test_engine *engine);

extern int rcu_get_nr_tests();

extern int rcu_get_nr_mods();

extern int rcu_has_mem_leak();

extern int rcu_add_fail_rec_impl(struct rcu_list *fail_rec_list, const char *info, const char *filename,
                                 const char *func_name, int line_no);

extern void rcu_init_exception();

extern int rcu_init_reg(struct rcu_registry *reg, const char *name);

extern int rcu_init_module(struct rcu_module *module, rcu_generic_function init, rcu_generic_function destroy, const char *name);

extern int rcu_restart_engine(struct rcu_test_engine *engine);

extern int rcu_run_test_module_impl(struct rcu_test_engine *engine, struct rcu_module *module);

extern void rcu_destroy_exception();

extern int rcu_destroy_test_dbase(struct rcu_test_engine *engine);

extern int rcu_stop_assert_engine(struct rcu_test_engine *engine);

extern int rcu_del_all_fail_rec_impl(struct rcu_list *fail_rec_list);

extern int rcu_free_test_func(struct rcu_test *func);

int rcu_run_test_func_impl(struct rcu_test_engine *engine, struct rcu_test *func);

#endif /* RCUNIT_H */

