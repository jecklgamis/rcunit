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

RCU_TEST(passing_func) {
    RCU_ASSERT(RCU_TRUE);
}

RCU_TEST(empty_func) {
    /* no assertions */
}

RCU_TEST(non_fatal_failing_func) {
    RCU_FAIL("intentional failure");
}

RCU_TEST(assert_false_func) {
    RCU_ASSERT(RCU_FALSE);
}

static int fixture_call_order[3];
static int fixture_call_idx = 0;

RCU_FIXTURE(ordered_setup) {
    fixture_call_order[fixture_call_idx++] = 1;
}

RCU_TEST(ordered_func) {
    fixture_call_order[fixture_call_idx++] = 2;
    RCU_ASSERT(RCU_TRUE);
}

RCU_FIXTURE(ordered_teardown) {
    fixture_call_order[fixture_call_idx++] = 3;
}

RCU_FIXTURE(failing_init) {
    RCU_FAIL("intentional module init failure");
}

TMK_TEST(rcu_test_passing_run) {
    rcu_add_test(passing_func);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_no_assertions_fails) {
    rcu_add_test(empty_func);
    TMK_ASSERT_EQUAL(RCU_E_NG, rcu_run_tests());
}

TMK_TEST(rcu_test_non_fatal_failure) {
    rcu_add_test(non_fatal_failing_func);
    TMK_ASSERT_EQUAL(RCU_E_NG, rcu_run_tests());
}

TMK_TEST(rcu_test_assert_false_fails) {
    rcu_add_test(assert_false_func);
    TMK_ASSERT_EQUAL(RCU_E_NG, rcu_run_tests());
}

TMK_TEST(rcu_test_fixture_invocation_order) {
    fixture_call_idx = 0;
    struct rcu_module *mod = rcu_get_module("ordered_mod");
    rcu_set_module_fixture(mod, ordered_setup, ordered_teardown);
    rcu_add_test_to_module(mod, ordered_func);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
    TMK_ASSERT_EQUAL(1, fixture_call_order[0]);
    TMK_ASSERT_EQUAL(2, fixture_call_order[1]);
    TMK_ASSERT_EQUAL(3, fixture_call_order[2]);
}

TMK_TEST(rcu_test_module_init_failure) {
    struct rcu_module *mod = rcu_get_module("failing_init_mod");
    rcu_set_module_fixture(mod, failing_init, NULL);
    rcu_add_test_to_module(mod, passing_func);
    TMK_ASSERT_EQUAL(RCU_E_NG, rcu_run_tests());
}
