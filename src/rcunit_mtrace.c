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

#include "rcunit.h"

#define RCU_ENABLE_DEBUG_MTRACE 1

rcu_pointer_cache g_ptr_cache_1;
rcu_pointer_cache g_ptr_cache_2;
int gs_mtrace_init_done = RCU_FALSE;

/* Enable pointer table compaction. Warning do not enable
 *  this for now!
 */
#define RCU_ENABLE_POINTER_TABLE_COMPACTION 0

int rcu_cre_ptr_tbl(rcu_pointer_cache *ptr_cache) {
    rcu_pointer_table *ptr_tbl = NULL;
    int a = 0;

    ptr_tbl = rcu_native_malloc(sizeof(rcu_pointer_table));
    if (ptr_tbl == NULL) {
        RCU_LOG_FATAL("Unable to allocate pointer table");
        return RCU_E_NG;
    }
    memset(ptr_tbl, 0, sizeof(rcu_pointer_table));
    for (a = 0; a < rcu_pointer_table_SIZE; a++) {
        ptr_tbl->tbl[a].ptr = NULL;
        ptr_tbl->tbl[a].size = 0L;
        ptr_tbl->tbl[a].used = RCU_FALSE;
    }
    rcu_init_list(&ptr_tbl->link);
    rcu_insert_list(&ptr_cache->ptr_tbl_list, &ptr_tbl->link);
    RCU_INCR(ptr_cache->nr_ptr_tbl);
    return RCU_E_OK;
}

/*
 *  Finds a free slot from any of the pointer tables in the pointer cache
 */
rcu_pointer_info *rcu_find_free_slot(rcu_pointer_cache *ptr_cache) {
    rcu_list *cursor = NULL;
    rcu_pointer_table *ptr_tbl = NULL;
    rcu_pointer_info *ptr_info = NULL;
    int a;

    RCU_FOR_EACH_ENTRY(&ptr_cache->ptr_tbl_list, cursor) {
        ptr_tbl = (rcu_pointer_table *) cursor;
        for (a = 0; a < rcu_pointer_table_SIZE; a++) {
            ptr_info = &ptr_tbl->tbl[a];
            if (!ptr_info->used) {
                return ptr_info;
            }
        }
    }
    return NULL;
}

/**
 *  Searches the pointer information of the given pointer
 */
rcu_pointer_info *rcu_srch_ptr_info(void *ptr, rcu_pointer_cache *ptr_cache) {
    int a = 0;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&ptr_cache->ptr_tbl_list, cursor) {
        rcu_pointer_table *ptr_tbl = (rcu_pointer_table *) cursor;
        for (a = 0; a < rcu_pointer_table_SIZE; a++) {
            rcu_pointer_info *ptr_info = &ptr_tbl->tbl[a];
            if (ptr_info->ptr == ptr) {
                return ptr_info;
            }
        }
    }
    return NULL;
}

/**
 *  Caches the given pointer and size
 */
int rcu_cache_ptr(void *ptr, size_t size, rcu_pointer_cache *ptr_cache) {
    rcu_pointer_info *ptr_info = NULL;
    int ercd;

    if (ptr == NULL) {
        RCU_LOG_WARN("Trying to cache a null pointer");
        return RCU_E_NG;
    }
    ptr_info = rcu_srch_ptr_info(ptr, ptr_cache);
    if (ptr_info != NULL) {
        RCU_LOG_FATAL("Pointer is currently cached (addr = %p, size = %lu)", ptr_info->ptr, ptr_info->size);
        return RCU_E_NG;
    }
    ptr_info = rcu_find_free_slot(ptr_cache);
    if (ptr_info == NULL) {
        RCU_LOG_DEBUG("All pointer tables are full. Trying to allocate one");
        ercd = rcu_cre_ptr_tbl(ptr_cache);
        ptr_info = rcu_find_free_slot(ptr_cache);
        if (ptr_info == NULL) {
            RCU_LOG_WARN("Cannot allocate pointer table. Giving up");
            return RCU_E_NG;
        }
    }
    ptr_info->ptr = ptr;
    ptr_info->size = size;
    ptr_info->used = RCU_TRUE;
    return RCU_E_OK;
}

