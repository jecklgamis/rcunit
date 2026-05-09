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

static int nr_invocations = 0;

int return_1() {
    nr_invocations++;
    return 1;
}

const char *return_string() {
    nr_invocations++;
    return "string";
}

const void *return_null_ptr() {
    nr_invocations++;
    return NULL;
}

/* Verify that assertion statements evaluate the condition only once */
TMK_TEST(rcu_test_assert_single_evaluation) {
    nr_invocations = 0;
    RCU_ASSERT(return_1());
    TMK_ASSERT_EQUAL(1, nr_invocations);

    nr_invocations = 0;
    RCU_ASSERT_TRUE(return_1());
    TMK_ASSERT_EQUAL(1, nr_invocations);

    nr_invocations = 0;
    RCU_ASSERT_FALSE(!return_1());
    TMK_ASSERT_EQUAL(1, nr_invocations);

    nr_invocations = 0;
    RCU_ASSERT_EQUAL(1, return_1());
    TMK_ASSERT_EQUAL(1, nr_invocations);

    nr_invocations = 0;
    RCU_ASSERT_EQUAL_STR("string", return_string());
    TMK_ASSERT_EQUAL(1, nr_invocations);

    nr_invocations = 0;
    RCU_ASSERT_NULL(return_null_ptr());
    TMK_ASSERT_EQUAL(1, nr_invocations);

    nr_invocations = 0;
    RCU_ASSERT_NOT_NULL(return_string());
    TMK_ASSERT_EQUAL(1, nr_invocations);
}

RCU_TEST(rcu_bit_assertion_tests) {
    rcu_u2 data;

    data = 1;
    RCU_ASSERT_BIT_SET(data, 0);
    RCU_ASSERT_BIT_NOT_SET(data, 1);

    data = 128;
    RCU_ASSERT_BIT_SET(data, 7);

    data = 256;
    RCU_ASSERT_BIT_SET(data, 8);

    data = 32768;
    RCU_ASSERT_BIT_SET(data, 15);

    data = 32768 + 256 + 128 + 1;
    RCU_ASSERT_BIT_SET(data, 0);
    RCU_ASSERT_BIT_SET(data, 7);
    RCU_ASSERT_BIT_SET(data, 8);
    RCU_ASSERT_BIT_SET(data, 15);

    RCU_ASSERT_BITS_SET(data, 0x8181);
    RCU_ASSERT_BITS_NOT_SET(data, 0x0002);
}

TMK_TEST(rcu_test_bit_assertions) {
    rcu_add_test(rcu_bit_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

RCU_TEST(rcu_integer_assertion_tests) {
    RCU_ASSERT_NOT_EQUAL(1, 2);
    RCU_ASSERT_GT(5, 3);
    RCU_ASSERT_LT(3, 5);
    RCU_ASSERT_GE(5, 5);
    RCU_ASSERT_GE(6, 5);
    RCU_ASSERT_LE(5, 5);
    RCU_ASSERT_LE(4, 5);
    RCU_ASSERT_IN_RANGE(5, 1, 10);
    RCU_ASSERT_IN_RANGE(1, 1, 10);
    RCU_ASSERT_IN_RANGE(10, 1, 10);
}

TMK_TEST(rcu_test_integer_assertions) {
    rcu_add_test(rcu_integer_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

RCU_TEST(rcu_byte_assertion_tests) {
    rcu_u1 expected[4];

    expected[0] = 0xde;
    expected[1] = 0xad;
    expected[2] = 0xc0;
    expected[3] = 0xde;

    rcu_u1 actual[4];
    actual[0] = 0xde;
    actual[1] = 0xad;
    actual[2] = 0xc0;
    actual[3] = 0xde;

    RCU_ASSERT_EQUAL_MEM(expected, actual, 4);

    actual[0] = 0xde;
    actual[1] = 0xad;
    actual[2] = 0xbe;
    actual[3] = 0xef;

    RCU_ASSERT_NOT_EQUAL_MEM(expected, actual, 4);

    rcu_u1 zeros[4] = {0, 0, 0, 0};
    RCU_ASSERT_ZERO_MEM(zeros, 4);

    long arr_expected[3] = {1, 2, 3};
    long arr_actual[3]   = {1, 2, 3};
    RCU_ASSERT_EQUAL_ARRAY(arr_expected, arr_actual, 3);
}

TMK_TEST(rcu_test_byte_assertions) {
    rcu_add_test(rcu_byte_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

RCU_TEST(rcu_float_assertion_tests) {
    RCU_ASSERT_EQUAL_FLOAT(1.00001, 1.00001);
    RCU_ASSERT_NOT_EQUAL_FLOAT(1.00001, 1.00000);
    RCU_ASSERT_NEAR(1.0, 1.0001, 0.001);
}

TMK_TEST(rcu_test_float_assertions) {
    rcu_add_test(rcu_float_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

RCU_TEST(rcu_pointer_assertion_tests) {
    int x;
    int y;
    int *px = &x;
    RCU_ASSERT_EQUAL_PTR(&x, px);
    RCU_ASSERT_NOT_EQUAL_PTR(&x, &y);
}

TMK_TEST(rcu_test_pointer_assertions) {
    rcu_add_test(rcu_pointer_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

RCU_TEST(rcu_string_assertion_tests) {
    RCU_ASSERT_EQUAL_STR("hello", "hello");
    RCU_ASSERT_NOT_EQUAL_STR("hello", "world");
    RCU_ASSERT_STR_CONTAINS("hello world", "world");
    RCU_ASSERT_STR_EMPTY("");
    RCU_ASSERT_STR_NOT_EMPTY("hello");
}

TMK_TEST(rcu_test_string_assertions) {
    rcu_add_test(rcu_string_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}
