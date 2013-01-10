/*====================================================================
 *  FRAMEWORK BASIC TESTS
 *==================================================================*/

DEF_LOCAL_TEST_FUNC(rcu_test_frmwk_001){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_REG(r2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(r2=rcu_cre_test_reg("r2",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_reg(r2)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_frmwk_002){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}


DEF_LOCAL_TEST_FUNC(rcu_test_frmwk_003){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}


DEF_LOCAL_TEST_FUNC(rcu_test_frmwk_004){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_init()==RCU_E_NG);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_frmwk_005){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_NG);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_frmwk_006){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_frmwk_007_f1,param){
    RCU_LOG_INFO("rcu_test_frmwk_007_f1 invoked!");
}


DEF_LOCAL_TEST_FUNC(rcu_test_frmwk_007){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_frmwk_007_f1,RCU_NULL,RCU_NULL,"rcu_test_frmwk_007_f1",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_GENERIC_FUNC(rcu_test_frmwk_008_test_run_hook,param){
RCU_INT run_event;
    run_event = RCU_GET_RUN_EVT_TYPE(param);
    if (run_event == RCU_TEST_RUN_STARTED){
        RCU_LOG_INFO("rcu_test_frmwk_008_test_run_hook invoked on test run start!");
    }else{
        RCU_LOG_INFO("rcu_test_frmwk_008_test_run_hook invoked on test run end!");
    }
}

RCU_DEF_TEST_FUNC(rcu_test_frmwk_008_f1,param){
    RCU_LOG_INFO("rcu_test_frmwk_008_f1 invoked!");
}

DEF_LOCAL_TEST_FUNC(rcu_test_frmwk_008){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_set_run_hook(rcu_test_frmwk_008_test_run_hook) == RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_frmwk_008_f1,RCU_NULL,RCU_NULL,"rcu_test_frmwk_008_f1",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}


DEF_LOCAL_TEST_FUNC(rcu_test_frmwk_009){
    LOCAL_ASSERT(rcu_dump_test_dbase()==RCU_E_NG);
     return(RCU_E_OK);
}
