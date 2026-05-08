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

#include "stdio.h"
#include "rcunit.h"

RCU_TEST(successful_test) {
        RCU_ASSERT(RCU_TRUE);
}

RCU_TEST(test_without_asserts) {
}

RCU_TEST(failed_test) {
        RCU_FAIL("failed_test");
}

RCU_SETUP(failed_setup) {
        RCU_FAIL("failed_setup");
}

RCU_TEARDOWN(failed_teardown) {
        RCU_FAIL("failed_teardown");
}

RCU_DEF_FUNC_TBL(test_table_with_all_kinds_test_failures)
RCU_INC_TEST(test_without_asserts)
RCU_INC_TEST(failed_test)
RCU_INC_TEST_FXT(successful_test, failed_setup, NULL
)
RCU_INC_TEST_FXT(successful_test, NULL, failed_teardown
)

RCU_DEF_FUNC_TBL_END

RCU_SETUP(module_setup) {
}

RCU_SETUP(module_teardown) {
}

RCU_SETUP(failed_module_setup) {
        RCU_FAIL("failed_module_setup");
}

RCU_SETUP(failed_module_teardown) {
        RCU_FAIL("failed_module_teardown");
}

RCU_DEF_FUNC_TBL(test_table_with_successful_tests)
RCU_INC_TEST(successful_test)

RCU_DEF_FUNC_TBL_END

RCU_DEF_MODULE_TBL(module_table)

RCU_INC_MODULE_FXT("m1", module_setup, module_teardown, test_table_with_successful_tests)

RCU_DEF_MODULE_TBL_END


int main(int argc, char **argv) {
    rcu_add_test_func_tbl(rcu_get_default_module(), test_table_with_all_kinds_test_failures);
    rcu_add_test_module_tbl(rcu_get_default_reg(), module_table);
    rcu_dump_test_dbase();
    return rcu_run_tests();
}

