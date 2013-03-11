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

/* Verify that assertion statements evaluate the condition only once 
 */
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
    RCU_ASSERT_EQUAL_STRING("string", return_string());
    TMK_ASSERT_EQUAL(1, nr_invocations);

    nr_invocations = 0;
    RCU_ASSERT_NULL(return_null_ptr());
    TMK_ASSERT_EQUAL(1, nr_invocations);
    
    nr_invocations = 0;
    RCU_ASSERT_NOT_NULL(return_string());
    TMK_ASSERT_EQUAL(1, nr_invocations);
}

RCU_TEST(rcu_bit_assertion_tests){
    rcu_u2 data;
    
    data = 1;
    RCU_ASSERT_BIT_SET(data, 0);

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
}

TMK_TEST(rcu_test_bit_assertions) {
    rcu_add_test(rcu_bit_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

RCU_TEST(rcu_byte_assertion_tests){
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
    
    RCU_ASSERT_SAME_BYTE_ARRAY(expected, actual, 4);
    
    actual[0] = 0xde;
    actual[1] = 0xad;
    actual[2] = 0xbe;
    actual[3] = 0xef;

    RCU_ASSERT_NOT_SAME_BYTE_ARRAY(expected, actual, 4);
}

TMK_TEST(rcu_test_byte_assertions) {
    rcu_add_test(rcu_byte_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

RCU_TEST(rcu_float_assertion_tests) {
    RCU_ASSERT_EQUAL_FLOATS(1.00001, 1.00001);
    RCU_ASSERT_NOT_EQUAL_FLOATS(1.00001, 1.00000);
}

TMK_TEST(rcu_test_float_assertions) {
    rcu_add_test(rcu_float_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}

RCU_TEST(rcu_pointer_assertion_tests) {
    int x;
    int *y = &x;
    RCU_ASSERT_EQUAL_PTRS(&x, y);
}

TMK_TEST(rcu_test_pointer_assertions) {
    rcu_add_test(rcu_pointer_assertion_tests);
    TMK_ASSERT_EQUAL(RCU_E_OK, rcu_run_tests());
}




