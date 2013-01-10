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

/** @brief Pointer cache 1*/
RCU_POINTER_CACHE g_ptr_cache_1;

/** @brief Pointer cache 2*/
RCU_POINTER_CACHE g_ptr_cache_2;

/** Initialization flag */
RCU_INT gs_mtrace_init_done=RCU_FALSE;

/** @brief Enable pointer table compaction. Warning do not enable
 *  this for now!
 */
#define RCU_ENABLE_POINTER_TABLE_COMPACTION 0

/**
 *  @brief Creates a pointer table in the pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 *  @param[in] ptr_cache Pointer cache
 */

RCU_INT rcu_cre_ptr_tbl(RCU_POINTER_CACHE *ptr_cache){
RCU_POINTER_TABLE *ptr_tbl;
RCU_INT a=0;
    ptr_tbl = rcu_native_malloc(sizeof(RCU_POINTER_TABLE));
    if (ptr_tbl == RCU_NULL){
        RCU_LOG_FATAL("Unable to allocate pointer table");
        return(RCU_E_NG);
    }
    memset(ptr_tbl,0x00,sizeof(RCU_POINTER_TABLE));
    for(a=0;a<RCU_POINTER_TABLE_SIZE;a++){
        ptr_tbl->tbl[a].ptr = RCU_NULL;
        ptr_tbl->tbl[a].size = 0L;
        ptr_tbl->tbl[a].used  = RCU_FALSE;
    }
    rcu_init_list(&ptr_tbl->link);
    rcu_insert_list(&ptr_cache->ptr_tbl_list,&ptr_tbl->link);
    RCU_INCR(ptr_cache->nr_ptr_tbl);
    return(RCU_E_OK);
}

/**
 *  @brief Finds a free slot from any of the pointer tables in the pointer cache
 *  @return Pointer information
 *  @param[in] ptr_cache Pointer cache
 */

RCU_POINTER_INFO *rcu_find_free_slot(RCU_POINTER_CACHE *ptr_cache){
RCU_GENERIC_LIST *cursor;
RCU_POINTER_TABLE *ptr_tbl;
RCU_POINTER_INFO *ptr_info;
RCU_INT a;
    RCU_FOR_EACH_ENTRY(&ptr_cache->ptr_tbl_list,cursor){
        ptr_tbl = (RCU_POINTER_TABLE*)cursor;
        for(a=0;a<RCU_POINTER_TABLE_SIZE;a++){
            ptr_info = &ptr_tbl->tbl[a];
            if(!ptr_info->used){
                return(ptr_info);
            }
        }
    }
    return(RCU_NULL);
}

/**
 *  @brief Searches the pointer information of the given pointer
 *  @param[in] ptr Pointer
 *  @param[in] ptr_cache Pointer cache
 *  @return Pointer information
 */

RCU_POINTER_INFO *rcu_srch_ptr_info(RCU_VOID *ptr,RCU_POINTER_CACHE
    *ptr_cache){
RCU_GENERIC_LIST *cursor;
RCU_POINTER_TABLE *ptr_tbl;
RCU_POINTER_INFO *ptr_info;
RCU_INT a=0;
    RCU_FOR_EACH_ENTRY(&ptr_cache->ptr_tbl_list,cursor){
        ptr_tbl = (RCU_POINTER_TABLE*)cursor;
        for(a=0;a<RCU_POINTER_TABLE_SIZE;a++){
            ptr_info = &ptr_tbl->tbl[a];
            if(ptr_info->ptr == ptr){
                return(ptr_info);
            }
        }
    }
    return(RCU_NULL);
}

