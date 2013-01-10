DEF_LOCAL_TEST_FUNC(rcu_test_analysis_001){
    RCU_LOG_INFO_P1("sizeof(RCU_TEST_FUNCTION) = %d",sizeof(RCU_TEST_FUNCTION));
    RCU_LOG_INFO_P1("sizeof(RCU_TEST_MODULE) = %d",sizeof(RCU_TEST_MODULE));
    RCU_LOG_INFO_P1("sizeof(RCU_TEST_REGISTRY) = %d",sizeof(RCU_TEST_REGISTRY));
    RCU_LOG_INFO_P1("sizeof(RCU_TEST_MACHINE) = %d",sizeof(RCU_TEST_MACHINE));
    return(RCU_E_OK);
}
