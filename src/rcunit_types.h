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

#ifndef RCUNIT_TYPES_H
#define RCUNIT_TYPES_H

/* Tags a deprecated function */
#define RCU_DEPRECATED

/* Tags an API function */
#define RCU_API

/* "True" definition */
#define RCU_TRUE 1

/* "False" definition */
#define RCU_FALSE (!RCU_TRUE)

/* 1-byte unsigned data type */
typedef unsigned char rcu_u1;

/* 2-byte unsigned data type */
typedef unsigned short rcu_u2;

/* 4-byte unsigned data type */
typedef unsigned long rcu_u4;

/* Signed 1-byte data type */
typedef char rcu_char;

/* Signed 2-byte data type */
typedef short rcu_short;

/* Signed 4 or 8 bytes data type */
typedef int rcu_int;

/* Signed 4 or 8 bytes data type */
typedef long rcu_long;

/* Floating point data type (32-bit) */
typedef float rcu_float;

/* Floating point data type (64-bit)  */
typedef double rcu_double;

/* Long long data type */
typedef long long rcu_long_long;

/* Long double data type */
typedef long double rcu_long_double;

/* The memory cell */
typedef long rcu_memcell;

/* The common function format */
typedef void(*rcu_generic_function)(void *);

#endif /* RCUNIT_TYPES_H */



