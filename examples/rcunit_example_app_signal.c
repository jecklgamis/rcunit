#include <stdio.h>
#include "rcunit.h"

/* A buffer */
typedef struct {
    unsigned char *data;
    size_t size;
} buffer_t;

RCU_TEST(my_test) {
    buffer_t *buff = NULL;
    RCU_ASSERT_NULL(buff);
    fprintf(stdout, "buffer data address = %p, size = %lu\n", buff->data, buff->size);
}

int main(int argc, char *argv[]) {
    rcu_add_test(my_test);
    return rcu_run_tests();
}