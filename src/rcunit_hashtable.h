/*
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

#ifndef RCUNIT_HASHTABLE_H
#define RCUNIT_HASHTABLE_H

/*  RCUNIT Hashtable Implementation
 *  This is a simple hash table implementation. A commonly used integer and
 *  string hashing functions are provided.
 */

/** @brief A generic hashtable */
typedef struct rcu_hashtable {
RCU_U4 nr_bucket;                   /**< number of buckets */
RCU_U4 nr_elem;                     /**< total number of elements */
RCU_GENERIC_LIST bucket[1];         /**< buckets  */
}RCU_HASHTABLE;

/** @brief Calculates the number of RCU_MEMCELLs required to create a hashtable of
 *  nr_bucket buckets.
 */
#define RCU_SIZEOF_HASHTABLE(nr_bucket) \
    (RCU_SIZEIN_MEMCELL(sizeof(RCU_HASHTABLE)) + RCU_SIZEIN_MEMCELL(sizeof(RCU_GENERIC_LIST))*((nr_bucket)-1))

#endif /* RCUNIT_HASHTABLE_H */
