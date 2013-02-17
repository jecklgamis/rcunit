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


