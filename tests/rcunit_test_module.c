/*====================================================================
 *  TEST_MODULE BASIC TESTS
 *==================================================================*/

RCU_DEF_TEST_FUNC(rcu_test_mod_001_f1,param){
    RCU_LOG_INFO("rcu_test_mod_001_f1 invoked!");
}

RCU_DEF_TEST_FUNC(rcu_test_mod_001_f2,param){
    RCU_LOG_INFO("rcu_test_mod_001_f2 invoked!");
}

RCU_DEF_TEST_FUNC(rcu_test_mod_001_f3,param){
    RCU_LOG_INFO("rcu_test_mod_001_f3 invoked!");
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_001){
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1) == RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(RCU_NULL,rcu_test_mod_001_f1,RCU_NULL,RCU_NULL,"rcu_test_mod_001_f1",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_001_f2,RCU_NULL,RCU_NULL,"rcu_test_mod_001_f2",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_001_f3,RCU_NULL,RCU_NULL,"rcu_test_mod_001_f3",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_002){
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT_NULL(m1=rcu_cre_test_mod(RCU_NULL,RCU_NULL,RCU_NULL,RCU_TRUE));
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_003){
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT_NULL(m1=rcu_cre_test_mod("default-mod",RCU_NULL,RCU_NULL,RCU_TRUE));
    return(RCU_E_OK);
}

