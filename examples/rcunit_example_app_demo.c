/*
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stdio.h"
#include "rcunit.h"

RCU_TEST(test_func_1) {
    RCU_ASSERT(RCU_TRUE);
}

RCU_TEST(test_func_2) {
    RCU_ASSERT(RCU_TRUE);
}

RCU_FIXTURE(module_setup) {
    puts("module_setup");
}

RCU_FIXTURE(module_teardown) {
    puts("module_teardown");
}

RCU_FIXTURE(module_setup_before_all) {
    puts("module_setup_before_all");
}

RCU_FIXTURE(module_teardown_after_all) {
    puts("module_teardown_after_all");
}

RCU_RUN_HOOK(run_hook) {
    int run_event = RCU_GET_RUN_EVENT_TYPE(param);
    if (run_event == RCU_TEST_RUN_STARTED) {
        puts("Test started");
    } else if (run_event == RCU_TEST_RUN_FINISHED) {
        puts("Test finished");
    }
}
int main(int argc, char **argv) {
    RCU_ADD_TEST("default", test_func_1)
    RCU_ADD_TEST("default", test_func_2)
    RCU_SET_MODULE_FIXTURES("default", module_setup, module_teardown)
    RCU_SET_MODULE_FIXTURES_ALL("default", module_setup_before_all, module_teardown_after_all)
    rcu_dump_test_registry();
    rcu_set_run_hook(run_hook);
    return rcu_run_tests();
}
