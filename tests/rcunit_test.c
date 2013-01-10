#include <rcunit.h>
#include <rcunit_test.h>

#define RCU_ENABLE_RUN_ALL_TESTS 0
#define RCU_ENABLE_RUN_ALL_TESTS_REPETITION 1

extern RCU_INT rcunit_sample_app_main(RCU_INT argc,RCU_CHAR **argv);
extern RCU_INT rcunit_sample_app_2_main(RCU_INT argc,RCU_CHAR **argv);

RCU_U4 g_nr_fail;

RCU_INT local_assert_impl(RCU_INT cond, RCU_INT abort,
    const RCU_CHAR *info1, const RCU_CHAR *info2, const RCU_CHAR *fname, RCU_INT line){
    if (!cond){
        RCU_LOG_WARN_P3("[Local-Test] Failed: [%s]: (%s:%d)",info1, fname,line);
        g_nr_fail++;
    }else{
        RCU_LOG_INFO_P3("[Local-Test] Passed: [%s]: (%s:%d)",info1, fname,line);
    }
    if (abort){
        RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ABORTTESTRUN));
    }
    return(RCU_E_OK);
}

RCU_INT run_local_test(LOCAL_TEST_ENTRY *tbl){
LOCAL_TEST_ENTRY *cursor;
    RCU_LOG_INFO("[Local-Test] Started.");
    g_nr_fail = 0;
    for(cursor = tbl;cursor->name!=RCU_NULL;cursor++){
        if (cursor->enable){
            RCU_LOG_INFO_P1("[Local-Test] Running test : %s",cursor->name);
            RCU_TRY{
                cursor->entry(0,RCU_NULL);
            }RCU_CATCH(e){
                RCU_LOG_WARN("[Local-Test] Test aborted");
                continue;
            }RCU_END_CATCH
        }else{
            RCU_LOG_WARN_P1("[Local-Test]Disabled test : %s",cursor->name);
        }
    }
    RCU_LOG_INFO_P1("[Local-Test] Failures : %lu",g_nr_fail);
    RCU_LOG_INFO("[Local-Test] Finished.");
    return(RCU_E_OK);
}

#if RCU_ENABLE_RUN_ALL_TESTS
#define INCLUDE_FRAMEWORK_ITEMS      1
#define INCLUDE_TEST_REGISTRY_ITEMS  1
#define INCLUDE_TEST_MODULE_ITEMS    1
#define INCLUDE_TEST_FUNCTION_ITEMS  1
#define INCLUDE_TEST_ASSERTION_ITEMS 1
#define INCLUDE_TEST_LOGGING_ITEMS   1
#define INCLUDE_TEST_MTRACE_ITEMS    1
#define INCLUDE_TEST_EXCEPTION_ITEMS 1
#define INCLUDE_TEST_RUN_ITEMS       1
#define INCLUDE_TEST_ANALYSIS_ITEMS  1
#define INCLUDE_TEST_REDIRECT_ITEMS  1
#define INCLUDE_TEST_RANDOM_EMELGEEK_ITEMS  1
#else
#define INCLUDE_FRAMEWORK_ITEMS      0
#define INCLUDE_TEST_REGISTRY_ITEMS  0
#define INCLUDE_TEST_MODULE_ITEMS    0
#define INCLUDE_TEST_FUNCTION_ITEMS  0
#define INCLUDE_TEST_ASSERTION_ITEMS 0
#define INCLUDE_TEST_LOGGING_ITEMS   0
#define INCLUDE_TEST_MTRACE_ITEMS    0
#define INCLUDE_TEST_EXCEPTION_ITEMS 0
#define INCLUDE_TEST_RUN_ITEMS       1
#define INCLUDE_TEST_ANALYSIS_ITEMS  0
#define INCLUDE_TEST_REDIRECT_ITEMS  0
#define INCLUDE_TEST_RANDOM_EMELGEEK_ITEMS  0
#endif

#if INCLUDE_FRAMEWORK_ITEMS
#include "rcunit_test_framework.c"
#endif

#if INCLUDE_TEST_REGISTRY_ITEMS
#include "rcunit_test_registry.c"
#endif

#if INCLUDE_TEST_MODULE_ITEMS
#include "rcunit_test_module.c"
#endif

#if INCLUDE_TEST_FUNCTION_ITEMS
#include "rcunit_test_function.c"
#endif

