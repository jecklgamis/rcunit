RCU_DEF_GENERIC_FUNC(rcu_test_redirect_001_out_hnd_int,param){
    fprintf(stdout,"rcu_test_redirect_001_out_hnd_int invoked\n");
}

RCU_DEF_GENERIC_FUNC(rcu_test_redirect_001_out_hnd_destroy,param){
    fprintf(stdout,"rcu_test_redirect_001_out_hnd_destroy invoked\n");
}

RCU_DEF_GENERIC_FUNC(rcu_test_redirect_001_out_hnd_out_impl,param){
RCU_OUTPUT_RECORD *out_rec = (RCU_OUTPUT_RECORD*)param;
    //fprintf(stdout,"rcu_test_redirect_001_out_hnd_out_impl invoked\n");
    fprintf(stdout,"%s",out_rec->data);
}

DEF_LOCAL_TEST_FUNC(rcu_test_redirect_001){
RCU_OUTPUT_HANDLER out_hnd;
    rcu_init();
    out_hnd.init = rcu_test_redirect_001_out_hnd_int;
    out_hnd.destroy = rcu_test_redirect_001_out_hnd_destroy;
    out_hnd.out_impl = rcu_test_redirect_001_out_hnd_out_impl;
    rcu_add_out_hnd(&out_hnd);
    rcu_run_test_mach();
    rcu_destroy();
    return(RCU_E_OK);
}
