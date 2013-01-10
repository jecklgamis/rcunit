/*====================================================================
 *  TEST_REGISTRY BASIC TESTS
 *==================================================================*/

DEF_LOCAL_TEST_FUNC(rcu_test_reg_001){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_MOD(m2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(m1=rcu_cre_test_mod("m1",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(m2=rcu_cre_test_mod("m2",RCU_NULL,RCU_NULL,RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_mod(r1,m1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_mod(r1,m2)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_run_test_mach()==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_002){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT_NULL(r1=rcu_cre_test_reg(RCU_NULL,RCU_TRUE));
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_003){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT_NULL(r1=rcu_cre_test_reg("default-reg",RCU_TRUE));
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_004){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_REG(r2)
     LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(r2=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_add_test_reg(r2)==RCU_E_NG);
    LOCAL_ASSERT(rcu_destroy_test_reg(r2)==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_005){
RCU_DEF_TEST_REG(test_reg)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_006){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_destroy_test_reg(r1)==RCU_E_NG);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_007){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_NG);
    LOCAL_ASSERT(rcu_destroy_test_reg(r1)==RCU_E_NG);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_008){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_reg(r1)==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_009){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_reg_by_name("r1")==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}


DEF_LOCAL_TEST_FUNC(rcu_test_reg_010){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_reg_by_name(RCU_NULL)==RCU_E_OK);
    LOCAL_ASSERT(rcu_run_test_reg_by_name(RCU_DEFAULT_REGISTRY)==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_011){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_run_test_reg_by_name("r1_xxx")==RCU_E_NG);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}


DEF_LOCAL_TEST_FUNC(rcu_test_reg_012){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_REG(r2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT_NOT_NULL(r2=rcu_cre_test_reg("r2",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    rcu_dump_test_dbase();
    /* r2 must not be runnable since it is not yet registered */
    LOCAL_ASSERT(rcu_run_test_reg(r2)==RCU_E_NG);
    LOCAL_ASSERT(rcu_destroy_test_reg(r2)==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}


DEF_LOCAL_TEST_FUNC(rcu_test_reg_013){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_FALSE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_reg_014){
RCU_DEF_TEST_REG(r1)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_TRUE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    rcu_dump_test_dbase();
    rcu_run_test_reg(r1);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}


DEF_LOCAL_TEST_FUNC(rcu_test_reg_015){
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_REG(r2)
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    LOCAL_ASSERT_NOT_NULL(r1=rcu_cre_test_reg("r1",RCU_FALSE));
    LOCAL_ASSERT_NOT_NULL(r2=rcu_cre_test_reg("r2",RCU_FALSE));
    LOCAL_ASSERT(rcu_add_test_reg(r1)==RCU_E_OK);
    rcu_dump_test_dbase();
    LOCAL_ASSERT(rcu_destroy_test_reg(r2)==RCU_E_OK);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

