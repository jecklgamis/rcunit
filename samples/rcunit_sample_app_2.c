#include <rcunit.h>
#include <rcunit_sample_app_externs.h>

int sample_test_codes_2(){
RCU_DEF_TEST_REG(r1)        /** Define a test registry */
RCU_DEF_TEST_MOD(m1)        /** Define a test module */

    /** Step 1. Initialize the framework */
    if (rcu_init() == RCU_E_NG){
        return(-1);
    }
    /** Step 2. Create a test registry */
    if ((r1=rcu_cre_test_reg("r1",RCU_TRUE)) == RCU_NULL){
        rcu_destroy();
        return(-1);
    }
    /** Step 3. Add the test registry */
    if ((rcu_add_test_reg(r1)) == RCU_E_NG){
        /** this needs to be manually destroyed because it is not yet
         *  registered to the test machine
         */
        rcu_destroy_test_reg(r1);
        rcu_destroy();
        return(-1);
    }
    /** Step 4. Create a test module */
    if ((m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE)) == RCU_NULL){
        rcu_destroy();
        return(-1);
    }
    /** Step 5. Register the test module */
    if ((rcu_add_test_mod(r1,m1)) == RCU_E_NG){
        /** this needs to be manually destroyed because it is not yet
         *  linked to the test machine
         */
        rcu_destroy_test_mod(m1);
        rcu_destroy();
        return(-1);
    }
    /** Step 6. Register the test functions  */
    if ((rcu_add_test_func(m1,test_get_min,RCU_NULL,RCU_NULL, "test_get_min", RCU_TRUE))
        == RCU_E_NG){
        rcu_destroy();
        return(-1);
    }

    if ((rcu_add_test_func(m1,test_get_max,RCU_NULL,RCU_NULL, "test_get_max", RCU_TRUE))
        == RCU_E_NG){
        rcu_destroy();
        return(-1);
    }

    /** Optionally, dump the test database */
    rcu_dump_test_dbase();

    /** Step 7. Run the test machine */
    rcu_run_test_mach();

    /** Step 8. Destroy the framework */
    rcu_destroy();

    return(0);
}

int main(int argc,char **argv){
    fprintf(stdout,"Running sample_test_codes_2...\n");
    sample_test_codes_2();
    return(0);
}

