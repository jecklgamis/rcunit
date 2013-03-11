/*
 * The MIT License (MIT)
 *
 * RCUNIT - A unit testing framework for C
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "rcunit_test.h" 

int rcu_test_exception_001_1() {
    TMK_LOG("rcu_test_exception_001_1 invoked!");
    RCU_THROW(RCU_GET_EXCP(RCU_EXCP_NULLPOINTER));
    return RCU_E_OK;
}

int rcu_test_exception_001_2() {
    TMK_LOG("rcu_test_exception_001_2 invoked!");
    RCU_TRY
    {
        rcu_test_exception_001_1();
    }

    RCU_CATCH(e) {
        TMK_LOG("rcu_test_exception_001_2 : caught  : %s", e->name);
        TMK_LOG("rcu_test_exception_001_2 : throwing : %s", RCU_GET_EXCP(RCU_EXCP_INVPARAM)->name);
        RCU_THROW(RCU_GET_EXCP(RCU_EXCP_INVPARAM));
    }
    RCU_END_CATCH
    return RCU_E_OK;
}

TMK_TEST(rcu_test_exception_handling) {
    int exception_id = 0;
    rcu_init();
    RCU_TRY
    {
        rcu_test_exception_001_2();
    }

    RCU_CATCH(exception) {
        TMK_LOG("rcu_test_exception_001 : caught  : %s", RCU_GET_EXCP(exception->id)->name);
        exception_id = exception->id;
    }
    RCU_END_CATCH
    TMK_ASSERT(exception_id == RCU_EXCP_INVPARAM);
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

void rcu_trigger_sigillfn() {
#if 1
    raise(SIGILL);
#else    
    //This causes SIGSEV in OSX 10.6
    rcu_u1 illbuf[4] = {0xff, 0xff, 0xff, 0xff};
    void (*fn)(void) = (void*) (illbuf);
    fn();
#endif    
}

void rcu_trigger_sigfpe() {
#if 1
    raise(SIGFPE);
#else    
    // This does not generate an FPE signal (OSX 10.6, Fedora 14/x86_64)
    int x = 0;
    int y = 1 / x;
#endif    
}

void rcu_trigger_sigsev() {
#if 1
    raise(SIGSEGV);
#else    

    struct dummy {
        int x;
    };
    struct dummy *ptr = NULL;
    ptr->x = 1;
#endif    
}

void rcu_trigger_sigbus() {
#if 1
    raise(SIGBUS);
#else
#endif
}

/**
 * Verify that we can catch floating point exception signal
 */
TMK_TEST(rcu_test_exception_catch_sigfpe) {
    RCU_TRY
    {
        rcu_trigger_sigfpe();
        TMK_FAIL("Expecting SIGFPE");
    }

    RCU_CATCH(e) {
        TMK_LOG("caught exception :%s", e->name);
        TMK_ASSERT_EQUAL_STRING("floating point", e->name)
    }
    RCU_END_CATCH
}

/** Verify that we can catch illegal instruction signal */
TMK_TEST(rcu_test_exception_catch_sigill) {
    RCU_TRY
    {
        rcu_trigger_sigillfn();
        TMK_FAIL("Expecting SIGILL");
    }

    RCU_CATCH(e) {
        TMK_LOG("caught exception :%s", e->name);
        TMK_ASSERT_EQUAL_STRING("illegal instruction", e->name)
    }
    RCU_END_CATCH
}

/** Verify that we can catch segmentation violation signal */
TMK_TEST(rcu_test_exception_catch_sigsev) {
    RCU_TRY
    {
        rcu_trigger_sigsev();
        TMK_FAIL("Expecting SIGSEV");
    }

    RCU_CATCH(e) {
        TMK_LOG("caught exception :%s", e->name);
        TMK_ASSERT_EQUAL_STRING("segmentation violation", e->name)

    }
    RCU_END_CATCH

}

TMK_TEST(rcu_test_exception_catch_sigbus) {
    RCU_TRY
    {
        rcu_trigger_sigbus();
        TMK_FAIL("Expecting SIGBUS");
    }

    RCU_CATCH(e) {
        TMK_LOG("caught exception :%s", e->name);
        TMK_ASSERT_EQUAL_STRING("bus error", e->name)
    }
    RCU_END_CATCH

}
