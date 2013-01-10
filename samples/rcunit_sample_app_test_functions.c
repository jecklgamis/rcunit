#include <rcunit.h>
#include <rcunit_sample_app_externs.h>

/**=====================================================================
 * Test functions
 *====================================================================*/

/** Test function for calc_sum */
RCU_DEF_TEST_FUNC(test_calc_sum,param){
    RCU_ASSERT(calc_sum(-1,-1) == -2);
    RCU_ASSERT(calc_sum(0,0) == 0);
    RCU_ASSERT(calc_sum(1,1) == 2);
}

/** Test function for calc_diff */
RCU_DEF_TEST_FUNC(test_calc_diff,param){
    RCU_ASSERT(calc_diff(-1,-2) == 1);
    RCU_ASSERT(calc_diff(0,1) == -1);
    RCU_ASSERT(calc_diff(2,1) == 1);
}

/** Test function for calc_product */
RCU_DEF_TEST_FUNC(test_calc_product,param){
int quotient= 0;
    RCU_ASSERT(calc_product(-1,-2) == 2);
    RCU_ASSERT(calc_product(0,1) == 0);
    RCU_ASSERT(calc_product(2,1) == 2);
}

/** Test function for calc_quotient */
RCU_DEF_TEST_FUNC(test_calc_quotient,param){
int quotient= 0;
int ercd;
    ercd = calc_quotient(12,3,&quotient);
    RCU_ASSERT(quotient == 4 && ercd == 0);

    ercd = calc_quotient(1,0,&quotient);
    RCU_ASSERT(ercd == -1);

    ercd = calc_quotient(1,1,&quotient);
    RCU_ASSERT(quotient == 1 && ercd == 0);
}

/** Test function for get_min */
RCU_DEF_TEST_FUNC(test_get_min,param){
    RCU_ASSERT(get_min(-1,-2) == -2);
    RCU_ASSERT(get_min(0,0) == 0);
    RCU_ASSERT(get_min(2,3) == 2);
}

/** Test function for get_max */
RCU_DEF_TEST_FUNC(test_get_max,param){
    RCU_ASSERT(get_max(-1,-2) == -1);
    RCU_ASSERT(get_max(0,0) == 0);
    RCU_ASSERT(get_max(2,3) == 3);
}

/** Test function table for calc_xxx functions */
RCU_DEF_FUNC_TBL(calc_xxx_test_tbl)
    RCU_INC_FUNC_AUTONAME(test_calc_sum,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(test_calc_diff,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(test_calc_product,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(test_calc_quotient,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

/** Test function table for get_xxx functions */
RCU_DEF_FUNC_TBL(get_xxx_test_tbl)
    RCU_INC_FUNC_AUTONAME(test_get_min,RCU_NULL,RCU_NULL,RCU_TRUE)
    RCU_INC_FUNC_AUTONAME(test_get_max,RCU_NULL,RCU_NULL,RCU_TRUE)
RCU_DEF_FUNC_TBL_END

/** Test module table for calc_xxx and get_xxx functions */
RCU_DEF_MOD_TBL(sample_test_mod_tbl)
    RCU_INC_MOD("m1",RCU_NULL,RCU_NULL,get_xxx_test_tbl,RCU_TRUE)
    RCU_INC_MOD("m2",RCU_NULL,RCU_NULL,calc_xxx_test_tbl,RCU_TRUE)
RCU_DEF_MOD_TBL_END