#if INCLUDE_TEST_ASSERTION_ITEMS
#include "rcunit_test_assertions.c"
#endif

#if INCLUDE_TEST_LOGGING_ITEMS
#include "rcunit_test_logging.c"
#endif

#if INCLUDE_TEST_MTRACE_ITEMS
#include "rcunit_test_mtrace.c"
#endif

#if INCLUDE_TEST_EXCEPTION_ITEMS
#include "rcunit_test_exceptions.c"
#endif

#if INCLUDE_TEST_RUN_ITEMS
#include "rcunit_test_run.c"
#endif

#if INCLUDE_TEST_ANALYSIS_ITEMS
#include "rcunit_test_analysis.c"
#endif

#if INCLUDE_TEST_REDIRECT_ITEMS
#include "rcunit_test_redirect.c"
#endif

#if INCLUDE_TEST_RANDOM_EMELGEEK_ITEMS
#include "rcunit_test_random_emelgeek.c"
#endif

DEF_LOCAL_TEST_TABLE_START(local_test_table)
    /* test machine/framework */
#if INCLUDE_FRAMEWORK_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_frmwk_001,1)
    INC_LOCAL_TEST_FUNC(rcu_test_frmwk_002,1)
    INC_LOCAL_TEST_FUNC(rcu_test_frmwk_003,1)
    INC_LOCAL_TEST_FUNC(rcu_test_frmwk_004,1)
    INC_LOCAL_TEST_FUNC(rcu_test_frmwk_005,1)
    INC_LOCAL_TEST_FUNC(rcu_test_frmwk_006,1)
    INC_LOCAL_TEST_FUNC(rcu_test_frmwk_007,1)
    INC_LOCAL_TEST_FUNC(rcu_test_frmwk_008,1)
    INC_LOCAL_TEST_FUNC(rcu_test_frmwk_009,1)
#endif
    /* test registry */
#if INCLUDE_TEST_REGISTRY_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_reg_001,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_002,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_003,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_004,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_005,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_006,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_007,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_008,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_009,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_010,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_011,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_012,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_013,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_014,1)
    INC_LOCAL_TEST_FUNC(rcu_test_reg_015,1)
#endif
    /* test module */
#if INCLUDE_TEST_MODULE_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_mod_001,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_002,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_003,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_004,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_005,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_006,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_007,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_008,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_009,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_010,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_011,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_012,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_013,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_014,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_015,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_016,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_017,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_018,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_019,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_020,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_021,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_022,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_023,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_024,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_025,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_026,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_027,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_028,1)
    INC_LOCAL_TEST_FUNC(rcu_test_mod_029,1)
#endif
    /* test function */
#if INCLUDE_TEST_FUNCTION_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_func_001,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_002,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_003,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_004,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_004,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_005,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_006,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_007,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_008,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_009,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_010,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_011,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_012,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_013,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_014,1)
    INC_LOCAL_TEST_FUNC(rcu_test_func_015,1)
#endif
    /* assertion macros */
#if INCLUDE_TEST_ASSERTION_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_assert_001,1)
    INC_LOCAL_TEST_FUNC(rcu_test_assert_002,1)
#endif

    /* logging facility */
#if INCLUDE_TEST_LOGGING_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_logging_001,1)
#endif

    /* memory allocation tracing */
#if INCLUDE_TEST_MTRACE_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_mtrace_001,1)
#endif

    /* exception facility */
#if INCLUDE_TEST_EXCEPTION_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_excp_001,1)
#endif

    /* test run statistics */
#if INCLUDE_TEST_RUN_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_run_001,1)
#endif

#if INCLUDE_TEST_ANALYSIS_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_analysis_001,1)
#endif

#if INCLUDE_TEST_REDIRECT_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_redirect_001,1)
#endif

#if INCLUDE_TEST_RANDOM_EMELGEEK_ITEMS
    INC_LOCAL_TEST_FUNC(rcu_test_random_emelgeek_001,1)
#endif

DEF_LOCAL_TEST_TABLE_END

RCU_INT main(RCU_INT argc,RCU_CHAR **argv){
#if RCU_ENABLE_RUN_ALL_TESTS
int a=0;
    for (a = 0; a != RCU_ENABLE_RUN_ALL_TESTS_REPETITION; a++){
        run_local_test(local_test_table);
    }
#else
    run_local_test(local_test_table);
#endif
    return(RCU_E_OK);
}
