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