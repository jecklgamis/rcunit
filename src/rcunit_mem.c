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

/** @brief Enable random malloc failure (for testing) */
#define RCU_ENABLE_RANDOM_MALLOC_FAILURE 0


/**
 *   @brief The native memory allocator used by RCUNIT
 *   @param[in] size Number of bytes to allocate
 *   @return Pointer to the allocated memory or RCU_NULL
 */

RCU_VOID *rcu_native_malloc(RCU_U4 size){
    return(malloc(size));
}

/**
 *   @brief The native memory deallocator used by RCUNIT
 *   @param[in] addr Allocated memory
 */

RCU_VOID rcu_native_free(RCU_VOID *addr){
    free(addr);
}

/**
 *   @brief Internal memory allocation wrapper
 *   @param[in] size Number of bytes to allocate
 *   @return Pointer to the allocated memory or RCU_NULL
 */

RCU_INTERFACE RCU_VOID *rcu_malloc(RCU_U4 size){
RCU_VOID *addr=RCU_NULL;
RCU_TEST_MACHINE *mach;
RCU_U4 aligned_size=size;
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_NULL);
    }
#if RCU_ENABLE_RANDOM_MALLOC_FAILURE
    {
        RCU_U4 trigger = rcu_gen_random_number(0,512);
        if (trigger < 256){
            return(RCU_NULL);
        }
    }
#endif
#if RCU_ENABLE_MALLOC_ALIGNMENT
    aligned_size = RCU_ALIGN_N(RCU_DEFAULT_MALLOC_ALIGNMENT,size);
#endif
    addr = (RCU_VOID*)rcu_native_malloc(aligned_size);
    if (addr != RCU_NULL){
#if RCU_ENABLE_TRACE_ALLOC
        RCU_LOG_INFO_P3("Allocated : addr = %p, size = %lu bytes (requested = %lu)",addr,aligned_size,size);
#endif
#if RCU_ENABLE_MTRACE
        RCU_TRACE_ALLOC_INTERNAL(addr,aligned_size);
#endif
    }
    return(addr);
}

/**
 *   @brief Internal memory deallocation wrapper
 *   @param[in] addr Allocated memory
 */

RCU_INTERFACE RCU_VOID rcu_free(RCU_VOID *addr){
RCU_TEST_MACHINE *mach;
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return;
    }
    if (addr != RCU_NULL){
#if RCU_ENABLE_MTRACE
        RCU_TRACE_FREE_INTERNAL(addr);
#endif
#if RCU_ENABLE_TRACE_ALLOC
        RCU_LOG_INFO_P1("Deallocated : addr = %p",addr);
#endif
    }
    rcu_native_free(addr);
}

/**
 *   @brief Memory cell allocation wrapper
 *   @param[in] nr_mem_cell Number of memory cells to allocate
 *   @return Allocated memory cell
 */

RCU_MEMCELL *rcu_alloc_mem_cell(RCU_U4 nr_mem_cell){
RCU_MEMCELL *mem_cell;
    if (nr_mem_cell == 0){
        return(RCU_NULL);
    }
    mem_cell = rcu_malloc(sizeof(RCU_MEMCELL)*nr_mem_cell);
    return(mem_cell);
}

/**
 *   @brief Memory cell deallocation wrapper
 *   @param[in] mem_cell Allocated memory cell
 */

RCU_VOID rcu_free_mem_cell(RCU_MEMCELL **mem_cell){
    if (mem_cell == RCU_NULL || *mem_cell == RCU_NULL){
        return;
    }
    rcu_free(*mem_cell);
    *mem_cell = RCU_NULL;
}
