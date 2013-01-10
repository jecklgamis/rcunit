#define NR_ALLOCS 1024


DEF_LOCAL_TEST_FUNC(rcu_test_mtrace_002){
RCU_INT a=0;
RCU_VOID *ptr[NR_ALLOCS];
    rcu_init();
    for(a=0;a<NR_ALLOCS;a++){
        ptr[a] = rcu_malloc(a+1);
    }
    for(a=0;a<NR_ALLOCS;a++){
        rcu_free(ptr[a]);
    }
    rcu_destroy();
    return(RCU_E_OK);
}

#define XXX_FREE(ptr,flag) \
if (flag){ \
        rcu_free(ptr); \
    }

DEF_LOCAL_TEST_FUNC(rcu_test_mtrace_001){
RCU_INT a=0;
RCU_VOID *ptr[16];
    rcu_init();
    ptr[0] = rcu_malloc(4);
    ptr[1] = rcu_malloc(4);
    ptr[2] = rcu_malloc(4);
    ptr[3] = rcu_malloc(4);
    ptr[4] = rcu_malloc(4);
    ptr[5] = rcu_malloc(4);
    ptr[6] = rcu_malloc(4);
    ptr[7] = rcu_malloc(4);
    ptr[8] = rcu_malloc(4);
    ptr[9] = rcu_malloc(4);
    ptr[10]= rcu_malloc(4);
    ptr[11]= rcu_malloc(4);
    ptr[12]= rcu_malloc(4);
    ptr[13]= rcu_malloc(4);
    ptr[14]= rcu_malloc(4);
    ptr[15]= rcu_malloc(4);
    ptr[16]= rcu_malloc(4);

    XXX_FREE(ptr[0],1);
    XXX_FREE(ptr[1],1);
    XXX_FREE(ptr[2],1);
    XXX_FREE(ptr[3],0);
    XXX_FREE(ptr[4],0);
    XXX_FREE(ptr[5],1);
    XXX_FREE(ptr[6],1);
    XXX_FREE(ptr[7],1);
    XXX_FREE(ptr[8],0);
    XXX_FREE(ptr[9],0);
    XXX_FREE(ptr[10],0);
    XXX_FREE(ptr[11],0);
    XXX_FREE(ptr[12],0);
    XXX_FREE(ptr[13],0);
    XXX_FREE(ptr[14],0);
    XXX_FREE(ptr[15],0);
    XXX_FREE(ptr[16],0);

    rcu_destroy();
    return(RCU_E_OK);
}





