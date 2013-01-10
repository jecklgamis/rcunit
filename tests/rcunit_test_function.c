/*====================================================================
 *  TEST_FUNCTION BASIC TESTS
 *==================================================================*/
RCU_DEF_TEST_FUNC(rcu_test_func_001_f1,param){
    RCU_LOG_INFO("rcu_test_func_001_f1 invoked!");
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_001){
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_001_f1,RCU_NULL,RCU_NULL,"rcu_test_func_001_f1",RCU_TRUE)==RCU_E_NG);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_002_f1,param){
    RCU_LOG_INFO("rcu_test_func_002_f1 invoked!");
}


DEF_LOCAL_TEST_FUNC(rcu_test_func_002){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_002_f1,RCU_NULL,RCU_NULL,"rcu_test_func_002_f1",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_002_f1,RCU_NULL,RCU_NULL,"rcu_test_func_002_f1",RCU_TRUE)==RCU_E_NG);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_003_f1,param){
    RCU_LOG_INFO("rcu_test_func_003_f1 invoked!");
}

RCU_DEF_TEST_FUNC(rcu_test_func_003_f2,param){
    RCU_LOG_INFO("rcu_test_func_003_f2 invoked!");
}


DEF_LOCAL_TEST_FUNC(rcu_test_func_003){
RCU_DEF_TEST_MOD(m1);
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_003_f1,RCU_NULL,RCU_NULL,"rcu_test_func_003_f1",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_func_003_f2,RCU_NULL,RCU_NULL,"rcu_test_func_003_f2",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_004_f1,param){
    RCU_LOG_INFO("rcu_test_func_004_f1 invoked!");
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_004){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_DEFAULT_MODULE,rcu_test_func_004_f1,RCU_NULL,RCU_NULL,RCU_NULL,RCU_TRUE)==RCU_E_NG);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_005_f1,param){
    RCU_LOG_INFO("rcu_test_func_005_f1 invoked!");
}

RCU_DEF_INIT_FUNC(rcu_test_func_005_f1_init,param){
    RCU_LOG_INFO("rcu_test_func_005_f1_init invoked!");
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_005){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_005_f1,rcu_test_func_005_f1_init,RCU_NULL,"rcu_test_func_005_f1",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_006_f1,param){
    RCU_LOG_INFO("rcu_test_func_006_f1 invoked!");
}

RCU_DEF_INIT_FUNC(rcu_test_func_006_f1_destroy,param){
    RCU_LOG_INFO("rcu_test_func_006_f1_destroy invoked!");
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_006){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_006_f1,RCU_NULL,rcu_test_func_006_f1_destroy,"rcu_test_func_006_f1",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_007_f1,param){
    RCU_LOG_INFO("rcu_test_func_007_f1 invoked!");
}

RCU_DEF_TEST_FUNC(rcu_test_func_007_f2,param){
    RCU_LOG_INFO("rcu_test_func_007_f2 invoked!");
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_007){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_007_f1,RCU_NULL,RCU_NULL,"rcu_test_func_007_f1",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_007_f2,RCU_NULL,RCU_NULL,"rcu_test_func_007_f1",RCU_TRUE)==RCU_E_NG);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_008_f1,param){
    RCU_LOG_INFO("rcu_test_func_008_f1 invoked!");
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_008){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_008_f1,RCU_NULL,RCU_NULL,"rcu_test_func_008_f1",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_008_f1,RCU_NULL,RCU_NULL,"rcu_test_func_008_f1_bogus",RCU_TRUE)==RCU_E_NG);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_009_f1,param){
    RCU_LOG_INFO("rcu_test_func_009_f1 invoked!");
}

RCU_DEF_TEST_FUNC(rcu_test_func_009_f2,param){
    RCU_LOG_INFO("rcu_test_func_009_f2 invoked!");
}

RCU_DEF_FUNC_TBL(rcu_test_func_009_ftbl1)
    RCU_INC_FUNC_AUTONAME(rcu_test_func_009_f1,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_func_009_f2,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

DEF_LOCAL_TEST_FUNC(rcu_test_func_009){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func_tbl(RCU_NULL,rcu_test_func_009_ftbl1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_010_f1,param){
    RCU_LOG_INFO("rcu_test_func_010_f1 invoked!");
}

RCU_DEF_TEST_FUNC(rcu_test_func_010_f2,param){
    RCU_LOG_INFO("rcu_test_func_010_f2 invoked!");
}

RCU_DEF_FUNC_TBL(rcu_test_func_010_ftbl1)
    RCU_INC_FUNC("rcu_test_func_010_f1",rcu_test_func_010_f1,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_func_010_f2,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

DEF_LOCAL_TEST_FUNC(rcu_test_func_010){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func_tbl(RCU_NULL,rcu_test_func_010_ftbl1)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_011){
    /** same as rcu_test_func_005 */
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_012){
    /** same as rcu_test_func_006 */
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_013_f1,param){
    RCU_LOG_INFO("rcu_test_func_013_f1 invoked!");
}

RCU_DEF_INIT_FUNC(rcu_test_func_013_f1_init,param){
    RCU_LOG_INFO("rcu_test_func_013_f1_init invoked!");
}

RCU_DEF_DESTROY_FUNC(rcu_test_func_013_f1_destroy,param){
    RCU_LOG_INFO("rcu_test_func_013_f1_destroy invoked!");
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_013){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_013_f1,
        rcu_test_func_013_f1_init,rcu_test_func_013_f1_destroy,
        "rcu_test_func_013_f1",RCU_FALSE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_014_f1,param){
}

RCU_DEF_INIT_FUNC(rcu_test_func_014_f1_init,param){
    RCU_FAIL_FATAL("rcu_test_func_014_f1_init is supposed to fail");
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_014){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_014_f1,rcu_test_func_014_f1_init,RCU_NULL,"rcu_test_func_014_f1",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_TEST_FUNC(rcu_test_func_015_f1,param){
    RCU_ASSERT(RCU_TRUE);
    RCU_ASSERT(RCU_TRUE);
    RCU_ASSERT(RCU_TRUE);
    RCU_ASSERT(RCU_FALSE);
    RCU_FAIL_FATAL("rcu_test_func_015_f1 fatal assertion");
}

RCU_DEF_DESTROY_FUNC(rcu_test_func_015_f1_destroy,param){
    RCU_FAIL_FATAL("rcu_test_func_015_f1_destroy is supposed to fail");
}

DEF_LOCAL_TEST_FUNC(rcu_test_func_015){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_func_015_f1,RCU_NULL,rcu_test_func_015_f1_destroy,"rcu_test_func_015_f1",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}
