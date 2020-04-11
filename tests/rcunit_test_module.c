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
TMK_TEST(rcu_test_get_mod) {
    TMK_ASSERT_NOT_NULL(rcu_get_mod("m1"));
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods())
}

/* Verify that we can retrieve the default test module */
TMK_TEST(rcu_test_get_default_mod) {
    rcu_module *mod = rcu_get_default_mod();
    TMK_ASSERT_NOT_NULL(mod);
}

/* Verify the default module can be retrieved by name */
TMK_TEST(rcu_test_get_default_mod_by_name) {
    TMK_ASSERT_NOT_NULL(rcu_get_mod("default-mod"));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods())
}

/* Verify the default module name cannot be used in creating module */
TMK_TEST(rcu_test_create_default_mod) {
    TMK_ASSERT_NULL(rcu_cre_test_mod("default-mod", NULL, NULL));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods())
}

static int nr_mod_fxt_calls = 0;

RCU_SETUP(mod_setup) {
    nr_mod_fxt_calls++;
}

RCU_TEARDOWN(mod_teardown) {
    nr_mod_fxt_calls++;
}

/* Verify that the module's setup function is invoked*/
TMK_TEST(rcu_test_mod_fixture_invocation) {
    RCU_DEF_TEST_MOD(mod = rcu_get_mod("m1"));
    TMK_ASSERT_NOT_NULL(mod);
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods())
    rcu_set_mod_fxt(mod, mod_setup, mod_teardown);
    rcu_run_tests();
    TMK_ASSERT_EQUAL(2, nr_mod_fxt_calls);
}

TMK_TEST(rcu_test_create_multiple_mods) {
    TMK_ASSERT_NOT_NULL(rcu_get_mod("m1"));
    TMK_ASSERT_NOT_NULL(rcu_get_mod("m2"));
    TMK_ASSERT_EQUAL(3, rcu_get_nr_mods());
}

TMK_TEST(rcu_test_create_and_destroy_mod) {
    RCU_DEF_TEST_MOD(m1)
    TMK_ASSERT_NOT_NULL(m1 = rcu_cre_test_mod("m1", NULL, NULL));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_mod(m1));
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods())

    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_destroy_test_mod(m1));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods())
}

RCU_DEF_FUNC_TBL(test_tbl)
RCU_INC_TEST(test)
RCU_DEF_FUNC_TBL_END

RCU_DEF_MOD_TBL(mod_tbl)
RCU_DEF_MOD_TBL_END

TMK_TEST(rcu_test_add_mod_tbl) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_mod_tbl(rcu_get_default_reg(), mod_tbl));
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

TMK_TEST(rcu_test_add_null_mod_tbl) {
    TMK_ASSERT_EQUAL(RCU_E_NG, rcu_add_test_mod_tbl(NULL, NULL));
}

RCU_DEF_MOD_TBL(rcu_test_mod_017_mtbl1)
RCU_INC_MOD_FXT("rcu_test_mod_017_m1", NULL, NULL, NULL)
RCU_DEF_MOD_TBL_END

TMK_TEST(rcu_test_mod_on_null_reg) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_mod_tbl(NULL, rcu_test_mod_017_mtbl1));
}

//TODO revisit this
TMK_TEST(rcu_test_run_mod) {
    RCU_DEF_TEST_REG(r1)
    RCU_DEF_TEST_MOD(m1)
    TMK_ASSERT_NOT_NULL(m1 = rcu_get_mod("m1"));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_test_mod(m1));
}

TMK_TEST(rcu_test_run_mod_by_name) {
    RCU_DEF_TEST_REG(r1)
    RCU_DEF_TEST_MOD(m1)
    TMK_ASSERT_NOT_NULL(m1 = rcu_get_mod("m1"));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_test_mod_by_name("m1"));
}

TMK_TEST(rcu_test_run_mod_by_unknown_name) {
    RCU_DEF_TEST_REG(r1)
    RCU_DEF_TEST_MOD(m1)
    TMK_ASSERT_NOT_NULL(m1 = rcu_get_mod("m1"));
    TMK_ASSERT_EQUAL(RCU_E_NG, rcu_run_test_mod_by_name("unknown_module_name"));
}


