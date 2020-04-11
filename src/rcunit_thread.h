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

typedef struct {
    rcu_list link;
    const char *name;
    pthread_t id;
    pthread_attr_t attr;
    int running;
} rcu_thread;

RCU_API rcu_thread *rcu_get_thread(const char *name, rcu_thread_routine run, void *arg);

RCU_API void rcu_join_thread(rcu_thread *thread);

#endif /* RCUNIT_THREAD_H **/****