int rcu_dump_ptr_tble(rcu_pointer_table *ptr_tbl, const char *info) {
    int a;
    RCU_LOG_INFO(" [MTRACE POINTER TABLE DUMP (addr =%p)]", ptr_tbl);
    for (a = 0; a < rcu_pointer_table_SIZE; a++) {
        RCU_LOG_INFO("[%u] %s(%p)", a, (ptr_tbl->tbl[a].used) ? "USED" : "EMPTY", ptr_tbl->tbl[a]);
    }
    RCU_LOG_INFO(" [MTRACE POINTER TABLE DUMP END");
    return RCU_E_OK;
}

int rcu_dump_ptr_cache(rcu_pointer_cache *ptr_cache) {
    rcu_list *cursor = NULL;
    rcu_pointer_table *ptr_tbl = NULL;
    char tbl_name[16];

    RCU_LOG_INFO("[MTRACE POINTER CACHE DUMP]");

    RCU_FOR_EACH_ENTRY(&ptr_cache->ptr_tbl_list, cursor) {
        ptr_tbl = (rcu_pointer_table *) cursor;
        memset(tbl_name, 0x00, 16);
        sprintf(tbl_name, "", ptr_tbl);
        rcu_dump_ptr_tble(ptr_tbl, tbl_name);
    }
    RCU_LOG_INFO("[MTRACE POINTER CACHE DUMP END]");
    return RCU_E_OK;
}

int rcu_cmpct_ptr_tble(rcu_pointer_cache *ptr_cache) {
    rcu_pointer_table *dst_ptr_tbl = NULL;
    rcu_pointer_table *src_ptr_tbl = NULL;
    rcu_list *dst_cursor = NULL;
    rcu_list *src_cursor = NULL;
    int a, b;
    int moved_src_index = 0;
    int moved_dst_index = 0;
    int src_tbl_empty;
    int nr_shrink;

    /** Make sure we have at least two pointer tables */
    if (ptr_cache->nr_ptr_tbl > 1) {
        nr_shrink = 0;
        RCU_LOG_INFO("Compacting pointer tables");
#if RCU_ENABLE_DEBUG_MTRACE
        rcu_dump_ptr_cache(ptr_cache);
#endif
        src_cursor = ptr_cache->ptr_tbl_list.next;
        dst_cursor = ptr_cache->ptr_tbl_list.prev;
        while (src_cursor != dst_cursor) {
            src_ptr_tbl = (rcu_pointer_table *) src_cursor;
            dst_ptr_tbl = (rcu_pointer_table *) dst_cursor;
#if RCU_ENABLE_DEBUG_MTRACE
            rcu_dump_ptr_tble(src_ptr_tbl, "src_ptr_tbl(before compaction)");
            rcu_dump_ptr_tble(dst_ptr_tbl, "dst_ptr_tbl(before compaction)");
#endif
            moved_src_index = 0;
            src_tbl_empty = RCU_TRUE;
            for (a = 0; a < rcu_pointer_table_SIZE; a++) {
                moved_dst_index = 0;
                if (!src_ptr_tbl->tbl[a].used) {
                    continue;
                }
                src_tbl_empty = RCU_FALSE;
                for (b = 0; b < rcu_pointer_table_SIZE; b++) {
                    if (dst_ptr_tbl->tbl[b].used) {
                        continue;
                    }
                    memcpy(&dst_ptr_tbl->tbl[b], &src_ptr_tbl->tbl[a],
                           sizeof(rcu_pointer_info));
#if RCU_ENABLE_DEBUG_MTRACE
                    RCU_LOG_INFO("moved : si=%lu di=%lu addr= %p",
                                 a, b, src_ptr_tbl->tbl[a].ptr);
#endif
                    src_ptr_tbl->tbl[a].ptr = NULL;
                    src_ptr_tbl->tbl[a].used = RCU_FALSE;
                    moved_src_index = a;
                    moved_dst_index = b;
                    break;
                }
            }
#if RCU_ENABLE_DEBUG_MTRACE
            rcu_dump_ptr_tble(src_ptr_tbl, "src_ptr_tbl(after compaction)");
            rcu_dump_ptr_tble(dst_ptr_tbl, "dst_ptr_tbl(after compaction)");
#endif

            /* if the source pointer table is empty or all the entries are moved
             * then we advance to the next source table.
             */
            if (src_tbl_empty || moved_src_index == rcu_pointer_table_SIZE - 1) {
                {
                    RCU_SAVE_CURSOR(src_cursor)
                        rcu_remove_list(src_cursor);
                        free(src_cursor);
                        RCU_INCR(nr_shrink);
                    RCU_RESTORE_CURSOR(src_cursor)
                }
            }
            /** if the source pointer table is not empty */
            if (moved_dst_index == 0 || moved_dst_index == rcu_pointer_table_SIZE - 1) {
                dst_cursor = dst_cursor->prev;
            }
        }
        RCU_LOG_INFO("Pointer table compaction done. Shrinked %u pointer table(s)", nr_shrink);
    }
#if RCU_ENABLE_DEBUG_MTRACE
    rcu_dump_ptr_cache(ptr_cache);
#endif
    return RCU_E_OK;
}

