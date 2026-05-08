/*
 * The MIT License (MIT)
 *
 * RCUNIT - A unit testing framework for C
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
    TMK_ASSERT_NOT_NULL(rcu_get_module("default-module"));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods())
}

/* Verify the default module name cannot be used in creating module */
TMK_TEST(rcu_test_create_default_module) {
    TMK_ASSERT_NULL(rcu_create_test_module("default-module", NULL, NULL));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods())
}

static int nr_module_fixture_calls = 0;

RCU_SETUP(mod_setup) {
    nr_module_fixture_calls++;
}

RCU_TEARDOWN(mod_teardown) {
    nr_module_fixture_calls++;
}

/* Verify that the module's setup function is invoked*/
TMK_TEST(rcu_test_module_fixture_invocation) {
    RCU_DEF_TEST_MODULE(module = rcu_get_module("m1"));
    TMK_ASSERT_NOT_NULL(module);
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods())
    rcu_set_module_fixture(module, mod_setup, mod_teardown);
    rcu_run_tests();
    TMK_ASSERT_EQUAL(2, nr_module_fixture_calls);
}

TMK_TEST(rcu_test_create_multiple_mods) {
    TMK_ASSERT_NOT_NULL(rcu_get_module("m1"));
    TMK_ASSERT_NOT_NULL(rcu_get_module("m2"));
    TMK_ASSERT_EQUAL(3, rcu_get_nr_mods());
}

TMK_TEST(rcu_test_create_and_destroy_module) {
    RCU_DEF_TEST_MODULE(m1)
    TMK_ASSERT_NOT_NULL(m1 = rcu_create_test_module("m1", NULL, NULL));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_module(m1));
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods())

    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_destroy_test_module(m1));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods())
}

RCU_DEF_FUNC_TBL(test_tbl)
RCU_INC_TEST(test)
RCU_DEF_FUNC_TBL_END

RCU_DEF_MODULE_TBL(mod_tbl)
RCU_DEF_MODULE_TBL_END

TMK_TEST(rcu_test_add_module_tbl) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_module_tbl(rcu_get_default_reg(), mod_tbl));
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

TMK_TEST(rcu_test_add_null_module_tbl) {
    TMK_ASSERT_EQUAL(RCU_E_NG, rcu_add_test_module_tbl(NULL, NULL));
}

RCU_DEF_MODULE_TBL(rcu_test_module_017_mtbl1)
RCU_INC_MODULE_FXT("rcu_test_module_017_m1", NULL, NULL, NULL)
RCU_DEF_MODULE_TBL_END

TMK_TEST(rcu_test_module_on_null_reg) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_module_tbl(NULL, rcu_test_module_017_mtbl1));
}

TMK_TEST(rcu_test_run_module) {
    RCU_DEF_TEST_MODULE(m1)
    TMK_ASSERT_NOT_NULL(m1 = rcu_get_module("m1"));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_test_module(m1));
}

TMK_TEST(rcu_test_run_module_by_name) {
    RCU_DEF_TEST_MODULE(m1)
    TMK_ASSERT_NOT_NULL(m1 = rcu_get_module("m1"));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_test_module_by_name("m1"));
}

TMK_TEST(rcu_test_run_module_by_unknown_name) {
    RCU_DEF_TEST_MODULE(m1)
    TMK_ASSERT_NOT_NULL(m1 = rcu_get_module("m1"));
    TMK_ASSERT_EQUAL(RCU_E_NG, rcu_run_test_module_by_name("unknown_module_name"));
}


