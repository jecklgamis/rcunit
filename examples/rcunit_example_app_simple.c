#include <stdio.h>
#include "rcunit.h"

RCU_TEST(test_something) {
    RCU_ASSERT(1);
}

int main(int argc, char *argv[]) {
    RCU_ADD_TEST(test_something);
    return rcu_run_tests();
}