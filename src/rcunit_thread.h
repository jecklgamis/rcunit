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

#ifndef RCUNIT_THREAD_H
#define RCUNIT_THREAD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "rcunit_types.h"
#include "rcunit_list.h"
#include "rcunit_mem.h"

#define RCU_DEBUG_THREADS 0

typedef void (*rcu_thread_routine)(void *);

struct rcu_thread {
    struct rcu_list link;
    const char *name;
    pthread_t id;
    pthread_attr_t attr;
    int running;
};

struct rcu_thread *rcu_get_thread(const char *name, rcu_thread_routine run, void *arg);

void rcu_join_thread(struct rcu_thread *thread);

#endif /* RCUNIT_THREAD_H **/

