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


RCU_TEST(test) {
        RCU_ASSERT(RCU_TRUE);
}

RCU_SETUP(setup) {
}

RCU_TEARDOWN(teardown) {
}

#include "rcunit_test_framework.c"
#include "rcunit_test_registry.c"
#include "rcunit_test_module.c"
#include "rcunit_test_function.c"
#include "rcunit_test_assertions.c"
#include "rcunit_test_mtrace.c"
#include "rcunit_test_exceptions.c"

TMK_TEST_FUNCTION_TABLE_START(test_function_table)
/* test registry */
                TMK_INCLUDE_TEST(rcu_test_get_default_reg)
                TMK_INCLUDE_TEST(rcu_test_reserved_reg_name)
                TMK_INCLUDE_TEST(rcu_test_null_reg_name)
                TMK_INCLUDE_TEST(rcu_test_create_reg)
                TMK_INCLUDE_TEST(rcu_test_destroy_reg)

/* test module */
                TMK_INCLUDE_TEST(rcu_test_get_mod)
                TMK_INCLUDE_TEST(rcu_test_get_default_mod)
                TMK_INCLUDE_TEST(rcu_test_get_default_mod_by_name)
                TMK_INCLUDE_TEST(rcu_test_create_default_mod)
                TMK_INCLUDE_TEST(rcu_test_mod_fixture_invocation)
                TMK_INCLUDE_TEST(rcu_test_create_multiple_mods)
                TMK_INCLUDE_TEST(rcu_test_create_and_destroy_mod)
                TMK_INCLUDE_TEST(rcu_test_add_mod_tbl)
                TMK_INCLUDE_TEST(rcu_test_add_null_mod_tbl)
                TMK_INCLUDE_TEST(rcu_test_mod_on_null_reg)
                TMK_INCLUDE_TEST(rcu_test_run_mod)
//TMK_INCLUDE_TEST(rcu_test_run_mod_by_name)
//TMK_INCLUDE_TEST(rcu_test_run_mod_by_unknown_name)

/* test function */
                TMK_INCLUDE_TEST(rcu_test_add_test)
                TMK_INCLUDE_TEST(rcu_test_add_test_fxt)
                TMK_INCLUDE_TEST(rcu_test_add_test_to_mod)
                TMK_INCLUDE_TEST(rcu_test_add_test_fxt_to_mod)
                TMK_INCLUDE_TEST(rcu_test_add_test_to_default_mod)
                TMK_INCLUDE_TEST(rcu_test_add_test_func_tbl)

/* run hooks */
                TMK_INCLUDE_TEST(rcu_test_run_hooks)

/* assertion macro tests */
                TMK_INCLUDE_TEST(rcu_test_assert_single_evaluation)
                TMK_INCLUDE_TEST(rcu_test_bit_assertions)
                TMK_INCLUDE_TEST(rcu_test_byte_assertions)
                TMK_INCLUDE_TEST(rcu_test_float_assertions)
                TMK_INCLUDE_TEST(rcu_test_pointer_assertions)

/* memory allocation tracing tests */
                TMK_INCLUDE_TEST(rcu_test_mtrace)

/* exception tests */
                TMK_INCLUDE_TEST(rcu_test_exception_handling)
                TMK_INCLUDE_TEST(rcu_test_exception_catch_sigfpe)
                TMK_INCLUDE_TEST(rcu_test_exception_catch_sigill)
                TMK_INCLUDE_TEST(rcu_test_exception_catch_sigsev)
                TMK_INCLUDE_TEST(rcu_test_exception_catch_sigbus)

TMK_TEST_FUNCTION_TABLE_END

void tmk_setup() {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_init());
    TMK_ASSERT_EQUAL(0, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods());
}

void tmk_teardown() {
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_destroy());
    TMK_ASSERT_EQUAL(0, rcu_get_nr_tests());
    TMK_ASSERT_EQUAL(1, rcu_get_nr_mods());
    TMK_ASSERT_FALSE(rcu_has_mem_leak());
}

int main(int argc, char **argv) {
    return tmk_run_tests(test_function_table, tmk_setup, tmk_teardown);
}

