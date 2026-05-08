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
    return(0);
}

