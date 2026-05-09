# rcunit User Guide

**Jerrico Gamis** <jecklgamis@gmail.com>

## Introduction

rcunit is a small framework for testing C programs. It uses non-local jumps to
emulate exceptions and handles program-terminating signals (e.g. SIGSEGV,
SIGILL) during test runs. rcunit supports test fixtures either per test or per
test module, and is free to use under the
[Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).

### Test Suite

A test suite or test group is a collection of tests that share the same test
fixture. In rcunit, this is implemented using modules.

### Test Phases

An xUnit test is typically divided into four phases:

- **Setup** — Sets up the test fixture
- **Exercise** — Interacts with the system under test
- **Verify** — Asserts the expected outcome
- **Tear down** — Tears down the test fixture

---

## Getting Started

### Installing rcunit

```sh
git clone https://github.com/jecklgamis/rcunit.git
cd rcunit
./autogen.sh
./configure
make
sudo make install
```

This builds and installs the static library (`librcunit.a`) in `/usr/local/lib`
and the headers in `/usr/local/include`. To change the install prefix:

```sh
./configure --prefix=/some/dir
```

### Writing a Simple Test

```c
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
    RCU_ASSERT_BIT_SET(data, 7);
}
```

### Using Test Modules

A module groups one or more related tests. `RCU_ADD_TEST` adds a test to a
named module, creating the module if it does not already exist:

```c
RCU_ADD_TEST("some-module", test_func);
```

### Writing a Test Module With Fixtures

A module can have per-test and per-suite fixture functions.
`RCU_SET_MODULE_FIXTURES` sets functions that run before and after each test.
`RCU_SET_MODULE_FIXTURES_ALL` sets functions that run once before and after all
tests in the module.

```c
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
    RCU_SET_MODULE_FIXTURES_ALL("some-module", setup_all, teardown_all);
    RCU_ADD_TEST("some-module", test_func);
    return rcu_run_tests();
}
```

### Writing a Test Run Hook

Test run hooks are callbacks invoked before and after the full test run.

```c
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

rcunit generates a plain-text test run report: `rcunit_test_run_report.txt`.

---

## API Reference

### Macros

```c
RCU_TEST(name)                                       /* define a test function */
RCU_FIXTURE(name)                                    /* define a fixture function */
RCU_RUN_HOOK(name)                                   /* define a run hook function */

