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

#include "rcunit_thread.h"

rcu_list g_rcu_thread_list;
static int g_rcu_threads_initialized;

RCU_API void rcu_init_threads() {
    if (!g_rcu_threads_initialized) {
        rcu_init_list(&g_rcu_thread_list);
        g_rcu_threads_initialized = 1;
    }
}

RCU_API void rcu_destroy_threads() {
    if (g_rcu_threads_initialized) {
        g_rcu_threads_initialized = 0;
    }
}

void rcu_init_thread(rcu_thread *thread) {
    if (thread != NULL) {
        rcu_init_list(&thread->link);
    }
}

void rcu_destroy_thread(rcu_thread **thread_addr) {
    rcu_init_threads();
    if (thread_addr != NULL && *thread_addr != NULL) {
        rcu_native_free(*thread_addr);
    }
    *thread_addr = NULL;
}

rcu_thread *rcu_srch_thread_by_name(const char *name) {
    rcu_list *cursor = NULL;

    RCU_FOR_EACH_ENTRY(&g_rcu_thread_list, cursor) {
        rcu_thread *thread = (rcu_thread *) cursor;
        if (!strcmp(thread->name, name)) {
            return thread;
        }
    }
    return NULL;
}

RCU_API rcu_thread *rcu_get_thread(const char *name, rcu_thread_routine routine, void *arg) {
    rcu_thread *thread = NULL;
    rcu_init_threads();
    if (name == NULL) {
        return NULL;
    }
    if ((thread = rcu_srch_thread_by_name(name)) == NULL) {
        if ((thread = rcu_native_malloc(sizeof(rcu_thread))) != NULL) {
            rcu_init_thread(thread);
            rcu_insert_list(&g_rcu_thread_list, &thread->link);
            thread->name = name;
            pthread_attr_init(&thread->attr);
            if (!pthread_create(&thread->id, &thread->attr, (void *) routine, (void *) arg)) {
#if RCU_DEBUG_THREADS
                fprintf(stdout, "Thread %s created  (id = %d, routine = %p)\n", thread->name, (int) thread->id, (void*) routine);
#endif
            }
        }
    }
    return thread;
}

void rcu_start_thread(rcu_thread *thread) {
}

void rcu_stop_thread(rcu_thread *thread) {
    if (thread != NULL && rcu_srch_thread_by_name(thread->name) != NULL) {
        pthread_join(thread->id, (void **) NULL);
    }
}

