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



