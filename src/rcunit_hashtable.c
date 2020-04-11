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

#include "rcunit_hashtable.h"

extern rcu_memcell *rcu_alloc_mem_cell(size_t nr_mem_cell);

rcu_hashtable *rcu_cre_hash_tbl(int nr_bucket) {
    rcu_hashtable *tbl;
    int tbl_size;
    if (nr_bucket == 0) {
        return NULL;
    }
    tbl_size = RCU_SIZEOF_HASHTABLE(nr_bucket);
    tbl = (rcu_hashtable *) rcu_alloc_mem_cell(tbl_size);
    if (tbl == NULL) {
        return NULL;
    }
    return NULL;
}

int rcu_gen_int32_hash_jenkins_impl(int key) {
    key += (key << 12);
    key ^= (key >> 22);
    key += (key << 4);
    key ^= (key >> 9);
    key += (key << 10);
    key ^= (key >> 2);
    key += (key << 7);
    key ^= (key >> 12);
    return (key);
}

int rcu_gen_int32_hash(int key) {
    return rcu_gen_int32_hash_jenkins_impl(key);
}

int rcu_gen_str_hash(const char *strz) {
    int hash = 0;
    while (*(strz++)) {
        int c = *strz;
        hash = (((hash << 3) + hash) << 2) + hash + c;
    }
    return hash;
}

