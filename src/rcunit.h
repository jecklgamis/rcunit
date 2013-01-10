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


#ifndef RCUNIT_H
#define RCUNIT_H

/** @brief RCU interface tag */
#define RCU_INTERFACE

/** @brief Test machine name */
#define RCU_DEFAULT_MACHINE_NAME  "default-mach"
/** @brief Default test registry name  */
#define RCU_DEFAULT_REGISTRY_NAME "default-reg"
/** @brief Default test module name  */
#define RCU_DEFAULT_MODULE_NAME   "default-mod"

/** @brief Timestamp buffer size */
#define RCU_TSTAMP_BUFF_SIZE 32

/** Standard C library header files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <time.h>

/** Other RCUNIT header files */
#include <rcunit_config.h>
#include <rcunit_types.h>
#include <rcunit_list.h>
#include <rcunit_hashtable.h>
#include <rcunit_io_device.h>

/** @brief "True" definition */
#define RCU_TRUE 1

/** @brief "False" definition */
#define RCU_FALSE (!RCU_TRUE)

/** @brief Test function not tested */
#define RCU_RUN_STAT_NOTTESTED      0
/** @brief Test run is successful */
#define RCU_RUN_STAT_SUCC           1
/** @brief Test run failed status */
#define RCU_RUN_STAT_FAIL           2
/** @brief Init handler failed status*/
#define RCU_RUN_STAT_FAIL_INIT      4
/** @brief Destroy handler failed status */
#define RCU_RUN_STAT_FAIL_DESTROY   8

/** @brief Sets explicit run status */
#define RCU_SET_RUN_STAT(func_or_mod, stat) \
    (func_or_mod)->run_stat |= (stat);

/** @brief Returns true the test run succeeded */
#define RCU_RUN_STAT_SUCCEEDED(func_or_mod) \
    ((func_or_mod)->run_stat & RCU_RUN_STAT_SUCC)

/** @brief Returns true the test run failed */
#define RCU_RUN_STAT_FAILED(func_or_mod) \
    ((func_or_mod)->run_stat & RCU_RUN_STAT_FAIL)

/** @brief Returns true if the init function failed */
#define RCU_RUN_STAT_INIT_FAILED(func_or_mod) \
    ((func_or_mod)->run_stat & RCU_RUN_STAT_FAIL_INIT)

/** @brief Returns true if the destroy function failed */
#define RCU_RUN_STAT_DESTROY_FAILED(func_or_mod) \
    ((func_or_mod)->run_stat & RCU_RUN_STAT_FAIL_DESTROY)

/** @brief Sets the abort flag */
#define RCU_SET_ABORT(func_or_mod) \
    (func_or_mod)->aborted = RCU_TRUE;

/** @brief Resets the abort flag */
#define RCU_CLEAR_ABORT(func_or_mod) \
    (func_or_mod)->aborted = RCU_FALSE;

/** @brief Tests the abort flag */
#define RCU_ABORTED(func_or_mod) \
    ((func_or_mod)->aborted)


/** WARNING:
 *  1. Do not change the order of the fields of the RCU_TEST_FUNCTION_ENTRY
 *     and RCU_TEST_MODULE_ENTRY structures.
 *  2. Do not move the position of the "link" fields.
 */

/** @brief Test function table entry */
typedef struct rcu_test_function_entry {
const RCU_CHAR *name;               /**< Test function name */
RCU_GENERIC_FUNCTION entry;         /**< Test function entry point */
RCU_GENERIC_FUNCTION init;          /**< Init function */
RCU_GENERIC_FUNCTION destroy;       /**< Destroy function */
RCU_INT enable;                     /**< Run enable flag */
}RCU_TEST_FUNCTION_ENTRY;

/** @brief Test module table entry */
typedef struct rcu_test_module_entry {
const RCU_CHAR *name;                   /**< Test module name */
RCU_GENERIC_FUNCTION init;          /**< Init function */
RCU_GENERIC_FUNCTION destroy;       /**< Destroy function */
RCU_TEST_FUNCTION_ENTRY *func_tbl;  /**< Test function table */
RCU_INT enable;                         /**< Run enable flag */
}RCU_TEST_MODULE_ENTRY;

