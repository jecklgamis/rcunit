RCU_INT rcu_test_excp_001_1(){
    RCU_LOG_INFO("rcu_test_excp_001_1 invoked!");
    RCU_THROW(RCU_GET_EXCP(RCU_EXCP_NULLPOINTER));
    return(RCU_E_OK);
}

RCU_INT rcu_test_excp_001_2(){
    RCU_LOG_INFO("rcu_test_excp_001_2 invoked!");
    RCU_TRY{
        rcu_test_excp_001_1();
    }RCU_CATCH(excp){
         RCU_LOG_WARN("rcu_test_excp_001_1 failed!");
         RCU_LOG_WARN_P1("rcu_test_excp_001_2 : catched exception :%d",excp->id);
         RCU_THROW(RCU_GET_EXCP(RCU_EXCP_INVPARAM));
    }RCU_END_CATCH
    return(RCU_E_OK);
}

DEF_LOCAL_TEST_FUNC(rcu_test_excp_001){
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    RCU_TRY{
        rcu_test_excp_001_2();
    }RCU_CATCH(excp){
        RCU_LOG_WARN_P1("rcu_test_excp_001: catched exception :%d",excp->id);
    }RCU_END_CATCH
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