RCU_DEF_INIT_FUNC(rcu_test_mod_004_mod_init,param){
    fprintf(stdout,"rcu_test_mod_004_mod_init invoked!\n");
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_004){
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",rcu_test_mod_004_mod_init,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1) == RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_DESTROY_FUNC(rcu_test_mod_005_mod_destroy,param){
    fprintf(stdout,"rcu_test_mod_005_mod_destroy invoked!\n");
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_005){
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,rcu_test_mod_005_mod_destroy,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1) == RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_006){
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_MOD(m2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(m2=rcu_cre_test_mod("m2",RCU_NULL,RCU_NULL,RCU_FALSE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1) == RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m2) == RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_007){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_008){
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_destroy_test_mod(m1)==RCU_E_NG);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_009){
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1) == RCU_E_NG);
    LOCAL_ASSERT(rcu_add_test_mod(r1,m1) == RCU_E_NG);
    LOCAL_ASSERT(rcu_destroy_test_mod(m1)==RCU_E_NG);
    LOCAL_ASSERT(rcu_destroy_test_reg(r1)==RCU_E_NG);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_010){
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_REG(r2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(r2=rcu_cre_test_reg("r2",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_reg(r2)==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(r1,m1) == RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod(r2,m1) == RCU_E_NG);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_011){
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(r1,m1) == RCU_E_NG);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy_test_mod(m1) == RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy_test_reg(r1) == RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_012){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_MOD(m2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(m2=rcu_cre_test_mod("m2",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1) == RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod(r1,m2) == RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_013){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_MOD(m2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(m2=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1) == RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod(r1,m2) == RCU_E_NG);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy_test_mod(m2)==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_014_f1)
RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_014_f2)

RCU_DEF_FUNC_TBL(rcu_test_mod_014_ftbl1)
    RCU_INC_FUNC_AUTONAME(rcu_test_mod_014_f1,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_mod_014_f2,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

RCU_DEF_MOD_TBL(rcu_test_mod_014_mtbl1)
    RCU_INC_MOD("rcu_test_mod_014_m1",RCU_NULL,RCU_NULL,rcu_test_mod_014_ftbl1,RCU_TRUE)
RCU_DEF_MOD_TBL_END

DEF_LOCAL_TEST_FUNC(rcu_test_mod_014){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod_tbl(RCU_NULL,rcu_test_mod_014_mtbl1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_015_f1)
RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_015_f2)

RCU_DEF_FUNC_TBL(rcu_test_mod_015_ftbl1)
    RCU_INC_FUNC_AUTONAME(rcu_test_mod_015_f1,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_mod_015_f2,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

RCU_DEF_MOD_TBL(rcu_test_mod_015_mtbl1)
    RCU_INC_MOD("rcu_test_mod_015_m1",RCU_NULL,RCU_NULL,rcu_test_mod_014_ftbl1,RCU_TRUE)
RCU_DEF_MOD_TBL_END

DEF_LOCAL_TEST_FUNC(rcu_test_mod_015){
    LOCAL_ASSERT(rcu_add_test_mod_tbl(RCU_NULL,rcu_test_mod_015_mtbl1)==RCU_E_NG);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_016){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod_tbl(RCU_NULL,RCU_NULL)==RCU_E_NG);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_MOD_TBL(rcu_test_mod_017_mtbl1)
    RCU_INC_MOD("rcu_test_mod_017_m1",RCU_NULL,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_MOD_TBL_END

DEF_LOCAL_TEST_FUNC(rcu_test_mod_017){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod_tbl(RCU_NULL,rcu_test_mod_017_mtbl1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_018_f1)
RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_018_f2)

RCU_DEF_FUNC_TBL(rcu_test_mod_018_ftbl1)
    RCU_INC_FUNC_AUTONAME(rcu_test_mod_018_f1,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(rcu_test_mod_018_f2,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

RCU_DEF_MOD_TBL(rcu_test_mod_018_mtbl1)
    RCU_INC_MOD(RCU_NULL,RCU_NULL,RCU_NULL,rcu_test_mod_018_ftbl1,RCU_TRUE)
RCU_DEF_MOD_TBL_END

DEF_LOCAL_TEST_FUNC(rcu_test_mod_018){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod_tbl(RCU_NULL,rcu_test_mod_018_mtbl1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_019){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_MOD(m2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(m2=rcu_cre_test_mod("m2",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(r1,m1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m2)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_020){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_mod(m1) == RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_021){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_mod_by_name("m1") == RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_022){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_mod(RCU_NULL) == RCU_E_OK);
    LOCAL_ASSERT(rcu_run_test_mod(RCU_DEFAULT_MODULE) == RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_023){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_mod_by_name("unknown_module_name") == RCU_E_NG);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_024){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_MOD(m2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(m2=rcu_cre_test_mod("m2",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_mod(m2) == RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy_test_mod(m2) == RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_025){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_mod_026){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_FALSE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_mod(m1) == RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_027_f1)
RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_027_f2)
RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_027_f3)

DEF_LOCAL_TEST_FUNC(rcu_test_mod_027){
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_FALSE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_027_f1,RCU_NULL,RCU_NULL,"rcu_test_mod_027_f1",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_027_f2,RCU_NULL,RCU_NULL,"rcu_test_mod_027_f2",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_027_f3,RCU_NULL,RCU_NULL,"rcu_test_mod_027_f3",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_INIT_FUNC(rcu_test_mod_028_mod_init,param){
    RCU_FAIL_FATAL("rcu_test_mod_028_mod_init must fail");
}

RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_028_f1)
RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_028_f2)
RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_028_f3)

DEF_LOCAL_TEST_FUNC(rcu_test_mod_028){
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",rcu_test_mod_028_mod_init,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_028_f1,RCU_NULL,RCU_NULL,"rcu_test_mod_028_f1",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_028_f2,RCU_NULL,RCU_NULL,"rcu_test_mod_028_f2",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_028_f3,RCU_NULL,RCU_NULL,"rcu_test_mod_028_f3",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

RCU_DEF_INIT_FUNC(rcu_test_mod_029_mod_destroy,param){
    RCU_FAIL_FATAL("rcu_test_mod_029_mod_destroy must fail");
}

RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_029_f1)
RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_029_f2)
RCU_DEF_EMPTY_TEST_FUNC(rcu_test_mod_029_f3)

DEF_LOCAL_TEST_FUNC(rcu_test_mod_029){
RCU_DEF_TEST_MOD(m1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,rcu_test_mod_029_mod_destroy,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(RCU_NULL,m1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_029_f1,RCU_NULL,RCU_NULL,"rcu_test_mod_029_f1",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_029_f2,RCU_NULL,RCU_NULL,"rcu_test_mod_029_f2",RCU_TRUE)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_func(m1,rcu_test_mod_029_f3,RCU_NULL,RCU_NULL,"rcu_test_mod_029_f3",RCU_TRUE)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

