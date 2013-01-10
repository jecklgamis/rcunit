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

#ifndef RCUNIT_MTRACE_H
#define RCUNIT_MTRACE_H

/*  The RCUNIT Memory Tracing Facility
 *  This is a simple pointer caching mechanism. Fundamentally, allocated
 *  pointers are cached (recorded) and uncached on deallocations. Any left
 *  pointers in the cache are considered leaks. Thus if a user assumes that
 *  a particular execution point all pointers are deallocated but there are
 *  left pointers in cache, then there is memory leak.
 *
 *  Below is the runtime memory layout of the pointer cache.
 *  RCU_POINTER_CACHE
 *      +-RCU_POINTER_TABLE
 *      +-RCU_POINTER_TABLE
 *      |   +-RCU_POINTER_INFO[RCU_POINTER_TABLE_SIZE];
 *      |   |   +-pointer #0
 *      |   |   +-size    #0
 *      |   |   ...
 *      |   |   +-pointer #RCU_POINTER_TABLE_SIZE[-1]
 *      |   |   +-size    #RCU_POINTER_TABLE_SIZE[-1]
 *      +-RCU_POINTER_TABLE
 *      +-RCU_POINTER_TABLE
 *      | ... (dynamically allocated)
 *      +-RCU_POINTER_TABLE
 *
 *  The pointer cache expands the pointer table storage when needed.
 */

#define RCU_DEFAULT_MALLOC_ALIGNMENT (RCU_SIZEOF_MEMCELL)
#define RCU_POINTER_TABLE_SIZE       32

#define RCU_POINTER_CACHE_1_NAME "rcunit"
#define RCU_POINTER_CACHE_2_NAME "user"

/** @brief Pointer information */
typedef struct rcu_pointer_info {
RCU_VOID *ptr;                      /**< Allocated pointer */
RCU_U4 size;                        /**< Allocated size */
RCU_INT used;                       /**< "Used" flag */
}RCU_POINTER_INFO;

/** @brief Pointer table */
typedef struct rcu_pointer_table {
RCU_GENERIC_LIST link;              /**< Link to the next pointer table */
RCU_POINTER_INFO tbl[RCU_POINTER_TABLE_SIZE];  /**< the pointer table */
}RCU_POINTER_TABLE;

/** @brief Pointer cache */
typedef struct rcu_pointer_cache{
RCU_GENERIC_LIST ptr_tbl_list;   /**< List of pointer tables */
RCU_U4 nr_alloc;                    /**< Total number of allocations */
RCU_U4 nr_free;                     /**< Total number of deallocations */
RCU_U4 nr_ptr_tbl;                  /**< Total number of pointer tables */
RCU_U4 nr_leak;                     /**< Number of memory leaks found */
RCU_U4 leak_size;                   /**< Total memory size of the memory leaks */
RCU_U4 nr_free_counter;             /**< Number of deallocations before compaction */
RCU_U4 max_alloc_size;              /**< Maximum allocated size */
RCU_U4 max_total_alloc_size;        /**< Maximum total allocated size */
const RCU_CHAR *name;                       /**< Identifier */
}RCU_POINTER_CACHE;

extern RCU_POINTER_CACHE g_ptr_cache_1;
extern RCU_POINTER_CACHE g_ptr_cache_2;

/** @brief Traces the allocation of the given pointer and size */
#define RCU_TRACE_ALLOC(ptr,size) \
    rcu_trace_alloc_impl((ptr),(size),__FILE__,__LINE__,&g_ptr_cache_2);

/** @brief Traces the deallocation of the given pointer */
#define RCU_TRACE_FREE(ptr) \
    rcu_trace_free_impl((ptr),__FILE__,__LINE__,&g_ptr_cache_2);


/** @brief Checks the memory leak. This must be called in a point where the
 *  the user assumes all allocated pointers were correspondingly
 *  deallocated.
 */
#define RCU_CHECK_MEMORY_LEAK() \
    rcu_check_mem_leak_impl(__FILE__,__LINE__,&g_ptr_cache_2);

/* These are used internally by RCUNIT */

/** @brief Traces the allocation of the given pointer and size */
#define RCU_TRACE_ALLOC_INTERNAL(ptr,size) \
    rcu_trace_alloc_impl((ptr),(size),__FILE__,__LINE__,&g_ptr_cache_1);

/** @brief Traces the deallocation of the given pointer */
#define RCU_TRACE_FREE_INTERNAL(ptr) \
    rcu_trace_free_impl((ptr),__FILE__,__LINE__,&g_ptr_cache_1);

/** @brief Checks the memory leak. This must be called in a point where the
 *  the user assumes all allocated pointers were correspondingly
 *  deallocated.
 */
#define RCU_CHECK_MEMORY_LEAK_INTERNAL() \
    rcu_check_mem_leak_impl(__FILE__,__LINE__,&g_ptr_cache_1);


/** Memory tracing function prototypes */
RCU_INT rcu_trace_alloc_impl(RCU_VOID *ptr,RCU_U4 size, const RCU_CHAR*fname,const RCU_INT line,RCU_POINTER_CACHE *ptr_cache);
RCU_INT rcu_trace_free_impl(RCU_VOID *ptr,const RCU_CHAR *fname,const RCU_INT line,RCU_POINTER_CACHE *ptr_cache);
RCU_INT rcu_check_mem_leak_impl(const RCU_CHAR *fname,const RCU_INT line,RCU_POINTER_CACHE *ptr_cache);
RCU_INT rcu_init_mtrace();
RCU_INT rcu_destroy_mtrace();

#endif /* RCUNIT_MTRACE_H */



