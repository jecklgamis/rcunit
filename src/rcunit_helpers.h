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

#ifndef RCUNIT_HELPERS_H
#define RCUNIT_HELPERS_H

#include "logmoko.h"
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

#define RCU_LOG_TRACE(format...) LMK_LOG_TRACE(the_rcu_logger, format);
#define RCU_LOG_DEBUG(format...) LMK_LOG_DEBUG(the_rcu_logger, format);
#define RCU_LOG_INFO(format...) LMK_LOG_INFO(the_rcu_logger, format);
#define RCU_LOG_WARN(format...) LMK_LOG_WARN(the_rcu_logger, format);
#define RCU_LOG_ERROR(format...) LMK_LOG_ERROR(the_rcu_logger, format);
#define RCU_LOG_FATAL(format...) LMK_LOG_FATAL(the_rcu_logger, format);

#define RCU_DEC_TEST_FUNC(func,param) \
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


