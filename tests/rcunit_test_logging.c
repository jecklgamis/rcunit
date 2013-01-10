DEF_LOCAL_TEST_FUNC(rcu_test_logging_001){
    rcu_set_log_level(RCU_LOG_LEVEL_ALL);
    LOCAL_ASSERT(rcu_init()==RCU_E_OK);
    RCU_LOG_INFO("LEVEL = RCU_LOG_LEVEL_ALL. This must be displayed");
    RCU_LOG_WARN("LEVEL = RCU_LOG_LEVEL_ALL. This must be displayed");
    RCU_LOG_FATAL("LEVEL = RCU_LOG_LEVEL_ALL. This must be displayed");
    rcu_set_log_level(RCU_LOG_LEVEL_OFF);
    RCU_LOG_INFO("LEVEL = RCU_LOG_LEVEL_OFF. This must not be displayed");
    RCU_LOG_WARN("LEVEL = RCU_LOG_LEVEL_OFF. This must not be displayed");
    RCU_LOG_FATAL("LEVEL = RCU_LOG_LEVEL_OFF. This must not be displayed");
    rcu_set_log_level(RCU_LOG_LEVEL_INFO);
    RCU_LOG_INFO("LEVEL = RCU_LOG_LEVEL_INFO. This must be displayed");
    RCU_LOG_WARN("LEVEL = RCU_LOG_LEVEL_INFO. This must be displayed");
    RCU_LOG_FATAL("LEVEL = RCU_LOG_LEVEL_INFO. This must be displayed");
    rcu_set_log_level(RCU_LOG_LEVEL_WARN);
    RCU_LOG_INFO("LEVEL = RCU_LOG_LEVEL_WARN. This must not be displayed");
    RCU_LOG_WARN("LEVEL = RCU_LOG_LEVEL_WARN. This must be displayed");
    RCU_LOG_FATAL("LEVEL = RCU_LOG_LEVEL_WARN. This must be displayed");
    rcu_set_log_level(RCU_LOG_LEVEL_FATAL);
    RCU_LOG_INFO("LEVEL = RCU_LOG_LEVEL_FATAL. This must not be displayed");
    RCU_LOG_WARN("LEVEL = RCU_LOG_LEVEL_FATAL. This must not be displayed");
    RCU_LOG_FATAL("LEVEL = RCU_LOG_LEVEL_FATAL. This must be displayed");
    rcu_set_log_level(RCU_LOG_LEVEL_OFF+1);
    /* note: if ne log level is not successfully set, the old setting must be used */
    RCU_LOG_INFO("LEVEL = RCU_LOG_LEVEL_FATAL. This must not be displayed");
    RCU_LOG_WARN("LEVEL = RCU_LOG_LEVEL_FATAL. This must not be displayed");
    RCU_LOG_FATAL("LEVEL = RCU_LOG_LEVEL_FATAL. This must be displayed");
    rcu_set_log_level(RCU_LOG_LEVEL_ALL);
    LOCAL_ASSERT(rcu_destroy()==RCU_E_OK);
    return(RCU_E_OK);
}

