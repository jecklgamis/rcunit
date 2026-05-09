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

#include "rcunit_test.h"

#define NR_ALLOCS 16

TMK_TEST(rcu_test_mtrace) {
    int a = 0;
    void *ptr[NR_ALLOCS];
    rcu_init();
    for (a = 0; a < NR_ALLOCS; a++) {
        ptr[a] = rcu_malloc(a + 1);
    }
    for (a = 0; a < NR_ALLOCS; a++) {
        rcu_free(ptr[a]);
    }
    void *ptr2 = rcu_malloc(8);
    TMK_ASSERT(rcu_has_mem_leak());
    rcu_free(ptr2);
    TMK_ASSERT_FALSE(rcu_has_mem_leak());            
}




