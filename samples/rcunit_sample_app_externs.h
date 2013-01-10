#ifndef RCUNIT_SAMPLE_APP_EXTERNS_H
#define RCUNIT_SAMPLE_APP_EXTERNS_H

extern int calc_sum(int x, int y);
extern int calc_diff(int x, int y);
extern int calc_product(int x, int y);
extern int calc_quotient(int x, int y, int *quotient);
extern int get_min(int x, int y);
extern int get_max(int x, int y);

RCU_DEC_TEST_FUNC(test_calc_sum,param);
RCU_DEC_TEST_FUNC(test_calc_diff,param);
RCU_DEC_TEST_FUNC(test_calc_product,param);
RCU_DEC_TEST_FUNC(test_calc_quotient,param);
RCU_DEC_TEST_FUNC(test_get_min,param);
RCU_DEC_TEST_FUNC(test_get_max,param);
RCU_DEC_TEST_FUNC_TBL(calc_xxx_test_tbl);
RCU_DEC_TEST_FUNC_TBL(get_xxx_test_tbl);
RCU_DEC_TEST_MOD_TBL(sample_test_mod_tbl);

#endif /* RCUNIT_SAMPLE_APP_EXTERNS_H  */

