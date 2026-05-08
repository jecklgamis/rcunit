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
 *      +-struct rcu_pointer_table
 *      +-struct rcu_pointer_table
 *      |   +-struct rcu_pointer_info[rcu_pointer_table_SIZE];
 *      |   |   +-pointer #0
 *      |   |   +-size    #0
 *      |   |   ...
 *      |   |   +-pointer #rcu_pointer_table_SIZE[-1]
 *      |   |   +-size    #rcu_pointer_table_SIZE[-1]
 *      +-struct rcu_pointer_table
 *      +-struct rcu_pointer_table
 *      | ... (dynamically allocated)
 *      +-struct rcu_pointer_table
 *
 *  The pointer cache expands the pointer table storage when needed.
 */

#define RCU_DEFAULT_MALLOC_ALIGNMENT (RCU_SIZEOF_MEMCELL)
#define rcu_pointer_table_SIZE       32

#define RCU_POINTER_CACHE_1_NAME "rcunit"
#define RCU_POINTER_CACHE_2_NAME "user"

#include "rcunit_list.h"

/* Pointer information */
struct rcu_pointer_info {
    void *ptr;
    size_t size;
    int used;
    char *filename;
    char *function;
    int line_no;
};

/* Pointer table */
struct rcu_pointer_table {
    struct rcu_list link;
    struct rcu_pointer_info tbl[rcu_pointer_table_SIZE];
};

/* Pointer cache */
struct rcu_pointer_cache {
    struct rcu_list ptr_tbl_list;
    size_t nr_alloc;
    size_t nr_free;
    size_t nr_ptr_tbl;
    size_t nr_free_counter;
    size_t max_alloc_size;
    size_t max_total_alloc_size;
    const char *name;
};

extern struct rcu_pointer_cache g_ptr_cache_1;
extern struct rcu_pointer_cache g_ptr_cache_2;

/* Traces the allocation of the given pointer and size */
#define RCU_TRACE_ALLOC(ptr, size) \
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
int rcu_trace_alloc_impl(void *ptr, size_t size, const char *filename, const char *function, const int line,
                         struct rcu_pointer_cache *ptr_cache);

int rcu_trace_free_impl(void *ptr, const char *filename, const char *function, const int line,
                        struct rcu_pointer_cache *ptr_cache);

int rcu_check_mem_leak_impl(const char *filename, const char *function, int line, struct rcu_pointer_cache *ptr_cache);

int rcu_init_mtrace();

int rcu_destroy_mtrace();

void rcu_ensure_mtrace_init();

struct rcu_pointer_cache *rcu_get_rcunit_ptr_cache();

struct rcu_pointer_cache *rcu_get_user_ptr_cache();

#endif /* RCUNIT_MTRACE_H */

