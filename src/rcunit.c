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

#include "rcunit.h"

struct rcu_test_engine the_test_engine;

extern struct rcu_test *rcu_search_test_func_by_name(struct rcu_module *module, const char *name);

extern struct rcu_test *rcu_search_test_func_entry(struct rcu_module *module, rcu_generic_function entry);

extern int rcu_init_test_engine(struct rcu_test_engine *engine);


RCU_API int rcu_run_test_engine() {
    struct rcu_test_engine *engine = &the_test_engine;
    char ts_buff[RCU_TSTAMP_BUFF_SIZE];
    rcu_init();
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO("Test run started %s", ts_buff);
    engine->run_level = RCU_RUN_LEVEL_ENGINE;
    rcu_run_tests_impl(engine);
    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);
    rcu_stop_engine(engine);
    rcu_gen_test_run_report(engine);
    RCU_LOG_INFO("Test run finished %s", ts_buff);
    RCU_LOG_INFO("Test Run Results: Passed : %d  Failed : %d", engine->nr_succ_test, engine->nr_failed_test);
    if (engine->nr_failed_reg == 0 && rcu_get_nr_tests() > 0) {
        RCU_LOG_INFO("Test successful!");
    } else {
        RCU_LOG_INFO("Test failed!");
    }
    rcu_destroy();
    return engine->nr_failed_reg > 0 ? RCU_E_NG : RCU_E_OK;
}

RCU_API int rcu_run_tests() {
    return rcu_run_test_engine();
}

void rcu_print_rcunit_info() {
    RCU_LOG_DEBUG("[ rcunit INFORMATION ]");
    RCU_LOG_DEBUG("rcunit version is %s (Built last %s %s)", RCU_VERSION_STRING, __DATE__, __TIME__);

#if RCU_ENABLE_MTRACE
    RCU_LOG_DEBUG("Memory leak checking is enabled");
#else
    RCU_LOG_DEBUG("Memory leak checking  is disabled");
#endif

#if RCU_ENABLE_MALLOC_ALIGNMENT
    RCU_LOG_DEBUG("Memory allocation aligned to %zu bytes", RCU_DEFAULT_MALLOC_ALIGNMENT);
#endif
    RCU_LOG_DEBUG("sizeof(char) is %zu bytes", sizeof(char));
    RCU_LOG_DEBUG("sizeof(rcu_short) is %zu bytes", sizeof(rcu_short));
    RCU_LOG_DEBUG("sizeof(int) is %zu bytes", sizeof(int));
    RCU_LOG_DEBUG("sizeof(rcu_long) is %zu bytes", sizeof(rcu_long));
    RCU_LOG_DEBUG("sizeof(rcu_float) is %zu bytes", sizeof(rcu_float));
    RCU_LOG_DEBUG("sizeof(rcu_double) is %zu bytes", sizeof(rcu_double));
    RCU_LOG_DEBUG("sizeof(rcu_long_long) is %zu bytes", sizeof(rcu_long_long));
    RCU_LOG_DEBUG("sizeof(rcu_long_double) is %zu bytes", sizeof(rcu_long_double));
    RCU_LOG_DEBUG("sizeof(void*) is %zu bytes", sizeof(void *));
    RCU_LOG_DEBUG("[ rcunit INFORMATION END ]");
}

RCU_API int rcu_init() {
    struct rcu_test_engine *engine = &the_test_engine;
    if (!rcu_is_engine_initialized(engine)) {
        RCU_LOG_DEBUG("Initializing rcunit");
        rcu_print_rcunit_info();
        rcu_init_test_engine(engine);
    }
    return RCU_E_OK;
}

RCU_API int rcu_destroy() {
    struct rcu_test_engine *engine = &the_test_engine;
    RCU_LOG_DEBUG("Destroying rcunit");
    if (rcu_is_engine_initialized(engine)) {
        rcu_destroy_test_engine(engine);
    }
    return RCU_E_OK;
}

int rcu_exit_hook(void *param) {
    RCU_LOG_FATAL("Terminating");
    rcu_destroy();
    return RCU_E_OK;
}

RCU_API int rcu_set_run_hook(rcu_generic_function run_hook) {
    struct rcu_test_engine *engine = &the_test_engine;
    rcu_init();
    engine->run_hook = run_hook;
    RCU_LOG_DEBUG("Test run hook set");
    return RCU_E_OK;
}

