# RCUNIT User Guide

**Jerrico Gamis** <jecklgamis@gmail.com>

## Introduction

RCUNIT is a small framework for testing C programs. It uses non-local jumps to
emulate exceptions and handles program-terminating signals (e.g. SIGILL) during
test runs. rcunit allows creation of test fixtures, either per test or per test
group. rcunit is free, you can use it under the terms of the
[Apache License](https://www.apache.org/licenses/LICENSE-2.0).

### Introduction to Test Design Methods

rcunit is essentially a black box testing tool. Black box testing, also called
functional or behavioral testing, is a test method designed to test the
functional requirements of a system. This type of testing does not require
knowledge of the internal structure of the system under test.

White box testing, also called structural testing, is used for testing different
execution paths. This type of testing can find implementation problems of the
functional requirements.

### Test Suite

A test suite or test group is a collection of tests that share the same test
fixture. In rcunit, this is implemented using modules.

### Test Phase

An xUnit test is typically divided into four phases:

- **Setup** — Sets up the test fixture
- **Exercise** — Interacts with the system under test
- **Verify** — Asserts the expected outcome
- **Tear down** — Tears down the test fixture

---

## Getting Started

### Installing rcunit

```sh
git clone git@github.com/jecklgamis/rcunit.git
cd rcunit
./autogen.sh
make
sudo make install
```

This builds and installs the static library (`librcunit.a`) in `/usr/local/lib`
and the headers in `/usr/local/include`.

### Writing a Simple Test

```c
#include <stdio.h>
#include "rcunit.h"

RCU_TEST(test_func) {
    RCU_ASSERT(1);
}

int main(int argc, char *argv[]) {
    RCU_ADD_TEST("my_module", test_func);
    return rcu_run_tests();
}
```

### Using Assertion Macros

Assertion macros assert specific conditions within a test or fixture. An
assertion failure records the failure, aborts the test, and marks it as failed.

```c
RCU_TEST(test_func) {
    unsigned char data = 128;
    RCU_ASSERT_BIT_SET(data, 7); /* Asserts that bit 7 is set */
}
```

### Using Test Module

A module (also known as a test suite or test group) groups one or more related
tests. The macro RCU_ADD_TEST can be used to group test cases into module

A test always belongs to a module. `RCU_ADD_TEST` adds to a named module,
creating it if needed:

```c
RCU_ADD_TEST("some-module", test_func);
```

### Writing Test Module With Setup and Teardown

A module can have setup and teardown functions that run before and after all
tests within that module.

```c
#include <stdio.h>
#include "rcunit.h"

RCU_TEST(test_func) {
}

RCU_FIXTURE(setup) {
}

RCU_FIXTURE(teardown) {
}

RCU_FIXTURE(setup_all) {
}

RCU_FIXTURE(teardown_all) {
}

int main(int argc, char *argv[]) {
    RCU_SET_MODULE_FIXTURES("some-module", setup, teardown);
    RCU_ADD_TEST("some-module", test_func);
    RCU_SET_MODULE_FIXTURES_ALL("some-module", setup_all, teardown_all);
    return rcu_run_tests();
}
```

### Writing a Test Run Hook

Test run hooks are callbacks invoked before and after the full test run.

```c
#include <stdio.h>
#include "rcunit.h"

RCU_RUN_HOOK(run_hook) {
    int run_event = RCU_GET_RUN_EVENT_TYPE(param);
    if (run_event == RCU_TEST_RUN_STARTED) {
        puts("Test started");
    } else if (run_event == RCU_TEST_RUN_FINISHED) {
        puts("Test finished");
    }
}

int main(int argc, char *argv[]) {
    rcu_set_run_hook(run_hook);
    return rcu_run_tests();
}
```

### Test Reports

`rcunit` generates a test run report in plain text format: `rcunit_test_run_report.txt`.

---

## rcunit APIs

### API

```c
RCU_ADD_TEST("module", func)                       
RCU_SET_MODULE_FIXTURES(module, setup, teardown)    
RCU_SET_MODULE_FIXTURES_ALL(module, setup, teardown) 
```

```c
int rcu_init();                                    
int rcu_destroy();                                 
int rcu_run_tests();
void rcu_dump_test_registry();
int rcu_set_run_hook(rcu_generic_function hook);
```

### Helper Macros

```c

RCU_ADD_TEST_F("module", func, setup, teardown)     /* register a test with fixtures */
RCU_TEST(name) { ... }                            /* define a test function */
RCU_FIXTURE(name) { ... }       /* define a setup or teardown function */
RCU_RUN_HOOK(name) { ... }      /* define a run hook function */
```

### Assertion Macros

```c
RCU_ASSERT(cond)
RCU_ASSERT_TRUE(cond)
RCU_ASSERT_FALSE(cond)
RCU_ASSERT_NULL(ptr)
RCU_ASSERT_NOT_NULL(ptr)
RCU_ASSERT_EQUAL(expected, actual)
RCU_ASSERT_EQUAL_STRING(expected, actual)
RCU_ASSERT_SAME_BYTE_ARRAY(array1, array2, size)
RCU_ASSERT_NOT_SAME_BYTE_ARRAY(array1, array2, size)
RCU_ASSERT_BIT_SET(data, bit_pos)
RCU_ASSERT_BIT_NOT_SET(data, bit_pos)
RCU_FAIL(msg)
```

---

## rcunit Internals

### Exception Handling

rcunit uses non-local jumps (`setjmp`/`longjmp`) to simulate exceptions. This
mechanism is used when running tests and test fixtures.

### Signal Handling

rcunit catches signals thrown during a test run, allowing subsequent tests to
continue executing instead of terminating the process.

```c
#include <stdio.h>
#include "rcunit.h"

typedef struct {
    unsigned char *data;
    size_t size;
} buffer_t;

RCU_TEST(my_test) {
    buffer_t *buff = NULL;
    RCU_ASSERT_NULL(buff);
    fprintf(stdout, "buffer data address = %p", buff->data); /* triggers SIGSEGV */
}

int main(int argc, char *argv[]) {
    RCU_ADD_TEST("signal_tests", my_test);
    return rcu_run_tests();
}
```

Example output:

```
[INFO]  Test run started Sun Feb 10 20:22:32 2013
[ERROR] Caught illegal instruction in 0x100001650
[INFO]  Test run report generated : rcunit_test_run_report.txt
[INFO]  Test run finished Sun Feb 10 20:22:32 2013
[INFO]  Test Run Results: Passed : 0  Failed : 1
[INFO]  Test failed!
```

### Logging

rcunit includes built-in logging support via `RCU_LOG_INFO`, `RCU_LOG_WARN`,
`RCU_LOG_ERROR`, and `RCU_LOG_DEBUG` (enabled with `-DRCU_DEBUG`).

### Testing

rcunit uses its own framework to test itself. The test suite lives in the
`tests/` directory.

---

## Contributors

- Tomasz Kantecki (Intel, Ireland) — signal handling
- Abigail Gamis (SHFL, Australia) — user guide, API refinements
