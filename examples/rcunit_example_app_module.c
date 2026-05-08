#include <stdio.h>
#include "rcunit.h"

/* This demonstrates the use of user-defined module */
int calc_sum(int x, int y) {
    return x + y;
}

RCU_TEST(test_calc_sum) {
    RCU_ASSERT_EQUAL(-2, calc_sum(-1, -1));
}

int main(int argc, char *argv[]) {
    rcu_module *math = rcu_get_mod("math");
    rcu_run_tests();
    return EXIT_SUCCESS;
}