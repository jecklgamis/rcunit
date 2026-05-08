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

#ifndef RCUNIT_API_H
#define RCUNIT_API_H

#include "rcunit_types.h"
#include "rcunit_helpers.h"

/* Test function APIs */
RCU_API int rcu_add_test(rcu_generic_function test);
#define RCU_ADD_TEST(fn) rcu_add_test_func(NULL, fn, NULL, NULL, #fn)

RCU_API int rcu_add_test_to_module(struct rcu_module *module, rcu_generic_function test);

RCU_API int rcu_add_test_fixture(rcu_generic_function test, rcu_generic_function setup, rcu_generic_function teardown);

RCU_API int rcu_add_test_fixture_to_module(struct rcu_module *module, rcu_generic_function test, rcu_generic_function setup,
                                    rcu_generic_function teardown);

/* Test module interfaces */
RCU_API struct rcu_module *rcu_get_module(const char *name);

RCU_API struct rcu_module *rcu_get_default_module();

RCU_API void rcu_set_module_fixture(struct rcu_module *module, rcu_generic_function setup,
                             rcu_generic_function teardown);

/* Test registry APIs */
RCU_API struct rcu_registry *rcu_get_default_reg();

RCU_API int rcu_add_module_to_reg(struct rcu_registry *reg, struct rcu_module *module);

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
struct rcu_test_function_entry name[] = {

#define RCU_INC_TEST(name) \
    { #name, name, NULL, NULL },

#define RCU_INC_TEST_FXT(test, setup, teardown) \
    { #test, test, setup, teardown },

#define RCU_DEF_FUNC_TBL_END \
    { NULL, NULL, NULL, NULL }};

/* Test module table APIs */
#define RCU_DEF_MODULE_TBL(name) \
    struct rcu_module_entry name[] = {

#define RCU_INC_MODULE(name, tbl) \
    { name, NULL, NULL, tbl },

#define RCU_INC_MODULE_FXT(name, setup, teardown, tbl) \
    { name, setup, teardown, tbl},

#define RCU_DEF_MODULE_TBL_END \
    { NULL, NULL, NULL, NULL }};

/* For Backward compatibility */
#define RCU_DEF_TEST_MODULE(name) \
    struct rcu_module *name;

#define RCU_DEF_INIT_FUNC(name) \
    RCU_SETUP(name)

#define RCU_DEF_DESTROY_FUNC(name) \
    RCU_TEARDOWN(name)

extern RCU_API int rcu_add_test_module(struct rcu_module *module);

extern RCU_API int rcu_destroy_test_module(struct rcu_module *module);

extern RCU_API int rcu_get_err();

extern RCU_API int rcu_has_mem_leak();

extern RCU_API int rcu_have_asserts();

extern RCU_API int rcu_init();

extern RCU_API int rcu_run_test_engine();

extern RCU_API int rcu_run_test_module_by_name(const char *name);

extern RCU_API int rcu_run_tests();

extern RCU_API int rcu_set_assert_hook(rcu_generic_function assert_hook);

extern RCU_API int rcu_set_run_hook(rcu_generic_function run_hook);

extern RCU_API struct rcu_exception *rcu_lookup_excp_by_id(rcu_exception_id id);

extern RCU_API struct rcu_module *rcu_get_default_module();

extern RCU_API struct rcu_module *rcu_get_module(const char *name);


extern RCU_API struct rcu_thread *rcu_get_thread(const char *name, rcu_thread_routine routine, void *arg);

extern RCU_API void *rcu_malloc(size_t size);

extern RCU_API void rcu_destroy_threads();

extern RCU_API void rcu_dump_asserts();

extern RCU_API void rcu_dump_test_dbase();

extern RCU_API void rcu_free(void *addr);

extern RCU_API void rcu_init_threads();

extern RCU_API int rcu_add_test_func_tbl(struct rcu_module *module, struct rcu_test_function_entry *func_tbl);

extern RCU_API int rcu_add_test_module_tbl(struct rcu_registry *reg, struct rcu_module_entry *mod_tbl);

extern RCU_API int rcu_run_test_module(struct rcu_module *module);

extern RCU_API void rcu_dump_test_reg(struct rcu_registry *reg);
extern int rcu_destroy_test_engine(struct rcu_test_engine *engine);


#endif /* RCUNIT_API_H */



