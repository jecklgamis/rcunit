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

#ifndef RCUNIT_API_H
#define RCUNIT_API_H

#include "rcunit_types.h"
#include "rcunit_helpers.h"

/* Main APIs */
RCU_API int rcu_init();
RCU_API int rcu_destroy();
RCU_API int rcu_run_tests();
RCU_API void rcu_dump_test_registry();
RCU_API int rcu_set_run_hook(rcu_generic_function hook);

/* Test function APIs */
RCU_API int rcu_add_test(rcu_generic_function test);
RCU_API int rcu_add_test_to_module(struct rcu_module *module, rcu_generic_function test);

/* Test module APIs */
RCU_API struct rcu_module *rcu_get_module(const char *name);
RCU_API struct rcu_module *rcu_get_default_module();
RCU_API struct rcu_module *rcu_create_test_module(const char *name, rcu_generic_function init, rcu_generic_function destroy);
RCU_API int rcu_add_test_module(struct rcu_module *module);
RCU_API int rcu_destroy_test_module(struct rcu_module *module);
RCU_API int rcu_run_test_module(struct rcu_module *module);
RCU_API int rcu_run_test_module_by_name(const char *name);
RCU_API void rcu_set_module_fixture(struct rcu_module *module, rcu_generic_function setup,
                             rcu_generic_function teardown);
RCU_API void rcu_set_module_fixture_all(struct rcu_module *module, rcu_generic_function setup,
    rcu_generic_function teardown);

/* Test registry APIs */
RCU_API struct rcu_registry *rcu_get_default_reg();

/* Diagnostic APIs */
RCU_API int rcu_get_nr_tests();
RCU_API int rcu_get_nr_mods();
RCU_API int rcu_has_mem_leak();

#define RCU_TEST(name) void name(void *param)
#define RCU_FIXTURE(name) void name(void *param)
#define RCU_RUN_HOOK(name) void name(void *param)

#define RCU_ADD_TEST(module, func) \
    do { \
        rcu_add_test_func(rcu_get_module(module), func, NULL, NULL, #func); \
    } while(0);

#define RCU_SET_MODULE_FIXTURES(module, setup, teardown) \
    do { \
        rcu_init(); \
        rcu_set_module_fixture(rcu_get_module(module), setup, teardown); \
    } while(0);

#define RCU_SET_MODULE_FIXTURES_ALL(module, setup, teardown) \
    do { \
        rcu_init(); \
        rcu_set_module_fixture_all(rcu_get_module(module), setup, teardown); \
    } while(0);


#endif /* RCUNIT_API_H */



