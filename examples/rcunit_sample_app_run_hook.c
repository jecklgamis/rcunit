#include <stdio.h>
#include "rcunit.h"

RCU_RUN_HOOK(run_hook) {
    int run_event;
    run_event = RCU_GET_RUN_EVT_TYPE(param);
    if (run_event == RCU_TEST_RUN_STARTED) {
        puts("Test started");
    } else if (run_event == RCU_TEST_RUN_FINISHED) {
        puts("Test finished");
    }
}

int main(int argc, char *argv[]) {
    rcu_init();
    rcu_set_run_hook(run_hook);
    rcu_run_tests();
    return EXIT_SUCCESS;
}