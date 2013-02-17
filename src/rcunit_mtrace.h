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

#ifndef RCUNIT_MTRACE_H
#define RCUNIT_MTRACE_H

/* The RCUNIT Memory Tracing Facility
 *  This is a simple pointer caching mechanism. Fundamentally, allocated
 *  pointers are cached (recorded) and uncached on deallocations. Any left
 *  pointers in the cache are considered leaks. Thus if a user assumes that
 *  a particular execution point all pointers are deallocated but there are
 *  left pointers in cache, then there is memory leak.
 *
 *  Below is the runtime memory layout of the pointer cache.
 *  RCU_POINTER_CACHE
 *      +-rcu_pointer_table
 *      +-rcu_pointer_table
 *      |   +-rcu_pointer_info[rcu_pointer_table_SIZE];
 *      |   |   +-pointer #0
 *      |   |   +-size    #0
 *      |   |   ...
 *      |   |   +-pointer #rcu_pointer_table_SIZE[-1]
 *      |   |   +-size    #rcu_pointer_table_SIZE[-1]
 *      +-rcu_pointer_table
 *      +-rcu_pointer_table
 *      | ... (dynamically allocated)
 *      +-rcu_pointer_table
 *
 *  The pointer cache expands the pointer table storage when needed.
 */

#define RCU_DEFAULT_MALLOC_ALIGNMENT (RCU_SIZEOF_MEMCELL)
#define rcu_pointer_table_SIZE       32

#define RCU_POINTER_CACHE_1_NAME "rcunit"
#define RCU_POINTER_CACHE_2_NAME "user"

#include "rcunit_list.h"

/* Pointer information */
typedef struct rcu_pointer_info {
    void *ptr;
    size_t size;
    int used;
    char *filename;
    char *function;
    int line_no;
} rcu_pointer_info;

/* Pointer table */
typedef struct rcu_pointer_table {
    rcu_list link; /**< Link to the next pointer table */
    rcu_pointer_info tbl[rcu_pointer_table_SIZE]; /**< the pointer table */
} rcu_pointer_table;

/* Pointer cache */
typedef struct rcu_pointer_cache {
    rcu_list ptr_tbl_list; /**< List of pointer tables */
    size_t nr_alloc; /**< Total number of allocations */
    size_t nr_free; /**< Total number of deallocations */
    size_t nr_ptr_tbl; /**< Total number of pointer tables */
    size_t nr_free_counter; /**< Number of deallocations before compaction */
    size_t max_alloc_size; /**< Maximum allocated size */
    size_t max_total_alloc_size; /**< Maximum total allocated size */
    const char *name; /**< Identifier */
} rcu_pointer_cache;

extern rcu_pointer_cache g_ptr_cache_1;
extern rcu_pointer_cache g_ptr_cache_2;

/* Traces the allocation of the given pointer and size */
#define RCU_TRACE_ALLOC(ptr,size) \
    rcu_trace_alloc_impl((ptr), (size), __FILE__,__func__, __LINE__, &g_ptr_cache_2);

/* Traces the deallocation of the given pointer */
#define RCU_TRACE_FREE(ptr) \
    rcu_trace_free_impl((ptr), __FILE__,__func__, __LINE__, &g_ptr_cache_2);

/* Checks the memory leak. This must be called in a point where the
 *  the user assumes all allocated pointers were correspondingly
 *  deallocated.
 */
#define RCU_CHECK_MEMORY_LEAK \
    rcu_check_mem_leak_impl(__FILE__, __func__, __LINE__, &g_ptr_cache_2);

/* These are used internally by RCUNIT */

/* Traces the allocation of the given pointer and size */
#define RCU_TRACE_ALLOC_INTERNAL(ptr, size) \
    rcu_trace_alloc_impl((ptr),(size), __FILE__, __func__, __LINE__, &g_ptr_cache_1);

/* Traces the deallocation of the given pointer */
#define RCU_TRACE_FREE_INTERNAL(ptr) \
    rcu_trace_free_impl((ptr), __FILE__, __func__, __LINE__, &g_ptr_cache_1);

/* Checks the memory leak. This must be called in a point where the
 *  the user assumes all allocated pointers were correspondingly
 *  deallocated.
 */
#define RCU_CHECK_MEMORY_LEAK_INTERNAL \
    rcu_check_mem_leak_impl(__FILE__,__func__, __LINE__, &g_ptr_cache_1);

/* Memory allocation tracing function prototypes */
int rcu_trace_alloc_impl(void *ptr, size_t size, const char *filename, const char *function, const int line, rcu_pointer_cache *ptr_cache);
int rcu_trace_free_impl(void *ptr, const char *filename, const char *function, const int line, rcu_pointer_cache *ptr_cache);
int rcu_check_mem_leak_impl(const char *filename, const char *function, int line, rcu_pointer_cache *ptr_cache);
int rcu_init_mtrace();
int rcu_destroy_mtrace();

void rcu_ensure_mtrace_init();
rcu_pointer_cache *rcu_get_rcunit_ptr_cache();
rcu_pointer_cache *rcu_get_user_ptr_cache();

#endif /* RCUNIT_MTRACE_H */

