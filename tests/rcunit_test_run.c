/*
#if 0
f0 - has no assertion statements
f1 - has no assertion failures
f2 - with non-fatal assertion failures
f3 - with fatal assertion failures
f4 - init failed (with non-fatal or fatal with assertion failures)
f5 - destroy failed (with non-fatal and fatal assertion failures)
f6-f15 - has no assertion failures
#endif

#if 0
m0 : no assertions
     no init
     no destroy
m1:
    test functions - OK
    init           - OK
    destroy        - OK
m2:
    test functions - NG
        -> with non-fatal assertion failures
        -> with fatal assertion failures
    init           - OK
    destroy        - OK
m3:
    test functions - OK
    init           - NG (with non-fatal or fatal assertion failures)
    destroy        - OK

m4:
    test functions - OK
    init           - OK
    destroy        - NG (with non-fatal or fatal assertion failures)

m5: test functions - OK
    init           - OK
    destroy        - OK
    registered as disabled

*/

//f0
RCU_DEF_TEST_FUNC(user_f0,param){
}

RCU_DEF_INIT_FUNC(user_f0_init,param){
}

RCU_DEF_DESTROY_FUNC(user_f0_destroy,param){
}

//f1
RCU_DEF_TEST_FUNC(user_f1,param){
    RCU_ASSERT(RCU_TRUE);
}

RCU_DEF_INIT_FUNC(user_f1_init,param){
}

RCU_DEF_DESTROY_FUNC(user_f1_destroy,param){
}

//f2
RCU_DEF_TEST_FUNC(user_f2,param){
    RCU_FAIL("user_f2");
}

RCU_DEF_INIT_FUNC(user_f2_init,param){
}

RCU_DEF_DESTROY_FUNC(user_f2_destroy,param){
}
//f3
RCU_DEF_TEST_FUNC(user_f3,param){
    RCU_FAIL_FATAL("user_f3");
}

RCU_DEF_INIT_FUNC(user_f3_init,param){
}

RCU_DEF_DESTROY_FUNC(user_f3_destroy,param){
}

//f4
RCU_DEF_TEST_FUNC(user_f4,param){
    RCU_ASSERT(RCU_TRUE);
}

RCU_DEF_INIT_FUNC(user_f4_init,param){
    RCU_FAIL("user_f4_init");
}

RCU_DEF_DESTROY_FUNC(user_f4_destroy,param){
}

//f5
RCU_DEF_TEST_FUNC(user_f5,param){
    RCU_ASSERT(RCU_TRUE);
}

RCU_DEF_INIT_FUNC(user_f5_init,param){
}

RCU_DEF_DESTROY_FUNC(user_f5_destroy,param){
    RCU_FAIL("user_f5_destroy");
}

RCU_DEF_TEST_FUNC(user_f100,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f101,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f102,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f103,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f104,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f105,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f106,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f107,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f108,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f109,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f110,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f111,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f112,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f113,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f114,param){RCU_ASSERT(RCU_TRUE);}
RCU_DEF_TEST_FUNC(user_f115,param){RCU_ASSERT(RCU_TRUE);}

