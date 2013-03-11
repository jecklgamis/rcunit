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

extern void test(void *param);
extern void setup(void *param);
extern void teardown(void *param);

TMK_TEST(rcu_test_add_test) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test(test));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_add_test_fxt) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_fxt(test, setup, teardown));
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_fxt(test, setup, teardown));
    TMK_ASSERT_EQUAL(2, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_add_test_to_mod) {
    rcu_module *mod = rcu_get_mod("mod");
    TMK_ASSERT_NOT_NULL(mod);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_to_mod(mod, test));
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_add_test_fxt_to_mod) {
    rcu_module *mod = rcu_get_mod("mod");
    TMK_ASSERT_NOT_NULL(mod);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_fxt_to_mod(mod, test, setup, teardown));
    TMK_ASSERT_EQUAL(2, rcu_get_nr_mods());
    TMK_ASSERT_EQUAL(1, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_add_test_to_default_mod) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_to_mod(rcu_get_default_mod(), test));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

TMK_TEST(rcu_test_add_test_fxt_to_default_mod) {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_fxt_to_mod(rcu_get_default_mod(), test, setup, teardown));
    TMK_ASSERT_EQUAL(1, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

RCU_DEF_FUNC_TBL(test_func_tbl)
RCU_INC_TEST(test)
RCU_DEF_FUNC_TBL_END

TMK_TEST(rcu_test_add_test_func_tbl) {
   TMK_ASSERT_EQUAL(RCU_E_OK, rcu_add_test_func_tbl(rcu_get_default_mod(), 
           test_func_tbl));
   TMK_ASSERT_EQUAL(1, rcu_get_nr_tests());
   TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}
