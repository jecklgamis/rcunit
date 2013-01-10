#include <rcunit.h>

/** This is a "malloc" wrapper */
void *sample_test_codes_5_malloc(int size){
void *ptr;
    ptr = malloc(size);
    if (ptr != NULL){
        RCU_TRACE_ALLOC(ptr,size);
    }
    return(ptr);
}

/** This is a "free" wrapper */
void sample_test_codes_5_free(void *ptr){
    if (ptr != NULL){
        RCU_TRACE_FREE(ptr);
    }
    free(ptr);
}

int sample_test_codes_5(){
int *ptr1,*ptr2,*ptr3;
    /** Initialize the framework */
    if (rcu_init() == RCU_E_NG){
        fprintf(stdout,"Unable to initialize RCUNIT framework\n");
        return(-1);
    }

    /** Allocate three pointers */
    ptr1 = (int*)sample_test_codes_5_malloc(sizeof(int));
    ptr2 = (int*)sample_test_codes_5_malloc(sizeof(int));
    ptr3 = (int*)sample_test_codes_5_malloc(sizeof(int));

    /** Deallocate ptr1 and ptr3 pointers */
    sample_test_codes_5_free(ptr1);
    sample_test_codes_5_free(ptr3);

    /** Allocate and assign to ptr2 again. There is no more reference
        to the previously allocated address and is considered a memory
        leak.
     */
    ptr2 = (int*)sample_test_codes_5_malloc(sizeof(int));

    /** Deallocate ptr2 */
    sample_test_codes_5_free(ptr2);

    /** Finally, check if we have a memory leak */
    RCU_CHECK_MEMORY_LEAK();

    /** Destroy the framework */
    rcu_destroy();

    return(0);
}

int main(int argc,char **argv){
    fprintf(stdout,"Running sample_test_codes_5...\n");
    sample_test_codes_5();
    return(0);
}

