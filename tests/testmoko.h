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

#ifndef TESTMOKO_H
#define TESTMOKO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>

#include "testmoko_exception.h"

#define TMK_API
#define TMK_NULLABLE

#define TEST_RESULT_SUCCESSFUL 0
#define TEST_RESULT_FAILED 1

typedef void(*tmk_test_function)();

typedef struct tmk_test_function_entry {
    const char *name;
    tmk_test_function test;
    tmk_test_function setup;
    tmk_test_function teardown;
    int result;
} tmk_test_function_entry;

#define TMK_TEST(name) \
    void name(void)

#define TMK_TEST_FUNCTION_TABLE_START(name) \
    tmk_test_function_entry name[]={

#define TMK_INCLUDE_TEST(func) \
    {#func, func, NULL, NULL, 1},

#define TMK_INCLUDE_TEST_ST(func, setup, teardown) \
    {#func, func, setup, teardown},

#define TMK_TEST_FUNCTION_TABLE_END \
     { NULL, NULL }                  \
    };

#define TMK_EXTERN_TEST_FUNCTION_TABLE(name) \
   extern tmk_test_function_entry name[];

/* Warning : when defining custom assertion, make sure, you do not
 * evaluate the condition more than once  (e.g. passing it as parameter in 
 * varargs) as the condition itself can be a function call. A solution to
 * to this is to assign/evaluate it to a local variable (block scope) 
 */

/* Asserts a condition */
#define TMK_ASSERT(cond) \
 { tmk_assert_impl((cond), __FILE__, __func__, __LINE__, "%s", #cond); }

/* Asserts a false condition */
#define TMK_ASSERT_TRUE(cond) \
 { tmk_assert_impl((cond), __FILE__,__func__, __LINE__, "%s is not true", #cond); }

/* Asserts a false condition */
#define TMK_ASSERT_FALSE(cond) \
 { tmk_assert_impl(!(cond), __FILE__,__func__,__LINE__, "%s is true", #cond); }

/* Asserts a null pointer */
#define TMK_ASSERT_NULL(ptr) \
 { tmk_assert_impl(((ptr) == NULL), __FILE__,__func__,__LINE__, "null"); }

/* Asserts a non-null pointer */
#define TMK_ASSERT_NOT_NULL(ptr) \
 { tmk_assert_impl(((ptr) != NULL), __FILE__,__func__,__LINE__,"pointer is not null"); }

/* Asserts that the two variables are equal */
#define TMK_ASSERT_EQUAL(expected, actual) \
    { long __expected = (long)expected; long __actual = (long)actual; \
      tmk_assert_impl((__expected == __actual), __FILE__, __func__, __LINE__,  \
      "%s expected to be %d but got %d", #actual,__expected, __actual); \
    }

/* Asserts that the two floating points are equal */
#define TMK_ASSERT_EQUAL_FLOATS(expected, actual) \
    { double __expected = (double)expected; double __actual = (double)actual; \
      tmk_assert_impl((__expected == __actual), __FILE__, __func__, __LINE__,  \
      "%s expected to be %lf but got %lf", #actual,__expected, __actual); \
    }

/* Asserts that the two pointers are equal */
#define TMK_ASSERT_EQUAL_PTRS(expected, actual) \
    { void *__expected = (void*)expected; void *__actual = (void*)actual; \
      tmk_assert_impl((__expected == __actual), __FILE__, __func__, __LINE__,  \
      "%s expected to be %p but got %", #actual,__expected, __actual); \
    }

/* Asserts that the two strings are equal */
#define TMK_ASSERT_EQUAL_STRING(expected, actual) \
    { const char *__expected = expected; const char *__actual = actual; \
     tmk_assert_impl( !(strcmp((__expected), (__actual))), __FILE__, __func__, __LINE__,  \
             "expected \"%s\" but got \"%s\"", __expected, __actual); }

/* Records an explicitly failed condition described by msg */
#define TMK_FAIL(msg) \
 { tmk_assert_impl(0, __FILE__, __func__, __LINE__, "Failed explicitly : %s", msg); }

void tmk_log_impl(const char *filename, const int line_no,
                  const char *format, ...);

#define TMK_LOG(format...) \
    tmk_log_impl(__FILE__, __LINE__, format);


#define TMK_PRINT_THIS_LINE \
    fprintf(stdout,">> %s %s %d\n", __FILE__,__func__,__LINE__);

extern TMK_API int tmk_run_tests(tmk_test_function_entry *tbl, TMK_NULLABLE void (*setup)(), void (*teardown)());

extern void tmk_assert_impl(int cond, const char *filename, const char *func_name, int line, const char *format, ...);


#endif /* TESTMOKO_H */