/**
 *  @brief Caches the given pointer and size
 *  @param[in] ptr Pointer
 *  @param[in] size Size
 *  @param[in] ptr_cache Pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_cache_ptr(RCU_VOID *ptr,RCU_U4 size,RCU_POINTER_CACHE
    *ptr_cache){
RCU_POINTER_INFO *ptr_info=RCU_NULL;
RCU_INT ercd;
    if (ptr==RCU_NULL){
        RCU_LOG_WARN("Trying to cache a null pointer");
        return(RCU_E_NG);
    }
    ptr_info = rcu_srch_ptr_info(ptr,ptr_cache);
    if (ptr_info != RCU_NULL){
        RCU_LOG_FATAL_P2("Pointer is currently cached (addr = %p, size = %lu)",
            ptr_info->ptr,ptr_info->size);
        return(RCU_E_NG);
    }
    ptr_info = rcu_find_free_slot(ptr_cache);
    if (ptr_info == RCU_NULL){
        RCU_LOG_INFO("All pointer tables are full. Trying to allocate one");
        ercd = rcu_cre_ptr_tbl(ptr_cache);
        ptr_info = rcu_find_free_slot(ptr_cache);
        if (ptr_info == RCU_NULL){
            RCU_LOG_WARN("Cannot allocate pointer table. Giving up");
            return(RCU_E_NG);
        }
    }
    ptr_info->ptr = ptr;
    ptr_info->size = size;
    ptr_info->used = RCU_TRUE;
    return(RCU_E_OK);
}

/**
 *  @brief Dumps the given pointer table
 *  @param[in] ptr_tbl Pointer table
 *  @param[in] info Some information
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_dump_ptr_tble(RCU_POINTER_TABLE *ptr_tbl,const RCU_CHAR *info){
RCU_INT a;
    RCU_LOG_INFO_P1(" [MTRACE POINTER TABLE DUMP (addr =%p)]",ptr_tbl);
    for(a=0;a<RCU_POINTER_TABLE_SIZE;a++){
        RCU_LOG_INFO_P3("[%u] %s(%p)",a,(ptr_tbl->tbl[a].used)?"USED":"EMPTY",ptr_tbl->tbl[a]);
    }
    RCU_LOG_INFO(" [MTRACE POINTER TABLE DUMP END");
    return(RCU_E_OK);
}

/**
 *  @brief Dumps a pointer cache
 *  @param[in] ptr_cache Pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_dump_ptr_cache(RCU_POINTER_CACHE *ptr_cache){
RCU_GENERIC_LIST *cursor;
RCU_POINTER_TABLE *ptr_tbl;
RCU_CHAR tbl_name[16];
    RCU_LOG_INFO("[MTRACE POINTER CACHE DUMP]");
    RCU_FOR_EACH_ENTRY(&ptr_cache->ptr_tbl_list,cursor){
        ptr_tbl = (RCU_POINTER_TABLE*)cursor;
        memset(tbl_name,0x00,16);
        sprintf(tbl_name,"",ptr_tbl);
        rcu_dump_ptr_tble(ptr_tbl,tbl_name);
    }
    RCU_LOG_INFO("[MTRACE POINTER CACHE DUMP END]");
    return(RCU_E_OK);
}

/**
 *  @brief Shrinks pointer tables
 *  @param[in] ptr_cache Pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_cmpct_ptr_tble(RCU_POINTER_CACHE *ptr_cache){
RCU_POINTER_TABLE *dst_ptr_tbl;
RCU_POINTER_TABLE *src_ptr_tbl;
RCU_GENERIC_LIST  *dst_cursor;
RCU_GENERIC_LIST  *src_cursor;
RCU_INT a,b;
RCU_INT moved_src_index=0;
RCU_INT moved_dst_index=0;
RCU_INT src_tbl_empty;
RCU_INT nr_shrink;

    /** Make sure we have at least two pointer tables */
    if (ptr_cache->nr_ptr_tbl > 1){
        nr_shrink =0;
        RCU_LOG_INFO("Compacting pointer tables");
#if RCU_ENABLE_DEBUG_MTRACE
        rcu_dump_ptr_cache(ptr_cache);
#endif
        src_cursor = ptr_cache->ptr_tbl_list.next;
        dst_cursor = ptr_cache->ptr_tbl_list.prev;
        while(src_cursor != dst_cursor){
            src_ptr_tbl = (RCU_POINTER_TABLE*)src_cursor;
            dst_ptr_tbl = (RCU_POINTER_TABLE*)dst_cursor;
#if RCU_ENABLE_DEBUG_MTRACE
            rcu_dump_ptr_tble(src_ptr_tbl,"src_ptr_tbl(before compaction)");
            rcu_dump_ptr_tble(dst_ptr_tbl,"dst_ptr_tbl(before compaction)");
#endif
            moved_src_index = 0;
            src_tbl_empty = RCU_TRUE;
            for(a=0;a<RCU_POINTER_TABLE_SIZE;a++){
                moved_dst_index = 0;
                if (!src_ptr_tbl->tbl[a].used){
                    continue;
                }
                src_tbl_empty = RCU_FALSE;
                for(b=0;b<RCU_POINTER_TABLE_SIZE;b++){
                    if (dst_ptr_tbl->tbl[b].used){
                        continue;
                    }
                    memcpy(&dst_ptr_tbl->tbl[b],&src_ptr_tbl->tbl[a],sizeof(RCU_POINTER_INFO));
#if RCU_ENABLE_DEBUG_MTRACE
                    RCU_LOG_INFO_P3("moved : si=%lu di=%lu addr= %p",
                        a,b,src_ptr_tbl->tbl[a].ptr);
#endif
                    src_ptr_tbl->tbl[a].ptr = RCU_NULL;
                    src_ptr_tbl->tbl[a].used = RCU_FALSE;
                    moved_src_index = a;
                    moved_dst_index = b;
                    break;
                }
            }
#if RCU_ENABLE_DEBUG_MTRACE
            rcu_dump_ptr_tble(src_ptr_tbl,"src_ptr_tbl(after compaction)");
            rcu_dump_ptr_tble(dst_ptr_tbl,"dst_ptr_tbl(after compaction)");
#endif

            /* if the source pointer table is empty or all the entries are moved
             * then we advance to the next source table.
             */
            if (src_tbl_empty || moved_src_index == RCU_POINTER_TABLE_SIZE - 1){
                {
                    RCU_SAVE_CURSOR(src_cursor)
                    rcu_remove_list(src_cursor);
                    free(src_cursor);
                    RCU_INCR(nr_shrink);
                    RCU_RESTORE_CURSOR(src_cursor)
                }
            }
            /** if the source pointer table is not empty */
            if (moved_dst_index == 0 || moved_dst_index == RCU_POINTER_TABLE_SIZE - 1){
                dst_cursor = dst_cursor->prev;
            }
        }
        RCU_LOG_INFO_P1("Pointer table compaction done. Shrinked %u pointer table(s)",nr_shrink);
    }
