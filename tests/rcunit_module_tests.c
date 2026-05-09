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

/* Verify that we can create a test module */
TMK_TEST(rcu_test_get_module) {
    TMK_ASSERT_NOT_NULL(rcu_get_module("m1"));
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods())
}

/* Verify that we can retrieve the default test module */
TMK_TEST(rcu_test_get_default_module) {
    struct rcu_module *module = rcu_get_default_module();
    TMK_ASSERT_NOT_NULL(module);
}

/* Verify the default module can be retrieved by name */
TMK_TEST(rcu_test_get_default_module_by_name) {
    TMK_ASSERT_NOT_NULL(rcu_get_module("default"));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods())
}

/* Verify the default module name cannot be used in creating module */
TMK_TEST(rcu_test_create_default_module) {
    TMK_ASSERT_NULL(rcu_create_test_module("default", NULL, NULL));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods())
}

static int nr_module_fixture_calls = 0;

RCU_FIXTURE(mod_setup) {
    nr_module_fixture_calls++;
}

RCU_FIXTURE(mod_teardown) {
    nr_module_fixture_calls++;
}

/* Verify that the module's setup function is invoked */
TMK_TEST(rcu_test_module_fixture_invocation) {
    struct rcu_module *module = rcu_get_module("m1");
    TMK_ASSERT_NOT_NULL(module);
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods())
    rcu_set_module_fixture_all(module, mod_setup, mod_teardown);
    rcu_run_tests();
    rcu_dump_test_registry();
    TMK_ASSERT_EQUAL(2, nr_module_fixture_calls);
}

TMK_TEST(rcu_test_create_multiple_mods) {
    TMK_ASSERT_NOT_NULL(rcu_get_module("m1"));
    TMK_ASSERT_NOT_NULL(rcu_get_module("m2"));
    TMK_ASSERT_EQUAL(3, rcu_get_nr_mods());
}

TMK_TEST(rcu_test_create_and_destroy_module) {
    struct rcu_module *m1;
    TMK_ASSERT_NOT_NULL(m1 = rcu_create_test_module("m1", NULL, NULL));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_module(m1));
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods())

    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_destroy_test_module(m1));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods())
}

TMK_TEST(rcu_test_run_module) {
    struct rcu_module *m1;
    TMK_ASSERT_NOT_NULL(m1 = rcu_get_module("m1"));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_test_module(m1));
}

TMK_TEST(rcu_test_run_module_by_name) {
    struct rcu_module *m1;
    TMK_ASSERT_NOT_NULL(m1 = rcu_get_module("m1"));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_test_module_by_name("m1"));
}

TMK_TEST(rcu_test_run_module_by_unknown_name) {
    struct rcu_module *m1;
    TMK_ASSERT_NOT_NULL(m1 = rcu_get_module("m1"));
    TMK_ASSERT_EQUAL(RCU_E_NG, rcu_run_test_module_by_name("unknown_module_name"));
}


