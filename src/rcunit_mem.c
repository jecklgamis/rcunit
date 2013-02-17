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

/*
 * The native memory allocator used by RCUNIT
 */
static int nr_native_allocs = 0;

void *rcu_native_malloc(size_t size) {
    return malloc(size);
}

/*
 *  The native memory deallocator used by RCUNIT
 */
void rcu_native_free(void *addr) {
    if (addr != NULL) {
        free(addr);
    }
}

/*
 *  Internal memory allocation wrapper
 */
RCU_API void *rcu_malloc(size_t size) {
    void *addr = NULL;
    size_t aligned_size = size;
    rcu_init();
#if RCU_ENABLE_RANDOM_MALLOC_FAILURE
    {
        rcu_init_random_number();
        unsigned int trigger = rcu_gen_random_number(1, 10);
        if (trigger < 5) {
            return NULL;
        }
    }
#elif RCU_ENABLE_FORCE_MALLOC_FAILURE
    {
        return NULL;
    }
#endif
#if RCU_ENABLE_MALLOC_ALIGNMENT
    aligned_size = RCU_ALIGN_N(RCU_DEFAULT_MALLOC_ALIGNMENT, size);
#endif
    addr = (void*) rcu_native_malloc(aligned_size);
    if (addr != NULL) {
#if RCU_ENABLE_MTRACE
        RCU_TRACE_ALLOC_INTERNAL(addr, aligned_size);
#endif
    }
    return addr;
}

/*
 * Internal memory deallocation wrapper
 */
RCU_API void rcu_free(void *addr) {
    if (addr != NULL) {
#if RCU_ENABLE_MTRACE
        RCU_TRACE_FREE_INTERNAL(addr);
#endif
    }
    rcu_native_free(addr);
}

/*
 *   Memory cell allocation wrapper
 */
rcu_memcell *rcu_alloc_mem_cell(size_t nr_mem_cell) {
    rcu_memcell *mem_cell = NULL;
    if (nr_mem_cell == 0) {
        return NULL;
    }
    mem_cell = rcu_malloc(sizeof (rcu_memcell) * nr_mem_cell);
    return mem_cell;
}

/*
 *   Memory cell deallocation wrapper
 */
void rcu_free_mem_cell(rcu_memcell **cell) {
    if (cell == NULL || *cell == NULL) {
        return;
    }
    rcu_free(*cell);
    *cell = NULL;
}