/** @brief Test registry runtime data structure */
typedef struct rcu_test_registry {
RCU_GENERIC_LIST link;              /**< Link to the next/previous test registry */
RCU_GENERIC_LIST mod_list;          /**< List of test modules */
const RCU_CHAR *name;               /**< Test registry name */
RCU_INT enable;                     /**< Run enable flag */
RCU_U4 nr_mod;                      /**< No. of registered test modules */
}RCU_TEST_REGISTRY;

/** @brief Test module runtime data structure */
typedef struct rcu_test_module {
RCU_GENERIC_LIST link;              /**< Link to the next/previous test module */
RCU_GENERIC_FUNCTION init;          /**< Init function */
RCU_GENERIC_FUNCTION destroy;       /**< Destroy function */
RCU_GENERIC_LIST func_list;         /**< List of test functions */
const RCU_CHAR *name;               /**< Test module name */
RCU_INT enable;                     /**< Run enable flag */
RCU_INT aborted;                    /**< Abort flag */
RCU_U4 nr_test;                     /**< No. of registered test functions */
RCU_INT run_stat;                   /**< Test module run status (set if init or destroy failed)*/
RCU_GENERIC_LIST fail_rec_list;     /**< Failure record list */
}RCU_TEST_MODULE;

/**  @brief  Failure record entry */
typedef struct rcu_failure_record {
RCU_GENERIC_LIST link;              /**< Link to the next/previous record */
RCU_CHAR *info;                     /**< Information */
}RCU_FAILURE_RECORD;

/** @brief Test function runtime data structure */

/* The nr_fatal_assert cannot go higher than 1. This can be used to
 * determine if the aborted function is caused by a fatal assertion
 * inside the test function itself.
 */
typedef struct rcu_test_functin {
RCU_GENERIC_LIST link;              /**< Link to the next/previous test function */
const RCU_CHAR *name;               /**< Test function name */
RCU_GENERIC_FUNCTION entry;         /**< Entry point */
RCU_GENERIC_FUNCTION init;          /**< Init function */
RCU_GENERIC_FUNCTION destroy;       /**< Destroy function */
RCU_INT enable;                     /**< Run enable flag */
RCU_INT aborted;                    /**< Abort flag */
RCU_INT run_stat;                   /**< Run status */
RCU_U4 nr_fail_assert;              /**< No. of failed assertions */
RCU_U4 nr_succ_assert;              /**< No. of successful assertions */
RCU_U4 nr_fatal_assert;             /**< No. of fatal assertions */
RCU_GENERIC_LIST fail_rec_list;     /**< Failure record list */
}RCU_TEST_FUNCTION;

/** @brief  Sets the currently executing test function */
#define RCU_SET_CURR_FUNC(mach,func) \
    (mach)->ae.curr_func=func;

/**  @brief Sets the currently executing test module */
#define RCU_SET_CURR_MOD(mach,mod) \
    (mach)->ae.curr_mod=mod;

/**  @brief Sets the currently executing test registry */
#define RCU_SET_CURR_REG(mach,reg)   \
    (mach)->ae.curr_reg=reg;

/**  @brief Gets the currently executing test function */
#define RCU_GET_CURR_FUNC(mach)  \
    (mach->ae.curr_func)

/**  @brief Gets the currently executing test module */
#define RCU_GET_CURR_MOD(mach)  \
    (mach->ae.curr_mod)

/**  @brief Gets the currently executing test registry  */
#define RCU_GET_CURR_REG(mach)  \
    (mach->ae.curr_reg)

/**  @brief Gets the currently executing test machine */
#define RCU_GET_CURR_MACH() \
    (&the_machine)

/* Test run levels (i.e. when a test module is run through its reference or
 *  name, it is said to run in test module level)
 */
/** @brief Test machine run level */
#define RCU_RUN_LEVEL_MACH   0
/** @brief Test registry run level */
#define RCU_RUN_LEVEL_REG    1
/** @brief Test module run level */
#define RCU_RUN_LEVEL_MOD    2

/**  @brief Sets the run level */
#define RCU_SET_RUN_LEVEL(mach,level) \
    (mach)->run_level= level;

/**  @brief Gets the run level */
#define RCU_GET_RUN_LEVEL(mach) \
    ((mach)->run_level)

/**  @brief Gets the default module */
#define RCU_GET_DEF_MOD() \
    (&the_machine.def_mod)

