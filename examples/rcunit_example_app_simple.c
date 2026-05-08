#include <stdio.h>
#include "rcunit.h"

RCU_TEST(test_something) {
    RCU_ASSERT(1);
}

int main(int argc, char *argv[]) {
    rcu_add_test(test_something);
    rcu_run_tests();
    return EXIT_SUCCESS;
}