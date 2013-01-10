#include <rcunit.h>
#include <rcunit_sample_app_externs.h>

int sample_test_codes_1(){

   /** Step 1. Initialize the framework */
    if (rcu_init() == RCU_E_NG){
        return(-1);
    }

    /** Step 2. Register test functions to the default test module */
    if ((rcu_add_test_func(RCU_DEFAULT_MODULE,test_get_min,RCU_NULL,RCU_NULL,
        "test_get_min", RCU_TRUE)) == RCU_E_NG){
        rcu_destroy();
        return(-1);
    }

    if ((rcu_add_test_func(RCU_DEFAULT_MODULE,test_get_max,RCU_NULL,RCU_NULL,
        "test_get_max", RCU_TRUE)) == RCU_E_NG){
        rcu_destroy();
        return(-1);
    }

    /** Optionally, dump the test database */
    rcu_dump_test_dbase();

    /** Step 3. Run the test machine */
    rcu_run_test_mach();

    /** Step 4.Destroy the framework */
    rcu_destroy();

    return(0);
}

int main(int argc,char **argv){
    fprintf(stdout,"Running sample_test_codes_1...\n");
    sample_test_codes_1();
    return(0);
}