#if RCU_ENABLE_DEBUG_MTRACE
    rcu_dump_ptr_cache(ptr_cache);
#endif
    return(RCU_E_OK);
}

/**
 *  @brief Uncaches a given pointer
 *  @param[in] ptr Pointer
 *  @param[in] ptr_cache Pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_uncache_ptr(RCU_VOID *ptr,RCU_POINTER_CACHE *ptr_cache){
RCU_POINTER_INFO *ptr_info=RCU_NULL;

    if (ptr == RCU_NULL){
        RCU_LOG_WARN("Trying to uncache a null pointer");
        return(RCU_E_NG);
    }
    ptr_info = rcu_srch_ptr_info(ptr,ptr_cache);
    if (ptr_info == RCU_NULL){
        RCU_LOG_FATAL_P1("Pointer does not exist in cache : %p",ptr);
        return(RCU_E_NG);
    }
    ptr_info->ptr = RCU_NULL;
    ptr_info->size = 0L;
    ptr_info->used = RCU_FALSE;
    RCU_INCR(ptr_cache->nr_free_counter);
    if (ptr_cache->nr_free_counter==RCU_POINTER_TABLE_SIZE){
        ptr_cache->nr_free_counter=0;
#if RCU_ENABLE_POINTER_TABLE_COMPACTION
        rcu_cmpct_ptr_tble();
#endif
    }
    return(RCU_E_OK);
}

/**
 *  @brief Memory allocation trace implementation
 *  @param[in] ptr Pointer
 *  @param[in] size Size
 *  @param[in] fname Source file name
 *  @param[in] line Source line number
 *  @param[in] ptr_cache Pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_trace_alloc_impl(RCU_VOID *ptr,RCU_U4 size, const RCU_CHAR
    *fname, const RCU_INT line, RCU_POINTER_CACHE *ptr_cache){
    if (!gs_mtrace_init_done){
        RCU_LOG_WARN("Memory tracing facility not initialized");
        return(RCU_E_NG);
    }
#if 0
    RCU_LOG_INFO_P4("ALLOC: addr = %p, size = %lu, file = %s, line = %lu",
        ptr,size,fname,line);
#endif
    RCU_INCR(ptr_cache->nr_alloc);
    rcu_cache_ptr(ptr,size,ptr_cache);

    ptr_cache->max_alloc_size = (size > ptr_cache->max_alloc_size)?
        size:ptr_cache->max_alloc_size;
    ptr_cache->max_total_alloc_size += size;
    return(RCU_E_OK);
}


/**
 *  @brief Memory deallocation trace implementation
 *  @param[in] ptr Pointer
 *  @param[in] fname Source file name
 *  @param[in] line Source line number
 *  @param[in] ptr_cache Pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_trace_free_impl(RCU_VOID *ptr,const RCU_CHAR *fname,const
    RCU_INT line, RCU_POINTER_CACHE *ptr_cache){
RCU_POINTER_INFO *ptr_info=RCU_NULL;

    if (!gs_mtrace_init_done){
        RCU_LOG_WARN("Memory tracing facility not initialized");
        return(RCU_E_NG);
    }
#if 0
    RCU_LOG_INFO_P3("FREE: addr = %p, file = %s, line = %lu",
        ptr,fname,line);
#endif
    RCU_INCR(ptr_cache->nr_free);
    rcu_uncache_ptr(ptr,ptr_cache);
    return(RCU_E_OK);
}

/**
 *  @brief Memory leak check implementation
 *  @param[in] fname Source file name
 *  @param[in] line Source line number
 *  @param[in] ptr_cache Pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_check_mem_leak_impl(const RCU_CHAR *fname,const RCU_INT line,
    RCU_POINTER_CACHE *ptr_cache){
RCU_GENERIC_LIST *cursor;
RCU_POINTER_TABLE *ptr_tbl;
RCU_POINTER_INFO *ptr_info;
RCU_INT a=0;
    if (!gs_mtrace_init_done){
        RCU_LOG_WARN("Memory tracing facility not initialized");
        return(RCU_E_NG);
    }
    ptr_cache->nr_leak =0;
    ptr_cache->leak_size = 0;
    RCU_FOR_EACH_ENTRY(&ptr_cache->ptr_tbl_list,cursor){
        ptr_tbl = (RCU_POINTER_TABLE*)cursor;
        for(a=0;a<RCU_POINTER_TABLE_SIZE;a++){
            ptr_info = &ptr_tbl->tbl[a];
            if(ptr_info->used){
                RCU_LOG_WARN_P2("LEAK :addr=%p size=%lu",ptr_info->ptr,
                    ptr_info->size);
                RCU_INCR(ptr_cache->nr_leak);
                RCU_INCR_BY(ptr_cache->leak_size,ptr_info->size);

            }
        }
    }
    RCU_LOG_INFO_P1("[ MEMORY LEAK CHECK REPORT START (%s) ]",
        ptr_cache->name);
    RCU_LOG_INFO_P4("alloc : %lu calls, free : %lu calls, leaks: %lu, size : %lu bytes",
        ptr_cache->nr_alloc,ptr_cache->nr_free,ptr_cache->nr_leak,
        ptr_cache->leak_size);
    RCU_LOG_INFO_P2("max_alloc_size : %lu bytes, max_total_alloc_size : %lu bytes",
        ptr_cache->max_alloc_size,ptr_cache->max_total_alloc_size);
    RCU_LOG_INFO_P1("[ MEMORY LEAK CHECK REPORT END (%s) ]",
        ptr_cache->name);
    return(RCU_E_OK);
}

/**
 *  @brief Initializes a pointer cache
 *  @param[in] ptr_cache Pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_init_ptr_cache(RCU_POINTER_CACHE *ptr_cache, const RCU_CHAR
    *name){
RCU_INT ercd;
RCU_INT a=0;
    memset(ptr_cache,0x00,sizeof(RCU_POINTER_CACHE));
    rcu_init_list(&ptr_cache->ptr_tbl_list);
    ercd = rcu_cre_ptr_tbl(ptr_cache);
    if (ercd == RCU_E_NG){
        RCU_LOG_FATAL("Unable to create pointer table");
        return(RCU_E_NG);
    }
    ptr_cache->nr_free_counter=0;
    ptr_cache->name = name;
    return(RCU_E_OK);
}

/**
 *  @brief Destroys a pointer cache
 *  @param[in] ptr_cache Pointer cache
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_ptr_cache(RCU_POINTER_CACHE *ptr_cache){
RCU_GENERIC_LIST *cursor;
RCU_POINTER_TABLE *ptr_tbl;

    RCU_FOR_EACH_ENTRY(&ptr_cache->ptr_tbl_list,cursor){
        ptr_tbl = (RCU_POINTER_TABLE*)cursor;
        RCU_SAVE_CURSOR(cursor)
        rcu_remove_list(cursor);
        rcu_native_free(cursor);
        RCU_RESTORE_CURSOR(cursor)
    }
    return(RCU_E_OK);
}

/**
 *  @brief Initializes the memory tracing facility
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_init_mtrace(){
RCU_INT a=0;
    if (gs_mtrace_init_done){
        RCU_LOG_WARN("Memory tracing facility already initialized");
        return(RCU_E_NG);
    }
    gs_mtrace_init_done = RCU_TRUE;
    rcu_init_ptr_cache(&g_ptr_cache_1,RCU_POINTER_CACHE_1_NAME);
    rcu_init_ptr_cache(&g_ptr_cache_2,RCU_POINTER_CACHE_2_NAME);
    RCU_LOG_INFO("Initializing memory tracing facility");
    return(RCU_E_OK);
}

/**
 *  @brief Destroys the memory tracing facility
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_mtrace(){
    RCU_LOG_INFO("Destroying memory tracing facility");
    if (!gs_mtrace_init_done){
        RCU_LOG_WARN("Memory tracing facility not initialized");
        return(RCU_E_NG);
    }
    rcu_destroy_ptr_cache(&g_ptr_cache_1);
    rcu_destroy_ptr_cache(&g_ptr_cache_2);
    gs_mtrace_init_done = RCU_FALSE;
    return(0);
}