/**  @brief Gets the default registry */
#define RCU_GET_DEF_REG() \
    (&the_machine.def_reg)

/** @brief Assertion engine runtime data structure */
typedef struct rcu_assertion_engine{
RCU_TEST_REGISTRY *curr_reg;        /**< Current test registry */
RCU_TEST_MODULE *curr_mod;          /**< Current test module */
RCU_TEST_FUNCTION *curr_func;       /**< Current test function */
RCU_GENERIC_LIST assert_list;       /**< Failure records from non-run contexts */
RCU_GENERIC_FUNCTION assert_hook;   /**< Assert callback when a failure is recorded */
}RCU_ASSERTION_ENGINE;

/** @brief Test machine run statistics */
typedef struct rcu_run_statistics {
RCU_U4 nr_fin_reg;                  /**< No. of completed test registries */
RCU_U4 nr_fail_reg;                 /**< No. of failed test registries */
RCU_U4 nr_succ_reg;                 /**< No. of successful test registries */
RCU_U4 nr_abort_reg;                /**< No. of test registries with aborted tests */
RCU_U4 nr_mod;                      /**< Total no. of test modules */
RCU_U4 nr_fin_mod;                  /**< No. of completed test modules */
RCU_U4 nr_fail_mod;                 /**< No. of failed test modules */
RCU_U4 nr_succ_mod;                 /**< No. of modules that succeed*/
RCU_U4 nr_abort_mod;                /**< No. of modules that aborted */
RCU_U4 nr_test;                     /**< Total no. of test functions */
RCU_U4 nr_fin_test;                 /**< No. of completed test functions */
RCU_U4 nr_fail_test;                /**< No. of failed test functions */
RCU_U4 nr_succ_test;                /**< No. of successful test functions */
RCU_U4 nr_abort_test;               /**< No. of aborted test functions */
RCU_U4 nr_succ_assert;              /**< No. of successful assertions */
RCU_U4 nr_fail_assert;              /**< No. of failed assertions */
RCU_U4 nr_fatal_assert;             /**< No. of fatal assertions */
}RCU_RUN_STATISTICS;

/** @brief Test run started */
#define RCU_TEST_RUN_STARTED    0

/** @brief Test run finished */
#define RCU_TEST_RUN_FINISHED   1

/** @brief Returns the run event type from the test run hook parameter */
#define RCU_GET_RUN_EVT_TYPE(param) \
    (*((RCU_INT*)param))

/** @brief Test function run context */
#define RCU_RUN_CTX_UNKNOWN         0
/** @brief Test function run context */
#define RCU_RUN_CTX_FUNC            1
/** @brief Test function init run context */
#define RCU_RUN_CTX_FUNC_INIT       2
/** @brief Test function destroy run context */
#define RCU_RUN_CTX_FUNC_DESTROY    3
/** @brief Test module init run context */
#define RCU_RUN_CTX_MOD_INIT        4
/** @brief Test module destroy run context */
#define RCU_RUN_CTX_MOD_DESTROY     5

/** Set test machine run context */
#define RCU_SET_RUN_CTX(mach,ctx)   \
    (mach)->run_ctx = (ctx);

/**  @brief  Get test machine run context */
#define RCU_GET_RUN_CTX(mach)   \
    ((mach)->run_ctx)

/**  @brief Test machine runtime data structure */
typedef struct rcu_test_machine {
RCU_GENERIC_LIST link;              /**< Unused */
RCU_TEST_MODULE def_mod;            /**< Default test module */
RCU_TEST_REGISTRY def_reg;          /**< Default tes registry */
RCU_GENERIC_LIST reg_list;          /**< List of test registries */
const RCU_CHAR *name;               /**< Test machine name */
RCU_INT init_done;                  /**< Init flag */
RCU_ASSERTION_ENGINE ae;            /**< Assertion engine */
RCU_RUN_STATISTICS run_stats;       /**< Run statistics */
RCU_INT run_level;                  /**< Test run level */
RCU_INT run_ctx;                    /**< Run context */
RCU_GENERIC_FUNCTION run_hook;      /**< Test run hook function */
RCU_U4 nr_reg;                      /**< Total no. of test registries */
RCU_OUTPUT_HANDLER def_out_hnd;     /**< Input/output device */
}RCU_TEST_MACHINE;

