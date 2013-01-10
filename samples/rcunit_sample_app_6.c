#include <rcunit.h>

/** This is dummy function that accepts a pointer to an integer. If the
 *  pointer is null, a null pointer exception is thrown. Else the
 *  integer parameter is set with a value 0xdabababe.
 */

void sample_test_codes_6_1(int *iptr){
    fprintf(stdout,"sample_test_codes_6_1 : invoked!\n");
    if (iptr == NULL){
        fprintf(stdout,"sample_test_codes_6_1: Throwing exception\n");
        RCU_THROW(RCU_GET_EXCP(RCU_EXCP_NULLPOINTER));
        /** This point will not be reached.  The execution will jump to
            the exception handler in the code that directly or
            indirectly called this function
         */
    }
    *iptr = 0xdababe;
}

/** This is a dummy function that accepts an integer. If the integer is
 *  0 the sample_test_codes_6_1 function is called with a null
 *  parameter. If the invocation is successful, the value set by
 *  sample_test_codes_6_1 in its output parameter is returned. Else an
 *  invalid parameter exception is thrown.
 */

int sample_test_codes_6_2(int flag){
int *temp_ptr = NULL;
int val = 0;
    fprintf(stdout,"sample_test_codes_6_2 : invoked!\n");
    temp_ptr = (flag == 0)?NULL:&val;
    RCU_TRY {
        sample_test_codes_6_1(temp_ptr);
    }RCU_CATCH(e) {
        fprintf(stdout,"sample_test_codes_6_2 : sample_test_codes_6_1 threw an exception : %s\n",e->name);
        fprintf(stdout,"sample_test_codes_6_2 : Throwing exception\n");
        RCU_THROW(RCU_GET_EXCP(RCU_EXCP_INVPARAM));
    }RCU_END_CATCH
    fprintf(stdout,"sample_test_codes_6_2 : sample_test_codes_6_1 is successful :0x%x\n",val);
    return(val);
}

int sample_test_codes_6(){
int flag=1;
int retcode;
    fprintf(stdout,"sample_test_codes_6 : invoked!\n");
    /** Initialize the framework */
    if (rcu_init() == RCU_E_NG){
        fprintf(stdout,"Unable to initialize RCUNIT framework\n");
        return(-1);
    }
    RCU_TRY {
        retcode = sample_test_codes_6_2(flag);
    }RCU_CATCH(e) {
        fprintf(stdout,"sample_test_codes_6 : sample_test_codes_6_2 threw an exception : %s\n",e->name);
        /** Note that the return value of a function that threw an
            exception is not known and must not be used.
         */
    }RCU_END_CATCH
    fprintf(stdout,"sample_test_codes_6 : sample_test_codes_6_2 returned : 0x%x\n",retcode);
    /** Destroy the framework */
    rcu_destroy();
    return(0);
}

int main(int argc,char **argv){
    fprintf(stdout,"Running sample_test_codes_6...\n");
    sample_test_codes_6();
    return(0);
}

