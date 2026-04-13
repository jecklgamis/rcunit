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
