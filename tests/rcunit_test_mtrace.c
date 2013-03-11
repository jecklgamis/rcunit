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




