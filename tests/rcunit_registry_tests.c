/* Verify that default registry is always available */
TMK_TEST(rcu_test_get_default_reg) {
    TMK_ASSERT_NOT_NULL(rcu_get_default_reg());
}