RCU_DEF_TEST_FUNC(user_f200,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f201,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f202,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f203,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f204,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f205,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f206,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f207,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f208,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f209,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f210,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f211,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f212,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f213,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f214,param){RCU_ASSERT(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f215,param){RCU_ASSERT(RCU_FALSE);}

RCU_DEF_TEST_FUNC(user_f300,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f301,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f302,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f303,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f304,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f305,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f306,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f307,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f308,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f309,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f310,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f311,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f312,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f313,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f314,param){RCU_ASSERT_FATAL(RCU_FALSE);}
RCU_DEF_TEST_FUNC(user_f315,param){RCU_ASSERT_FATAL(RCU_FALSE);}

RCU_DEF_INIT_FUNC(user_m1_init,param){
}

RCU_DEF_INIT_FUNC(user_m1_destroy,param){
}

RCU_DEF_INIT_FUNC(user_m2_init,param){
}

RCU_DEF_INIT_FUNC(user_m2_destroy,param){
}

RCU_DEF_INIT_FUNC(user_m3_init,param){
    RCU_FAIL("user_m3_init");
}

RCU_DEF_INIT_FUNC(user_m3_destroy,param){
}

RCU_DEF_INIT_FUNC(user_m4_init,param){
}

RCU_DEF_INIT_FUNC(user_m4_destroy,param){
    RCU_FAIL("user_m4_destroy");
}

RCU_DEF_INIT_FUNC(user_m5_init,param){
}

RCU_DEF_INIT_FUNC(user_m5_destroy,param){
}

//Test entity tables
RCU_DEF_FUNC_TBL(user_ftbl0)
RCU_DEF_FUNC_TBL_END

//This contains successful test functions
RCU_DEF_FUNC_TBL(user_ftbl1)
    RCU_INC_FUNC_AUTONAME(user_f100,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f101,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f102,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

//This contains failed test functions (all types)
RCU_DEF_FUNC_TBL(user_ftbl2)
    RCU_INC_FUNC_AUTONAME(user_f0,user_f0_init,user_f0_destroy,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f1,user_f1_init,user_f1_destroy,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f2,user_f2_init,user_f2_destroy,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f3,user_f3_init,user_f3_destroy,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f4,user_f4_init,user_f4_destroy,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f5,user_f5_init,user_f5_destroy,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

RCU_DEF_FUNC_TBL(user_ftbl3)
    RCU_INC_FUNC_AUTONAME(user_f103,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f104,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f105,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

RCU_DEF_FUNC_TBL(user_ftbl4)
    RCU_INC_FUNC_AUTONAME(user_f106,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f107,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f108,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

RCU_DEF_FUNC_TBL(user_ftbl5)
    RCU_INC_FUNC_AUTONAME(user_f109,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f110,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f111,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

//This is for the default test module
RCU_DEF_FUNC_TBL(user_ftbl6)
    RCU_INC_FUNC_AUTONAME(user_f112,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f113,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f114,RCU_NULL,RCU_NULL,RCU_FALSE)
    RCU_INC_FUNC_AUTONAME(user_f200,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f201,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f202,RCU_NULL,RCU_NULL,RCU_FALSE)
    RCU_INC_FUNC_AUTONAME(user_f300,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f301,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(user_f302,RCU_NULL,RCU_NULL,RCU_FALSE)

RCU_DEF_FUNC_TBL_END

RCU_DEF_MOD_TBL(user_mtbl1)
    RCU_INC_MOD("m0",RCU_NULL,RCU_NULL,user_ftbl0,RCU_TRUE)
    RCU_INC_MOD("m1",user_m1_init,user_m1_destroy,user_ftbl1,RCU_TRUE)
    RCU_INC_MOD("m2",user_m2_init,user_m2_destroy,user_ftbl2,RCU_TRUE)
    RCU_INC_MOD("m3",user_m3_init,user_m3_destroy,user_ftbl3,RCU_TRUE)
    RCU_INC_MOD("m4",user_m4_init,user_m4_destroy,user_ftbl4,RCU_TRUE)
    RCU_INC_MOD("m5",user_m4_init,user_m4_destroy,user_ftbl5,RCU_FALSE)
RCU_DEF_MOD_TBL_END

RCU_DEF_GENERIC_FUNC(rcu_test_run_001_out_hnd_int,param){
    fprintf(stdout,"rcu_test_run_001_out_hnd_int invoked\n");
}

RCU_DEF_GENERIC_FUNC(rcu_test_run_001_out_hnd_destroy,param){
    fprintf(stdout,"rcu_test_run_001_out_hnd_destroy invoked\n");
}

RCU_DEF_GENERIC_FUNC(rcu_test_run_001_out_hnd_out_impl,param){
RCU_OUTPUT_RECORD *out_rec = (RCU_OUTPUT_RECORD*)param;
    fprintf(stdout,"%s",out_rec->data);
}

DEF_LOCAL_TEST_FUNC(rcu_test_run_001){
RCU_DEF_TEST_MOD(m1)
RCU_DEF_TEST_MOD(m2)
RCU_DEF_TEST_MOD(m3)
RCU_DEF_TEST_REG(r1)
RCU_DEF_TEST_REG(r2)
RCU_OUTPUT_HANDLER out_hnd;
    rcu_init();
    out_hnd.init = rcu_test_run_001_out_hnd_int;
    out_hnd.destroy = rcu_test_run_001_out_hnd_destroy;
    out_hnd.out_impl = rcu_test_run_001_out_hnd_out_impl;
    rcu_add_out_hnd(&out_hnd);
    r1=rcu_cre_test_reg("r1",RCU_TRUE);
    rcu_add_test_reg(r1);
    r2=rcu_cre_test_reg("r2",RCU_FALSE);
    rcu_add_test_reg(r2);
    rcu_add_test_func_tbl(RCU_DEFAULT_MODULE,user_ftbl6);
    rcu_add_test_mod_tbl(RCU_DEFAULT_REGISTRY,user_mtbl1);
    rcu_dump_test_dbase();
    rcu_run_test_mach();
    rcu_destroy();
    return(0);
}

