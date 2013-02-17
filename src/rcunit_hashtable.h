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

#ifndef RCUNIT_HASHTABLE_H
#define RCUNIT_HASHTABLE_H

#include <stdio.h>
#include "rcunit_types.h"
#include "rcunit_list.h"

/*  RCUNIT hash table implementation
 *  This is a simple hash table implementation. A commonly used integer and
 *  string hashing functions are provided.
 */

/* A generic hash table */
typedef struct rcu_hashtable {
    int nr_bucket;
    int nr_elem;
    rcu_list bucket[1];
} rcu_hashtable;

/* Calculates the number of memory cells required to store nbytes */
#define RCU_SIZEIN_MEMCELL(nbytes) (((nbytes)+3) >> 2)

/* Calculates the number of RCU_MEMCELLs required to create a hashtable of
 *  nr_bucket buckets.
 */
#define RCU_SIZEOF_HASHTABLE(nr_bucket) \
    (RCU_SIZEIN_MEMCELL(sizeof(rcu_hashtable)) + RCU_SIZEIN_MEMCELL(sizeof(rcu_list))*((nr_bucket)-1))

#endif /* RCUNIT_HASHTABLE_H */
