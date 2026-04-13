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

#ifndef RCUNIT_LIST_H
#define RCUNIT_LIST_H

#include <stdio.h>

/* A doubly linked list */
typedef struct rcu_list {
    struct rcu_list *next;
    struct rcu_list *prev;
} rcu_list;

/* Iterates a given list */
#define RCU_FOR_EACH_ENTRY(list, cursor) \
    for((cursor) = (list)->next; (cursor) != (list); (cursor) = (cursor)->next)

#define RCU_FOR_EACH_ENTRY_WITH_CURSOR(list, cursor) \
    rcu_list *(cursor); \
    for((cursor) = (list)->next; (cursor) != (list); (cursor) = (cursor)->next)

#define RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(list, cursor, index) \
    rcu_list *(cursor); \
    int (index) = 0; \
    for((cursor) = (list)->next; (cursor) != (list); (cursor) = (cursor)->next, (index)++)

/* Saves the current cursor */
#define RCU_SAVE_CURSOR(cursor)                      \
    {                                                \
    rcu_list *__saved_node__ = (cursor)->prev;

/* Restores the current cursor */
#define RCU_RESTORE_CURSOR(cursor) \
    (cursor) = __saved_node__; \
    }

/** List operations function prototypes */
void rcu_init_list(rcu_list *list);

int rcu_is_list_empty(rcu_list *list);

void rcu_insert_list(rcu_list *list, rcu_list *entry);

void rcu_remove_list(rcu_list *entry);

int rcu_get_list_size(rcu_list *entry);

#endif /* RCUNIT_LIST_H */


