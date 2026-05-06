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