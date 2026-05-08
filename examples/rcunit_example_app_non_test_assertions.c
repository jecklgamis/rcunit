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
#include <rcunit.h>

void sample_test_codes_7_non_test_func(){
int x=100;
    RCU_ASSERT(x == 100);
    RCU_ASSERT(x == 101);
    RCU_ASSERT(x == 102);
    RCU_ASSERT(x == 103);
    RCU_ASSERT(x == 104);
}

RCU_ASSERT_HOOK(sample_test_codes_7_assert_hook){
rcu_failure_record *fail_rec;
    fail_rec = (rcu_failure_record*)param;
    fprintf(stdout,"Assertion failure : %s\n",fail_rec->info);
}

int sample_test_codes_7(){

    /** Step 1. Initialize the framework */
    if (rcu_init() == RCU_E_NG){
        return(-1);
    }
    /** Step 2. Register a non-test run assertion hook */
    if (rcu_set_assert_hook(sample_test_codes_7_assert_hook) == RCU_E_NG){
        rcu_destroy();
        return(-1);
    }
    /** Step 3. Call non-test functions with assertion statements */
    sample_test_codes_7_non_test_func();

    /** Step 4.Destroy the framework */
    rcu_destroy();

    return(0);
}

int main(int argc,char **argv){
    fprintf(stdout,"Running sample_test_codes_7...\n");
    sample_test_codes_7();
    return(EXIT_SUCCESS);
}

