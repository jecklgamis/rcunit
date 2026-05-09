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

static int invoked = 0;

RCU_DEF_GENERIC_FUNC(run_hook) {
    int run_event;
    run_event = RCU_GET_RUN_EVT_TYPE(param);
    if (run_event == RCU_TEST_RUN_STARTED) {
        invoked++;
    } else if (run_event == RCU_TEST_RUN_FINISHED) {
        invoked++;
    }
}

TMK_TEST(rcu_test_run_hooks) {
    rcu_set_run_hook(run_hook);
    rcu_run_tests();
    TMK_ASSERT_EQUAL(2, invoked);
}
