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

#ifndef RCUNIT_LIST_H
#define RCUNIT_LIST_H

/** @brief A generic doubly linked list */
typedef struct rcu_generic_list {
struct rcu_generic_list *next;      /**< Link to the next entry in the list */
struct rcu_generic_list *prev;      /**< Link to the previous entry in the list */
}RCU_GENERIC_LIST;

/** @brief Iterates a given list */
#define RCU_FOR_EACH_ENTRY(list,cursor) \
    for((cursor)=(list)->next;(cursor)!=(list);(cursor)=(cursor)->next)


/** @brief Saves the current cursor */
#define RCU_SAVE_CURSOR(cursor)                      \
    {                                                \
    RCU_GENERIC_LIST *__saved_node = (cursor)->prev;

/** @brief Restores the current cursor */
#define RCU_RESTORE_CURSOR(cursor) \
    (cursor) = __saved_node; \
    }

/** List operations function prototypes */
 RCU_VOID rcu_init_list(RCU_GENERIC_LIST *list);
 RCU_INT rcu_is_list_empty(RCU_GENERIC_LIST *list);
 RCU_VOID rcu_insert_list(RCU_GENERIC_LIST *list,RCU_GENERIC_LIST *entry);
 RCU_VOID rcu_remove_list(RCU_GENERIC_LIST *entry);

#endif /* RCUNIT_LIST_H */


