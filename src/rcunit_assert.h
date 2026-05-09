/*
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
 { rcu_assert_impl(!((ptr)), __FILE__,__func__,__LINE__, "expecting null pointer"); }

/* Asserts a non-null pointer */
#define RCU_ASSERT_NOT_NULL(ptr) \
 { rcu_assert_impl(!!(ptr), __FILE__,__func__,__LINE__,"expecting non-null pointer"); }

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

/* Asserts that all bits in mask are set */
#define RCU_ASSERT_BITS_SET(data, mask) \
 { long __data = (long)(data); long __mask = (long)(mask); \
   rcu_assert_impl((__data & __mask) == __mask, __FILE__,__func__,__LINE__, \
   "not all bits of mask 0x%lx are set in 0x%lx", __mask, __data); }

/* Asserts that none of the bits in mask are set */
#define RCU_ASSERT_BITS_NOT_SET(data, mask) \
 { long __data = (long)(data); long __mask = (long)(mask); \
   rcu_assert_impl((__data & __mask) == 0, __FILE__,__func__,__LINE__, \
   "some bits of mask 0x%lx are set in 0x%lx", __mask, __data); }

/* Asserts that a > b (integer types) */
#define RCU_ASSERT_GREATER_THAN(a, b) \
 { long __a = (long)(a); long __b = (long)(b); \
   rcu_assert_impl(__a > __b, __FILE__,__func__,__LINE__, \
   "%s (%ld) is not greater than %s (%ld)", #a, __a, #b, __b); }

/* Asserts that a < b (integer types) */
#define RCU_ASSERT_LESS_THAN(a, b) \
 { long __a = (long)(a); long __b = (long)(b); \
   rcu_assert_impl(__a < __b, __FILE__,__func__,__LINE__, \
   "%s (%ld) is not less than %s (%ld)", #a, __a, #b, __b); }

/* Asserts that a >= b (integer types) */
#define RCU_ASSERT_GREATER_EQUAL(a, b) \
 { long __a = (long)(a); long __b = (long)(b); \
   rcu_assert_impl(__a >= __b, __FILE__,__func__,__LINE__, \
   "%s (%ld) is not greater than or equal to %s (%ld)", #a, __a, #b, __b); }

/* Asserts that a <= b (integer types) */
#define RCU_ASSERT_LESS_EQUAL(a, b) \
 { long __a = (long)(a); long __b = (long)(b); \
   rcu_assert_impl(__a <= __b, __FILE__,__func__,__LINE__, \
   "%s (%ld) is not less than or equal to %s (%ld)", #a, __a, #b, __b); }

/* Asserts that min <= val <= max (integer types) */
#define RCU_ASSERT_IN_RANGE(val, min, max) \
 { long __val = (long)(val); long __min = (long)(min); long __max = (long)(max); \
   rcu_assert_impl(__val >= __min && __val <= __max, __FILE__,__func__,__LINE__, \
   "%s (%ld) is not in range [%ld, %ld]", #val, __val, __min, __max); }

/* Asserts that |expected - actual| <= delta (floating point) */
#define RCU_ASSERT_NEAR(expected, actual, delta) \
 { double __expected = (double)(expected); double __actual = (double)(actual); \
   double __delta = (double)(delta); \
   double __diff = __expected - __actual; \
   double __abs_diff = __diff < 0 ? -__diff : __diff; \
   rcu_assert_impl(__abs_diff <= __delta, __FILE__,__func__,__LINE__, \
   "%s expected to be near %lf but got %lf (diff %lf > delta %lf)", \
   #actual, __expected, __actual, __abs_diff, __delta); }

/* Asserts that two strings are not equal */
#define RCU_ASSERT_NOT_EQUAL_STRING(expected, actual) \
 { const char *__expected = (expected); const char *__actual = (actual); \
   rcu_assert_impl(strcmp(__expected, __actual) != 0, __FILE__,__func__,__LINE__, \
   "%s expected to differ from \"%s\" but they are equal", #actual, __expected); }

/* Asserts that haystack contains needle */
#define RCU_ASSERT_STRING_CONTAINS(haystack, needle) \
 { const char *__haystack = (haystack); const char *__needle = (needle); \
   rcu_assert_impl(strstr(__haystack, __needle) != NULL, __FILE__,__func__,__LINE__, \
   "\"%s\" does not contain \"%s\"", __haystack, __needle); }

/* Asserts that a string is empty */
#define RCU_ASSERT_STRING_EMPTY(str) \
 { const char *__str = (str); \
   rcu_assert_impl(__str != NULL && __str[0] == '\0', __FILE__,__func__,__LINE__, \
   "%s is not empty", #str); }

/* Asserts that a string is not empty */
#define RCU_ASSERT_STRING_NOT_EMPTY(str) \
 { const char *__str = (str); \
   rcu_assert_impl(__str != NULL && __str[0] != '\0', __FILE__,__func__,__LINE__, \
   "%s is empty", #str); }

/* Asserts that two pointers are not equal */
#define RCU_ASSERT_NOT_EQUAL_PTRS(expected, actual) \
 { void *__expected = (void *)(expected); void *__actual = (void *)(actual); \
   rcu_assert_impl(__expected != __actual, __FILE__,__func__,__LINE__, \
   "%s expected to differ from %p but they are equal", #actual, __expected); }

/* Asserts that a memory region is all zeros */
#define RCU_ASSERT_ZERO_MEMORY(ptr, size) \
 { size_t __size = (size); const unsigned char *__p = (const unsigned char *)(ptr); \
   int __ok = 1; size_t __i; \
   for (__i = 0; __i < __size; __i++) { if (__p[__i] != 0) { __ok = 0; break; } } \
   rcu_assert_impl(__ok, __FILE__,__func__,__LINE__, \
   "%s has non-zero byte at offset %zu", #ptr, __i); }

/* Asserts that two int arrays are element-wise equal */
#define RCU_ASSERT_ARRAY_EQUAL(expected, actual, n) \
 { size_t __n = (n); const long *__expected = (const long *)(expected); \
   const long *__actual = (const long *)(actual); \
   int __ok = 1; size_t __i; \
   for (__i = 0; __i < __n; __i++) { \
       if (__expected[__i] != __actual[__i]) { __ok = 0; break; } \
   } \
   rcu_assert_impl(__ok, __FILE__,__func__,__LINE__, \
   "%s differs from %s at index %zu (expected %ld, got %ld)", \
   #actual, #expected, __i, __expected[__i], __actual[__i]); }

#endif /* RCUNIT_ASSERT_H */

