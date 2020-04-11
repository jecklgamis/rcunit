#include <stdio.h>
#include "rcunit.h"

int calc_sum(int x, int y) {
    return x + y;
}

int calc_diff(int x, int y) {
    return x - y;
}

RCU_TEST(test_calc_sum) {
        RCU_ASSERT_EQUAL(3, calc_sum(1, 2))
}

RCU_TEST(test_calc_diff) {
        RCU_ASSERT_EQUAL(-1, calc_diff(1, 2))
}

RCU_DEF_FUNC_TBL(calc_tests_table)
RCU_INC_TEST(test_calc_sum)
RCU_INC_TEST(test_calc_diff)

RCU_DEF_FUNC_TBL_END

int main(int argc, char *argv[]) {
    rcu_add_test_func_tbl(rcu_get_default_mod(), calc_tests_table);
    rcu_run_tests();
    return EXIT_SUCCESS;
}