/**
 *  Uncaches a given pointer
 */
int rcu_uncache_ptr(void *ptr, rcu_pointer_cache *ptr_cache) {
    rcu_pointer_info *ptr_info = NULL;

    if (ptr == NULL) {
        RCU_LOG_WARN("Trying to uncache a null pointer");
        return RCU_E_NG;
    }
    ptr_info = rcu_srch_ptr_info(ptr, ptr_cache);
    if (ptr_info == NULL) {
        RCU_LOG_FATAL("Pointer does not exist in cache : %p", ptr);
        return RCU_E_NG;
    }
    ptr_info->ptr = NULL;
    ptr_info->size = 0L;
    ptr_info->used = RCU_FALSE;
    RCU_INCR(ptr_cache->nr_free_counter);
    if (ptr_cache->nr_free_counter == rcu_pointer_table_SIZE) {
        ptr_cache->nr_free_counter = 0;
#if RCU_ENABLE_POINTER_TABLE_COMPACTION
        rcu_cmpct_ptr_tble();
#endif
    }
    return RCU_E_OK;
}

/**
 *  Memory allocation trace implementation
 */
RCU_API int rcu_trace_alloc_impl(void *ptr, size_t size,
                                 const char *filename, const char *function, const int line,
                                 rcu_pointer_cache *ptr_cache) {
    rcu_ensure_mtrace_init();
    RCU_INCR(ptr_cache->nr_alloc);
    rcu_cache_ptr(ptr, size, ptr_cache);
    ptr_cache->max_alloc_size = (size > ptr_cache->max_alloc_size) ? size
                                                                   : ptr_cache->max_alloc_size;
    ptr_cache->max_total_alloc_size += size;
    return RCU_E_OK;
}

/**
 *  Memory deallocation trace implementation
 */
RCU_API int rcu_trace_free_impl(void *ptr, const char *filename, const char *function,
                                const int line, rcu_pointer_cache *ptr_cache) {
    rcu_pointer_info *ptr_info = NULL;
    rcu_ensure_mtrace_init();
    RCU_INCR(ptr_cache->nr_free);
    rcu_uncache_ptr(ptr, ptr_cache);
    return RCU_E_OK;
}

RCU_API int rcu_check_mem_leak_impl(const char *filename, const char *function,
                                    const int line, rcu_pointer_cache *ptr_cache) {
    rcu_list *cursor;
    rcu_pointer_table *ptr_tbl;
    rcu_pointer_info *ptr_info;
    int a = 0;
    int nr_leaks = 0;
    size_t leak_size = 0;

    rcu_init();
    rcu_ensure_mtrace_init();
    RCU_LOG_INFO("Checking memory leak in %s:%d", filename, line);

    RCU_FOR_EACH_ENTRY(&ptr_cache->ptr_tbl_list, cursor) {
        ptr_tbl = (rcu_pointer_table *) cursor;
        for (a = 0; a < rcu_pointer_table_SIZE; a++) {
            ptr_info = &ptr_tbl->tbl[a];
            if (ptr_info->used) {
                RCU_LOG_WARN("Found leak : addr = %p size= % lu", ptr_info->ptr, ptr_info->size);
                RCU_INCR(nr_leaks);
                RCU_INCR_BY(leak_size, ptr_info->size);
            }
        }
    }
    RCU_LOG_INFO("[ MEMORY LEAK CHECK REPORT  (%s) ]", ptr_cache->name);
    RCU_LOG_INFO("alloc = %lu calls, free = %lu calls, nr_leaks = %d, size = %lu bytes",
                 ptr_cache->nr_alloc, ptr_cache->nr_free, nr_leaks, leak_size);
    RCU_LOG_INFO("max_alloc_size = %lu bytes, total_alloc_size = %lu bytes",
                 ptr_cache->max_alloc_size, ptr_cache->max_total_alloc_size);
    return RCU_E_OK;
}

