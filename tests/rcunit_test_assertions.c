
/** this contains only non-fatal assertions */
RCU_DEF_TEST_FUNC(rcu_test_assert_001_f1,param){
RCU_INT ercd;
    RCU_ASSERT(RCU_FALSE);
    RCU_FAIL("rcu_test_assert_001_f1");
    RCU_ASSERT_TRUE(RCU_FALSE);
    RCU_ASSERT_FALSE(RCU_TRUE);
    RCU_ASSERT_NULL(&ercd);
    RCU_ASSERT_NOT_NULL(RCU_NULL);
    RCU_ASSERT_FATAL(RCU_FALSE) //the only fatal;
}

RCU_DEF_TEST_FUNC(rcu_test_assert_001_f2,param){
    RCU_ASSERT_FATAL(RCU_FALSE);
}

RCU_DEF_TEST_FUNC(rcu_test_assert_001_f3,param){
    RCU_FAIL_FATAL("rcu_test_assert_001_f3");
}

RCU_DEF_TEST_FUNC(rcu_test_assert_001_f4,param){
    RCU_ASSERT_TRUE_FATAL(RCU_FALSE);
}

RCU_DEF_TEST_FUNC(rcu_test_assert_001_f5,param){
    RCU_ASSERT_FALSE_FATAL(RCU_TRUE);
}

RCU_DEF_TEST_FUNC(rcu_test_assert_001_f6,param){
RCU_INT ercd;
    RCU_ASSERT_NULL_FATAL(&ercd);
}

RCU_DEF_TEST_FUNC(rcu_test_assert_001_f7,param){
    RCU_ASSERT_NOT_NULL_FATAL(RCU_NULL);
}
/** The only successful assertion */
RCU_DEF_TEST_FUNC(rcu_test_assert_001_f8,param){
    RCU_ASSERT_NULL(RCU_NULL);
    RCU_ASSERT_NULL(RCU_NULL);
    RCU_ASSERT_NULL(RCU_NULL);

}

RCU_DEF_FUNC_TBL(rcu_test_assert_001_ftbl1)
    RCU_INC_FUNC_AUTONAME(rcu_test_assert_001_f1,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_assert_001_f2,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_assert_001_f3,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_assert_001_f4,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_assert_001_f5,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_assert_001_f6,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_assert_001_f7,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_assert_001_f8,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

/** This tests the non-fatal assertions */
DEF_LOCAL_TEST_FUNC(rcu_test_assert_001){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func_tbl(RCU_DEFAULT_MODULE,
        rcu_test_assert_001_ftbl1)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_GENERIC_FUNC(rcu_test_assert_002_assert_hook,param){
    RCU_LOG_INFO("rcu_test_assert_002_assert_hook invoked!");
}

/** This is a test for assertions outside the known run contexts
     (ie. test function, and init/destroy handlers)
 */
DEF_LOCAL_TEST_FUNC(rcu_test_assert_002){
RCU_INT x=0;
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_set_assert_hook(rcu_test_assert_002_assert_hook)==RCU_E_OK);
    RCU_ASSERT(x==1);
    RCU_ASSERT(x==2);
    rcu_dump_asserts();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}





