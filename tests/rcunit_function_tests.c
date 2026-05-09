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

#include "rcunit_test.h" 

extern void test(void *param);
extern void setup(void *param);
extern void teardown(void *param);

TMK_TEST(rcu_test_add_test) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test(test));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_add_test_to_module) {
    struct rcu_module *module = rcu_get_module("module");
    TMK_ASSERT_NOT_NULL(module);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_to_module(module, test));
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_add_test_fixture_to_module) {
    RCU_ADD_TEST("module",test);
    rcu_set_module_fixture(rcu_get_module("module"), setup, teardown);
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods());
    TMK_ASSERT_EQUAL(1, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_add_test_to_default_module) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_to_module(rcu_get_default_module(), test));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_add_test_fixture_to_default_module) {
    rcu_set_module_fixture(rcu_get_default_module(), setup, teardown);
    TMK_ASSERT_EQUAL(1, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