/** @brief Default test registry alternate reference */
#define RCU_DEFAULT_REGISTRY RCU_NULL

/** @brief Default test module alternate reference */
#define RCU_DEFAULT_MODULE RCU_NULL

/** Framework interfaces */
RCU_INT rcu_init();
RCU_INT rcu_destroy();
RCU_INT rcu_run_test_mach();
RCU_INT rcu_dump_test_dbase();
RCU_INT rcu_dump_asserts();
RCU_INT rcu_set_run_hook(RCU_GENERIC_FUNCTION run_hook);
RCU_INT rcu_set_assert_hook(RCU_GENERIC_FUNCTION assert_hook);


/** Test registry interfaces */
RCU_TEST_REGISTRY *rcu_cre_test_reg(const RCU_CHAR *name, RCU_INT enable);
RCU_INT rcu_destroy_test_reg(RCU_TEST_REGISTRY *reg);
RCU_INT rcu_add_test_reg(RCU_TEST_REGISTRY *reg);
RCU_INT rcu_run_test_reg(RCU_TEST_REGISTRY *reg);
RCU_INT rcu_run_test_reg_by_name(const RCU_CHAR *name);

/** Test module interfaces */
RCU_TEST_MODULE *rcu_cre_test_mod(const RCU_CHAR *name, RCU_GENERIC_FUNCTION init, RCU_GENERIC_FUNCTION destroy, RCU_INT enable);
RCU_INT rcu_destroy_test_mod(RCU_TEST_MODULE *mod);
RCU_INT rcu_add_test_mod(RCU_TEST_REGISTRY *reg, RCU_TEST_MODULE *mod);
RCU_INT rcu_add_test_mod_tbl(RCU_TEST_REGISTRY *reg, RCU_TEST_MODULE_ENTRY *mod_tbl);
RCU_INT rcu_run_test_mod(RCU_TEST_MODULE *mod);
RCU_INT rcu_run_test_mod_by_name(const RCU_CHAR *name);

/** Test function interfaces */
RCU_INT rcu_add_test_func(RCU_TEST_MODULE *mod, RCU_GENERIC_FUNCTION entry, RCU_GENERIC_FUNCTION init,RCU_GENERIC_FUNCTION destroy, const RCU_CHAR *name,RCU_INT enable);
RCU_INT rcu_add_test_func_tbl(RCU_TEST_MODULE *mod, RCU_TEST_FUNCTION_ENTRY *func_tbl);

/** Output handler interfaces */
RCU_INT rcu_add_out_hnd(RCU_OUTPUT_HANDLER *out_hnd);
RCU_INT rcu_del_out_hnd(RCU_OUTPUT_HANDLER *out_hnd);

/** Internally used functions */
RCU_INT rcu_assert_impl(RCU_INT cond, RCU_INT abort, const RCU_CHAR *info1, const RCU_CHAR *info2, const RCU_CHAR *fname, RCU_INT line);
RCU_INT rcu_get_timestamp(RCU_CHAR *ts_buff,const RCU_INT ts_buff_len);
RCU_INT rcu_is_mach_alive(RCU_TEST_MACHINE *mach);
RCU_INT rcu_del_all_fail_rec(RCU_TEST_MACHINE *mach);
RCU_INT rcu_del_all_fail_rec_from_func(RCU_TEST_FUNCTION *func);
RCU_INT rcu_del_all_fail_rec_from_mod(RCU_TEST_MODULE *mod);
RCU_INT rcu_print(const RCU_CHAR *str);
RCU_INT rcu_sys_info();

/** Other RCUNIT header files */
#include <rcunit_error.h>
#include <rcunit_assert.h>
#include <rcunit_log.h>
#include <rcunit_report.h>
#include <rcunit_helpers.h>
#include <rcunit_exception.h>
#include <rcunit_mem.h>
#include <rcunit_mtrace.h>
#include <rcunit_random.h>


/** External variable declarations (used internally) */
extern const RCU_CHAR *g_error_msg_tbl[];
extern RCU_CHAR g_log_buff[RCU_LOG_BUFF_SIZE];
extern RCU_INT g_ercd;
extern RCU_TEST_MACHINE the_machine;
#endif /* RCUNIT_H */



