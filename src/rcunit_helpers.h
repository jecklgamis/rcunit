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

#ifndef RCUNIT_HELPERS_H
#define RCUNIT_HELPERS_H

#include <stdio.h>
#include "rcunit.h"

/* Calculates the n-byte aligned value */
#define RCU_ALIGN_N(n, val) \
    ((val + (n)-1) & ~((n)-1))

/* Calculates the number of n-bytes required to store value */
#define RCU_SIZE_IN_N(n, val) (((val)+(((n))-1)) >> 2)

/* Size in bytes of a memory cell (e.g. 32 bits in a 32-bit platform) */
#define RCU_SIZEOF_MEMCELL (sizeof(int))

/* Calculates the number of memory cells required to store nbytes */
#define RCU_SIZEIN_MEMCELL(nbytes) (((nbytes)+3) >> 2)

/* Increments an integer value */
#define RCU_INCR(__value__) \
    (++__value__);

/* Decrements an integer value */
#define RCU_DECR(__value__) \
    (--__value__);

/* Increments an integer value by a specified amount */
#define RCU_INCR_BY(__value__, __inc__) \
    __value__ +=__inc__;

/* Decrements an integer value by a specified amount */
#define RCU_DECR_BY(__value__, __dec__) \
    __value__ -=__dec__;

/* Resets a counter */
#define RCU_RESET(__value__) \
    __value__ = 0;

/* Traverses a test function table */
#define RCU_FOR_EACH_FUNC_ENTRY(func_tbl, cursor, index) \
    for(index = 0, cursor = func_tbl; cursor->name != NULL; cursor++, index++)

/* Traverses a test module table */
#define RCU_FOR_EACH_MOD_ENTRY(mod_tbl, cursor, index) \
    for(index = 0,cursor = mod_tbl; cursor->name != NULL; cursor++, index++)

/* Declares a log handler implementation */
#define RCU_DEC_LOG_HANDLER_IMPL(log_impl, param) \
    extern void log_impl(void *param);

/* Declares an output handler implementation */
#define RCU_DEC_OUT_HANDLER_IMPL(out_impl, param) \
    extern void out_impl(void *param);

#define RCU_LOG_TRACE(fmt, ...) fprintf(stdout, "[TRACE] " fmt "\n", ##__VA_ARGS__)
#define RCU_LOG_DEBUG(fmt, ...) fprintf(stdout, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#define RCU_LOG_INFO(fmt, ...)  fprintf(stdout, "[INFO]  " fmt "\n", ##__VA_ARGS__)
#define RCU_LOG_WARN(fmt, ...)  fprintf(stdout, "[WARN]  " fmt "\n", ##__VA_ARGS__)
#define RCU_LOG_ERROR(fmt, ...) fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)
#define RCU_LOG_FATAL(fmt, ...) fprintf(stderr, "[FATAL] " fmt "\n", ##__VA_ARGS__)

#define RCU_DEC_TEST_FUNC(func, param) \
    extern void func(void* param);

/* Declares a test function table */
#define RCU_DEC_TEST_FUNC_TBL(func_tbl) \
    extern rcu_test_function_entry func_tbl[];

/* Declares a test module */
#define RCU_DEC_TEST_MOD(mod) \
    extern rcu_module mod;

/* Declares a test module table */
#define RCU_DEC_TEST_MOD_TBL(mod) \
    extern rcu_module_entry mod[];

#define RCU_RET_IF_NULL(ptr) \
    if ((ptr) == NULL) return;

#define RCU_RET_VALUE_IF_NULL(ptr, value) \
    if ((ptr) == NULL) return value;

#endif /* RCUNIT_HELPERS_H */


