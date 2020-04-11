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

#ifndef RCUNIT_QUEUE_H
#define RCUNIT_QUEUE_H

#include <stdio.h>
#include "rcunit_types.h"
#include "rcunit_list.h"

typedef rcu_list rcu_queue;

void rcu_init_queue(rcu_queue *queue);

void rcu_destroy_queue(rcu_queue *queue);

void rcu_insert_queue(rcu_queue *queue, rcu_queue *element);

rcu_queue *rcu_remove_queue(rcu_queue *queue);

int rcu_get_queue_size(rcu_queue *entry);

int rcu_is_queue_empty(rcu_queue *list);

#endif /* RCUNIT_QUEUE_H */


