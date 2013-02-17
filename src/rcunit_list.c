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


#include "rcunit_list.h"

void rcu_init_list(rcu_list *list) {
    if (list != NULL) {
        list->prev = list;
        list->next = list;
    }
}

int rcu_is_list_empty(rcu_list *list) {
    int result = 0;
    if (list != NULL) {
        result = (list->next == list);
    }
    return result;
}

void rcu_insert_list(rcu_list *list, rcu_list *entry) {
    if (list != NULL && entry != NULL) {
        entry->prev = list->prev;
        entry->next = list;
        list->prev->next = entry;
        list->prev = entry;
    }
}

void rcu_remove_list(rcu_list *entry) {
    if (entry != NULL) {
        if (entry->next != entry) {
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev;
        }
    }
}

int rcu_get_list_size(rcu_list *list) {
    int size = 0L;
    rcu_list *cursor = NULL;
    if (list != NULL) {
        if (list != NULL) {

            RCU_FOR_EACH_ENTRY(list, cursor) {
                size++;
            }
        }
    }
    return size;
}