RCU_ADD_TEST(module, func)                           /* add test to a named module */
RCU_SET_MODULE_FIXTURES(module, setup, teardown)     /* per-test fixtures */
RCU_SET_MODULE_FIXTURES_ALL(module, setup, teardown) /* per-suite fixtures */
```

### Functions

```c
int rcu_init();
int rcu_destroy();
int rcu_run_tests();
void rcu_dump_test_registry();
int rcu_set_run_hook(rcu_generic_function hook);
```

### Assertion Macros

#### General

| Macro | Description |
|-------|-------------|
| `RCU_ASSERT(cond)` | Fails if `cond` is false |
| `RCU_ASSERT_TRUE(cond)` | Fails if `cond` is not true |
| `RCU_ASSERT_FALSE(cond)` | Fails if `cond` is not false |
| `RCU_FAIL(msg)` | Unconditionally fails with message |

#### Pointers

| Macro | Description |
|-------|-------------|
| `RCU_ASSERT_NULL(ptr)` | Fails if `ptr` is not null |
| `RCU_ASSERT_NOT_NULL(ptr)` | Fails if `ptr` is null |
| `RCU_ASSERT_EQUAL_PTR(expected, actual)` | Fails if pointers are not equal |
| `RCU_ASSERT_NOT_EQUAL_PTR(expected, actual)` | Fails if pointers are equal |

#### Integers

| Macro | Description |
|-------|-------------|
| `RCU_ASSERT_EQUAL(expected, actual)` | Fails if values are not equal |
| `RCU_ASSERT_NOT_EQUAL(expected, actual)` | Fails if values are equal |
| `RCU_ASSERT_GT(a, b)` | Fails if `a <= b` |
| `RCU_ASSERT_LT(a, b)` | Fails if `a >= b` |
| `RCU_ASSERT_GE(a, b)` | Fails if `a < b` |
| `RCU_ASSERT_LE(a, b)` | Fails if `a > b` |
| `RCU_ASSERT_IN_RANGE(val, min, max)` | Fails if `val` is not in `[min, max]` |

#### Floating Point

| Macro | Description |
|-------|-------------|
| `RCU_ASSERT_EQUAL_FLOAT(expected, actual)` | Fails if values are not exactly equal |
| `RCU_ASSERT_NOT_EQUAL_FLOAT(expected, actual)` | Fails if values are exactly equal |
| `RCU_ASSERT_NEAR(expected, actual, delta)` | Fails if `|expected - actual| > delta` |

#### Strings

| Macro | Description |
|-------|-------------|
| `RCU_ASSERT_EQUAL_STR(expected, actual)` | Fails if strings are not equal |
| `RCU_ASSERT_NOT_EQUAL_STR(expected, actual)` | Fails if strings are equal |
| `RCU_ASSERT_STR_CONTAINS(haystack, needle)` | Fails if `haystack` does not contain `needle` |
| `RCU_ASSERT_STR_EMPTY(str)` | Fails if string is not empty |
| `RCU_ASSERT_STR_NOT_EMPTY(str)` | Fails if string is empty |

#### Byte Arrays

| Macro | Description |
|-------|-------------|
| `RCU_ASSERT_EQUAL_MEM(a, b, size)` | Fails if byte arrays differ |
| `RCU_ASSERT_NOT_EQUAL_MEM(a, b, size)` | Fails if byte arrays are equal |
| `RCU_ASSERT_ZERO_MEM(ptr, size)` | Fails if any byte in the region is non-zero |
| `RCU_ASSERT_EQUAL_ARRAY(expected, actual, n)` | Fails if integer arrays differ element-wise |

#### Bits

| Macro | Description |
|-------|-------------|
| `RCU_ASSERT_BIT_SET(data, bit_pos)` | Fails if bit at `bit_pos` is not set |
| `RCU_ASSERT_BIT_NOT_SET(data, bit_pos)` | Fails if bit at `bit_pos` is set |
| `RCU_ASSERT_BITS_SET(data, mask)` | Fails if any bit in `mask` is not set |
| `RCU_ASSERT_BITS_NOT_SET(data, mask)` | Fails if any bit in `mask` is set |

---

## Internals

### Exception Handling

rcunit uses non-local jumps (`setjmp`/`longjmp`) to simulate exceptions when
running tests and fixtures.

### Signal Handling

rcunit installs signal handlers for SIGSEGV, SIGILL, SIGFPE, and SIGBUS during
test execution. When a signal fires, rcunit records the failure and continues
with the next test rather than terminating the process.

```c
#include "rcunit.h"

RCU_TEST(my_test) {
    int *p = NULL;
    *p = 1; /* triggers SIGSEGV — rcunit catches it and marks test failed */
}

int main(int argc, char *argv[]) {
    RCU_ADD_TEST("signal_tests", my_test);
    return rcu_run_tests();
}
```

Example output:

```
[INFO]  Test run started Sun Feb 10 20:22:32 2013
[ERROR] Caught segmentation violation in my_test
[INFO]  Test run report generated : rcunit_test_run_report.txt
[INFO]  Test run finished Sun Feb 10 20:22:32 2013
[INFO]  Test Run Results: Passed : 0  Failed : 1
[INFO]  Test failed!
```

### Logging

rcunit includes built-in logging via `RCU_LOG_INFO`, `RCU_LOG_WARN`,
`RCU_LOG_ERROR`, and `RCU_LOG_DEBUG`. Debug logging is enabled by compiling
with `-DRCU_DEBUG`.

### Self-Tests

rcunit uses its own framework to test itself. The test suite lives in the
`tests/` directory and can be run with `make check`.

---

## Contributors

- Tomasz Kantecki (Intel, Ireland) — signal handling
- Abigail Gamis (SHFL, Australia) — user guide, API refinements
