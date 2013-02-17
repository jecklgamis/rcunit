/* Verify that  default registry is always available */
TMK_TEST(rcu_test_get_default_reg) {
//    TMK_ASSERT_NOT_NULL(rcu_get_default_reg());
//    TMK_ASSERT_EQUAL(rcu_get_default_reg(), rcu_get_reg("default-reg"));
}

/** Verify the the default name is reserved */
TMK_TEST(rcu_test_reserved_reg_name) {
    TMK_ASSERT_NULL(rcu_cre_test_reg("default-reg"));
}

/* Verify a null registry name is not allowed */
TMK_TEST(rcu_test_null_reg_name) {
    TMK_ASSERT_NULL(rcu_cre_test_reg(NULL));
}

/** Verify creation of registry */
TMK_TEST(rcu_test_create_reg) {
    TMK_ASSERT_NOT_NULL(rcu_cre_test_reg("r1"));
}

TMK_TEST(rcu_test_destroy_reg) {
    RCU_DEF_TEST_REG(r1)
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT(rcu_destroy_test_reg(r1) == RCU_E_OK);
}

#if 0
TMK_TEST(rcu_test_add_test_mod_) {
    RCU_DEF_TEST_REG(r1)
    RCU_DEF_TEST_MOD(m1)
    RCU_DEF_TEST_MOD(m2)
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT_NOT_NULL(m1 = rcu_cre_test_mod("m1", NULL, NULL));
    TMK_ASSERT_NOT_NULL(m2 = rcu_cre_test_mod("m2", NULL, NULL));
    TMK_ASSERT(rcu_add_test_mod(r1, m1) == RCU_E_OK);
    TMK_ASSERT(rcu_add_test_mod(r1, m2) == RCU_E_OK);
    rcu_run_tests();
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

TMK_TEST(rcu_test_reg_001) {
    RCU_DEF_TEST_REG(r1)
    RCU_DEF_TEST_MOD(m1)
    RCU_DEF_TEST_MOD(m2)
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT_NOT_NULL(m1 = rcu_cre_test_mod("m1", NULL, NULL));
    TMK_ASSERT_NOT_NULL(m2 = rcu_cre_test_mod("m2", NULL, NULL));
    TMK_ASSERT(rcu_add_test_mod(r1, m1) == RCU_E_OK);
    TMK_ASSERT(rcu_add_test_mod(r1, m2) == RCU_E_OK);
    rcu_run_tests();
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

TMK_TEST(rcu_test_reg_004) {
    RCU_DEF_TEST_REG(r1)
    RCU_DEF_TEST_REG(r2)
    TMK_ASSERT(rcu_init() == RCU_E_OK);
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT_NOT_NULL(r2 = rcu_cre_test_reg("r1"));
    TMK_ASSERT(rcu_destroy_test_reg(r2) == RCU_E_OK);
    TMK_ASSERT(rcu_destroy_test_reg(r1) == RCU_E_OK);
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}


TMK_TEST(rcu_test_reg_007) {
    RCU_DEF_TEST_REG(r1)
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT(rcu_destroy_test_reg(r1) == RCU_E_OK);

}

TMK_TEST(rcu_test_reg_008) {
    RCU_DEF_TEST_REG(r1)
    TMK_ASSERT(rcu_init() == RCU_E_OK);
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

TMK_TEST(rcu_test_reg_009) {
    RCU_DEF_TEST_REG(r1)
    TMK_ASSERT(rcu_init() == RCU_E_OK);
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

TMK_TEST(rcu_test_reg_010) {
    RCU_DEF_TEST_REG(r1)
    TMK_ASSERT(rcu_init() == RCU_E_OK);
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

TMK_TEST(rcu_test_reg_011) {
    RCU_DEF_TEST_REG(r1)
    TMK_ASSERT(rcu_init() == RCU_E_OK);
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

TMK_TEST(rcu_test_reg_012) {
    RCU_DEF_TEST_REG(r1)
    RCU_DEF_TEST_REG(r2)
    TMK_ASSERT(rcu_init() == RCU_E_OK);
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT_NOT_NULL(r2 = rcu_cre_test_reg("r2"));
    TMK_ASSERT(rcu_destroy_test_reg(r2) == RCU_E_OK);
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}

TMK_TEST(rcu_test_reg_013) {
    RCU_DEF_TEST_REG(r1)
    TMK_ASSERT(rcu_init() == RCU_E_OK);
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    rcu_run_tests();
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);

}

TMK_TEST(rcu_test_reg_014) {
    RCU_DEF_TEST_REG(r1)
    TMK_ASSERT(rcu_init() == RCU_E_OK);
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT(rcu_add_test_reg(r1) == RCU_E_OK);
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);

}

TMK_TEST(rcu_test_reg_015) {
    RCU_DEF_TEST_REG(r1)
    RCU_DEF_TEST_REG(r2)
    TMK_ASSERT(rcu_init() == RCU_E_OK);
    TMK_ASSERT_NOT_NULL(r1 = rcu_cre_test_reg("r1"));
    TMK_ASSERT_NOT_NULL(r2 = rcu_cre_test_reg("r2"));
    TMK_ASSERT(rcu_add_test_reg(r1) == RCU_E_OK);
    TMK_ASSERT(rcu_destroy_test_reg(r2) == RCU_E_OK);
    TMK_ASSERT(rcu_destroy() == RCU_E_OK);
}


#endif