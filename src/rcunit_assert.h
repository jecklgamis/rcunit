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

#ifndef RCUNIT_ASSERT_H
#define RCUNIT_ASSERT_H

#include "rcunit.h"


extern void rcu_assert_impl(int cond, const char *filename, const char *func_name, int line,
                            const char *format, ...);

/* Asserts a true condition */
#define RCU_ASSERT(cond) \
 { rcu_assert_impl((cond), __FILE__,__func__, __LINE__, "%s", #cond); }

/* Asserts a false condition */
#define RCU_ASSERT_TRUE(cond) \
 { rcu_assert_impl((cond), __FILE__,__func__,__LINE__, "%s is false but expected to be true", #cond); }

/* Asserts a false condition */
#define RCU_ASSERT_FALSE(cond) \
 { rcu_assert_impl(!(cond), __FILE__,__func__,__LINE__, "%s is true but expected to be false", #cond); }

/* Asserts a null pointer */
#define RCU_ASSERT_NULL(ptr) \
 { rcu_assert_impl(((ptr) == NULL), __FILE__,__func__,__LINE__, "expecting null pointer"); }

/* Asserts a non-null pointer */
#define RCU_ASSERT_NOT_NULL(ptr) \
 { rcu_assert_impl(((ptr) != NULL), __FILE__,__func__,__LINE__,"expecting non-null pointer"); }

/* Asserts that the two values/variables are equal (integer types) */
#define RCU_ASSERT_EQUAL(expected, actual) \
    { long __expected = expected; long __actual = actual; \
     rcu_assert_impl((__expected == __actual), __FILE__,__func__, __LINE__,"%s expected to be %s but got %ld", #actual, #expected, (long)__actual);  \
    }

/* Asserts that the two values/variables are not equal (integer types) */
#define RCU_ASSERT_NOT_EQUAL(expected, actual) \
    { long __expected = expected; long __actual = actual; \
     rcu_assert_impl((__expected != __actual), __FILE__,__func__, __LINE__,"%s not expected to be %s but got %ld", #actual, #expected, (long)__actual);  \
    }

#define RCU_ASSERT_EQUAL_FLOATS(expected, actual) \
    { double __expected = expected; double __actual = actual; \
     rcu_assert_impl((__expected == __actual), __FILE__,__func__, __LINE__,"%s expected to be %s but got %lf", #actual, #expected, (double)__actual);  \
    }

#define RCU_ASSERT_NOT_EQUAL_FLOATS(expected, actual) \
    { double __expected = expected; double __actual = actual; \
     rcu_assert_impl((__expected != __actual), __FILE__,__func__, __LINE__,"%s not expected to be %s but got %lf", #actual, #expected, (double)__actual);  \
    }

#define RCU_ASSERT_EQUAL_PTRS(expected, actual) \
    { void *__expected = expected; void *__actual = actual; \
     rcu_assert_impl((__expected == __actual), __FILE__,__func__, __LINE__,"%s expected to be %s but got %p", #actual, #expected, (void*)__actual);  \
    }

/* Asserts that the two strings are equal */
#define RCU_ASSERT_EQUAL_STRING(expected, actual) \
    { const char *__expected = expected; const char *__actual = actual; \
    rcu_assert_impl( !(strcmp((__expected), (__actual))), __FILE__,__func__, __LINE__, "%s expected to be \"%s\" but got \"%s\"", #actual, #expected,__actual); \
    }

/* Records an explicitly failed condition described by msg */
#define RCU_FAIL(msg) \
 { rcu_assert_impl(RCU_FALSE, __FILE__,__func__, __LINE__, "failed explicitly : \"%s\"", msg); }

/* Asserts that the two byte arrays are equal */
#define RCU_ASSERT_SAME_BYTE_ARRAY(array1, array2, size) \
    { rcu_assert_impl( !(memcmp(array1, array2, size)), __FILE__,__func__, __LINE__, "arrays are not the same"); }

#define RCU_ASSERT_NOT_SAME_BYTE_ARRAY(array1, array2, size) \
    { rcu_assert_impl( (memcmp(array1, array2, size)), __FILE__,__func__, __LINE__, "byte arrays are the same"); }

/** Asserts that a bit is set at the given bit position (zero-based) */
#define RCU_ASSERT_BIT_SET(data, bit_pos) \
 { int __data = data; int __bit_pos = bit_pos; rcu_assert_impl(__data & (1 << __bit_pos), __FILE__,__func__,__LINE__,"bit %d is not set in %lu",__bit_pos, (rcu_u4) __data); }

/** Asserts that a bit is set at the given bit position (zero-based) */
#define RCU_ASSERT_BIT_NOT_SET(data, bit_pos) \
 { int __data = data; int __bit_pos = bit_pos; rcu_assert_impl((data & (1 << bit_pos)) == 0,__FILE__,__func__,__LINE__,"bit %d is set in %lu",__bit_pos, (rcu_u4) __data); }


#endif /* RCUNIT_ASSERT_H */

