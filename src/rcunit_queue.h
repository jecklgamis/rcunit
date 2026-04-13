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


