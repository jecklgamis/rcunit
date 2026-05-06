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
