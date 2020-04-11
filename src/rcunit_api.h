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

#ifndef RCUNIT_API_H
#define RCUNIT_API_H

#include "rcunit_types.h"
#include "rcunit_helpers.h"

/* Test function APIs */
RCU_API int rcu_add_test(rcu_generic_function test);

RCU_API int rcu_add_test_to_mod(rcu_module *mod, rcu_generic_function test);

RCU_API int rcu_add_test_fxt(rcu_generic_function test, rcu_generic_function setup, rcu_generic_function teardown);

RCU_API int rcu_add_test_fxt_to_mod(rcu_module *mod, rcu_generic_function test, rcu_generic_function setup,
                                    rcu_generic_function teardown);

/* Test module interfaces */
RCU_API rcu_module *rcu_get_mod(const char *name);

RCU_API rcu_module *rcu_get_default_mod();

RCU_API void rcu_set_mod_fxt(rcu_module *mod, rcu_generic_function setup,
                             rcu_generic_function teardown);

/* Test registry APIs */
RCU_API rcu_registry *rcu_get_reg(const char *name);

RCU_API rcu_registry *rcu_get_default_reg();

RCU_API int rcu_add_mod_to_reg(rcu_registry *reg, rcu_module *mod);

/* Main APIs*/
RCU_API int rcu_init();

RCU_API int rcu_destroy();

RCU_API int rcu_run_tests();

RCU_API void rcu_dump_test_dbase();

RCU_API int rcu_set_run_hook(rcu_generic_function hook);

#define RCU_DEF_GENERIC_FUNC(name) void name(void *param)
#define RCU_TEST(name) RCU_DEF_GENERIC_FUNC(name)
#define RCU_SETUP(name) RCU_DEF_GENERIC_FUNC(name)
#define RCU_TEARDOWN(name) RCU_DEF_GENERIC_FUNC(name)
#define RCU_RUN_HOOK(name) RCU_DEF_GENERIC_FUNC(name)
#define RCU_RUN_HOOK(name) RCU_DEF_GENERIC_FUNC(name)
#define RCU_ASSERT_HOOK(name) RCU_DEF_GENERIC_FUNC(name)

/* Test function table APIs */
#define RCU_DEF_FUNC_TBL(name) \
rcu_test_function_entry name[] = {

#define RCU_INC_TEST(name) \
    { #name, name, NULL, NULL },

#define RCU_INC_TEST_FXT(test, setup, teardown) \
    { #test, test, setup, teardown },

#define RCU_DEF_FUNC_TBL_END \
    { NULL, NULL, NULL, NULL }};

/* Test module table APIs */
#define RCU_DEF_MOD_TBL(name) \
    rcu_module_entry name[] = {

#define RCU_INC_MOD(name, tbl) \
    { name, NULL, NULL, tbl },

#define RCU_INC_MOD_FXT(name, setup, teardown, tbl) \
    { name, setup, teardown, tbl},

#define RCU_DEF_MOD_TBL_END \
    { NULL, NULL, NULL, NULL }};

/* For Backward compatibility */
#define RCU_DEF_TEST_MOD(name) \
    rcu_module *name;

#define RCU_DEF_TEST_REG(name) \
    rcu_registry *name;

#define RCU_DEF_INIT_FUNC(name) \
    RCU_SETUP(name)

#define RCU_DEF_DESTROY_FUNC(name) \
    RCU_TEARDOWN(name)

RCU_API rcu_registry *rcu_cre_test_reg(const char *name);

#endif /* RCUNIT_API_H */



