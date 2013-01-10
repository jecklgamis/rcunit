/*
 * RCUNIT - A unit testing framework for C.
 * Copyright (C) 2006 Jerrico L. Gamis
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 *
 */

#include <rcunit.h>

/**
 *  @brief Initializes the given list or list entry
 *  @param[in] list  List or list entry
 */

RCU_VOID rcu_init_list(RCU_GENERIC_LIST *list ){
    list->prev=list;
    list->next=list;
}

/**
 *  @brief Returns true if the given list is empty
 *  @param[in] list List
 *  @return Boolean value (RCU_TRUE, RCU_FALSE)
 */

RCU_INT rcu_is_list_empty(RCU_GENERIC_LIST *list){
    return((list->next==list)?RCU_TRUE:RCU_FALSE);
}

/**
 *  @brief Inserts a entry into a list
 *  @param[in] list List
 *  @param[in] entry List entry
 */

RCU_VOID rcu_insert_list(RCU_GENERIC_LIST *list,RCU_GENERIC_LIST *entry){
    entry->prev=list->prev;
    entry->next=list;
    list->prev->next=entry;
    list->prev=entry;
}

/**
 *  @brief Removes an entry from a list
 *  @param[in] entry List entry
 */

RCU_VOID rcu_remove_list(RCU_GENERIC_LIST *entry){
    if (entry->next!=entry ) {
        entry->prev->next=entry->next;
        entry->next->prev=entry->prev;
    }
}
