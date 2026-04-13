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