RCU_API int rcu_has_mem_leak() {
    int nr_leaks = 0;
    size_t leak_size = 0;
    rcu_get_mtrace_results(rcu_get_rcunit_ptr_cache(), &nr_leaks, &leak_size);
    return nr_leaks > 0 ? RCU_TRUE : RCU_FALSE;
}

void rcu_get_mtrace_results(rcu_pointer_cache *ptr_cache, int *nr_leaks, size_t *leak_size) {
    int a = 0;
    rcu_ensure_mtrace_init();

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&ptr_cache->ptr_tbl_list, cursor) {
        rcu_pointer_table *ptr_tbl = (rcu_pointer_table *) cursor;
        for (a = 0; a < rcu_pointer_table_SIZE; a++) {
            rcu_pointer_info *ptr_info = &ptr_tbl->tbl[a];
            if (ptr_info->used) {
                RCU_INCR(*nr_leaks);
                RCU_INCR_BY(*leak_size, ptr_info->size);
            }
        }
    }
}

/**
 *  Initializes a pointer cache
 */
int rcu_init_ptr_cache(rcu_pointer_cache *ptr_cache, const char *name) {
    int ercd;
    int a = 0;

    memset(ptr_cache, 0, sizeof(rcu_pointer_cache));
    rcu_init_list(&ptr_cache->ptr_tbl_list);
    ercd = rcu_cre_ptr_tbl(ptr_cache);
    if (ercd == RCU_E_NG) {
        RCU_LOG_FATAL("Unable to create pointer table");
        return RCU_E_NG;
    }
    ptr_cache->nr_free_counter = 0;
    ptr_cache->name = name;
    return RCU_E_OK;
}

/**
 *  Destroys a pointer cache
 */
int rcu_destroy_ptr_cache(rcu_pointer_cache *ptr_cache) {
    rcu_list *cursor;
    rcu_pointer_table *ptr_tbl;

    RCU_FOR_EACH_ENTRY(&ptr_cache->ptr_tbl_list, cursor) {
        ptr_tbl = (rcu_pointer_table *) cursor;
        RCU_SAVE_CURSOR(cursor)
            rcu_remove_list(cursor);
            rcu_native_free(cursor);
        RCU_RESTORE_CURSOR(cursor)
    }
    return RCU_E_OK;
}

/**
 *  Initializes the memory tracing facility
 */
int rcu_init_mtrace() {
    if (gs_mtrace_init_done) {
        return RCU_E_OK;
    }
    RCU_LOG_DEBUG("Initializing memory tracing");
    rcu_init_ptr_cache(&g_ptr_cache_1, RCU_POINTER_CACHE_1_NAME);
    rcu_init_ptr_cache(&g_ptr_cache_2, RCU_POINTER_CACHE_2_NAME);
    gs_mtrace_init_done = RCU_TRUE;
    return RCU_E_OK;
}

int rcu_destroy_mtrace() {
    RCU_LOG_DEBUG("Destroying memory tracing");
    if (!gs_mtrace_init_done) {
        return RCU_E_NG;
    }
    rcu_destroy_ptr_cache(&g_ptr_cache_1);
    rcu_destroy_ptr_cache(&g_ptr_cache_2);
    gs_mtrace_init_done = RCU_FALSE;
    return RCU_E_OK;
}

void rcu_ensure_mtrace_init() {
    if (!gs_mtrace_init_done) {
        rcu_init_mtrace();
    }
}

rcu_pointer_cache *rcu_get_rcunit_ptr_cache() {
    return &g_ptr_cache_1;
}

rcu_pointer_cache *rcu_get_user_ptr_cache() {
    return &g_ptr_cache_2;
}

