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

