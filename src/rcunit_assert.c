/*
 * The MIT License (MIT)
 * 
 * RCUNIT - A unit testing framework for C.
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "rcunit.h"

#define RCU_TEMP_BUFF_SIZE 1024
static char g_temp_buff[RCU_TEMP_BUFF_SIZE];

void rcu_assert_impl(int cond, const char *filename, const char *func_name,
                     int line_no, const char *format, ...) {
    rcu_test_machine *machine = NULL;
    rcu_test *func = NULL;
    rcu_module *mod = NULL;
    int run_ctx;
    char assert_msg_buff[RCU_TEMP_BUFF_SIZE];

    va_list ap;
    memset(&assert_msg_buff[0], 0, RCU_TEMP_BUFF_SIZE);
    va_start(ap, format);
    vsprintf(assert_msg_buff, format, ap);
    va_end(ap);

    rcu_init();
    machine = &the_test_machine;

    func = RCU_GET_CURR_FUNC(machine);
    mod = RCU_GET_CURR_MOD(machine);
    run_ctx = RCU_GET_RUN_CTX(machine);

    switch (run_ctx) {
        case RCU_RUN_CTX_MOD_INIT:
        case RCU_RUN_CTX_MOD_DESTROY:
            if (!cond) {
                RCU_LOG_ERROR("Assert failed in %s (%s:%d) : %s", func_name,
                              filename, line_no, assert_msg_buff);
                if (run_ctx == RCU_RUN_CTX_MOD_INIT) {
                    RCU_SET_INIT_FAILED(mod);
                } else {
                    RCU_SET_DESTROY_FAILED(mod);
                }
                rcu_add_fail_rec_to_mod(mod, assert_msg_buff, filename, line_no, RCU_TRUE);
                RCU_SET_RUN_CTX(machine, RCU_RUN_CTX_UNKNOWN);
                RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ABORTMODRUN));
            }
            break;
        case RCU_RUN_CTX_FUNC_INIT:
        case RCU_RUN_CTX_FUNC_DESTROY:
            if (!cond) {
                RCU_LOG_ERROR("Assert failed in %s (%s:%d) : %s", func_name,
                              filename, line_no, assert_msg_buff);
                if (run_ctx == RCU_RUN_CTX_MOD_INIT) {
                    RCU_SET_INIT_FAILED(func);
                } else {
                    RCU_SET_DESTROY_FAILED(func);
                }
                rcu_add_fail_rec_to_func(func, assert_msg_buff, filename, line_no);
                RCU_SET_RUN_CTX(machine, RCU_RUN_CTX_UNKNOWN);
                RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ASSERTIONFAILURE));
            }
            break;
        case RCU_RUN_CTX_FUNC:
            if (!strcmp(func->name, "set_on_run")) {
                memset(func->name, 0, RCU_TEST_FUNCTION_NAME_LENGTH);
                sprintf(func->name, "%s", func_name);
            }
            if (!cond) {
                RCU_LOG_ERROR("Assert failed in %s (%s:%d) : %s", func_name,
                              filename, line_no, assert_msg_buff);
                RCU_INCR(func->nr_fail_assert);
                RCU_SET_RUN_STAT(func, RCU_RUN_STAT_TEST_FAILED);
                rcu_add_fail_rec_to_func(func, assert_msg_buff, filename, line_no);
                RCU_SET_RUN_CTX(machine, RCU_RUN_CTX_UNKNOWN);
                RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ASSERTIONFAILURE));
            } else {
                RCU_INCR(func->nr_succ_assert);
            }
            break;
        default:
            if (!cond) {
                RCU_LOG_ERROR("Assert failed in non-test function %s (%s:%d)",
                              func_name, filename, line_no);
                rcu_add_fail_rec_impl(&machine->ae.assert_list, assert_msg_buff, filename, func_name, line_no);
            }
    }

}

RCU_API int rcu_have_asserts() {
    rcu_init();
    rcu_test_machine *machine = &the_test_machine;
    return (!rcu_is_list_empty(&machine->ae.assert_list));
}

RCU_API void rcu_dump_asserts() {
    rcu_list *cursor = NULL;
    rcu_test_machine *machine = NULL;
    rcu_failure_record *fail_rec = NULL;
    rcu_init();
    machine = &the_test_machine;
    if (!rcu_have_asserts()) {
        RCU_LOG_WARN("No assertions to dump found");
    }
    RCU_LOG_WARN("[ NON TEST RUN ASSERTIONS DUMP START]");

    RCU_FOR_EACH_ENTRY(&machine->ae.assert_list, cursor) {
        fail_rec = (rcu_failure_record *) cursor;
        RCU_LOG_WARN("%s", fail_rec->info);
    }
    RCU_LOG_WARN("[ NON TEST RUN ASSERTIONS DUMP END]");
}

int rcu_restart_assert_engine(rcu_test_machine *machine) {
    memset(&machine->ae, 0x00, sizeof(rcu_assertion_engine));
    RCU_LOG_DEBUG("Assertion engine restarted");
    return RCU_E_OK;
}

int rcu_stop_assert_engine(rcu_test_machine *machine) {
    RCU_LOG_DEBUG("Assertion engine stopped");
    return RCU_E_OK;
}

RCU_API int rcu_set_assert_hook(rcu_generic_function assert_hook) {
    rcu_test_machine *machine = &the_test_machine;
    rcu_init();
    if (assert_hook != NULL) {
        machine->ae.assert_hook = assert_hook;
        RCU_LOG_DEBUG("Assertion hook set");
    }
    return RCU_E_OK;
}

rcu_failure_record *rcu_cre_fail_rec(const char *info, const char *filename, const char *func_name, int line_no) {
    rcu_failure_record *fail_rec = NULL;
    int info_len;
    const char *error_str = NULL;

    if (filename == NULL) {
        return NULL;
    }
    if ((fail_rec = rcu_malloc(sizeof(rcu_failure_record))) == NULL) {
        return NULL;
    }
    memset(g_temp_buff, 0, RCU_TEMP_BUFF_SIZE);
    error_str = "failed";
    sprintf(g_temp_buff, "%s(%d) : %s", filename, line_no, info);
    info_len = strlen(g_temp_buff) + 1;

    if ((fail_rec->info = rcu_malloc(info_len)) == NULL) {
        rcu_free(fail_rec);
        return NULL;
    }
    rcu_init_list(&fail_rec->link);
    strcpy(fail_rec->info, g_temp_buff);
    return fail_rec;
}

int rcu_add_fail_rec_impl(rcu_list *fail_rec_list, const char *info, const char *filename, const char *func_name,
                          int line_no) {
    rcu_failure_record *fail_rec = NULL;
    rcu_test_machine *machine = NULL;

    if (fail_rec_list == NULL) {
        return RCU_E_NG;
    }
    if (filename == NULL) {
        return RCU_E_NG;
    }
    if ((fail_rec = rcu_cre_fail_rec(info, filename, func_name, line_no)) == NULL) {
        return RCU_E_NG;
    }
    rcu_insert_list(fail_rec_list, &fail_rec->link);
    machine = &the_test_machine;
    if (machine->run_ctx == RCU_RUN_CTX_UNKNOWN) {
        if (machine->ae.assert_hook != NULL) {
            machine->ae.assert_hook(fail_rec);
        }
    }
    return RCU_E_OK;
}

int rcu_del_all_fail_rec_impl(rcu_list *fail_rec_list) {
    rcu_failure_record *fail_rec = NULL;
    rcu_list *cursor = NULL;

    if (fail_rec_list == NULL) {
        return RCU_E_NG;
    }

    RCU_FOR_EACH_ENTRY(fail_rec_list, cursor) {
        fail_rec = (rcu_failure_record *) cursor;
        rcu_free(fail_rec->info);
        RCU_SAVE_CURSOR(cursor)
            rcu_remove_list(cursor);
            rcu_free(fail_rec);
        RCU_RESTORE_CURSOR(cursor)
    }
    return RCU_E_OK;
}

int rcu_del_all_fail_rec(rcu_test_machine *machine) {
    rcu_list *cursor1 = NULL;
    rcu_list *cursor2 = NULL;
    rcu_list *cursor3 = NULL;
    rcu_list *cursor4 = NULL;
    rcu_test *func = NULL;
    rcu_module *mod = NULL;
    rcu_registry *reg = NULL;

    RCU_LOG_DEBUG("Destroying all failure records from test runs");
    if (machine == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVMACH));
        return RCU_E_NG;
    }

    RCU_FOR_EACH_ENTRY(&machine->reg_list, cursor1) {
        reg = (rcu_registry *) cursor1;

        RCU_FOR_EACH_ENTRY(&reg->mod_list, cursor2) {
            mod = (rcu_module *) cursor2;

            RCU_FOR_EACH_ENTRY(&mod->func_list, cursor3) {
                func = (rcu_test *) cursor3;
                rcu_del_all_fail_rec_from_func(func);
            }
            rcu_del_all_fail_rec_from_mod(mod);
        }
    }
    RCU_LOG_DEBUG("Destroying all failure records from non-test runs");
    rcu_del_all_fail_rec_impl(&machine->ae.assert_list);
    return RCU_E_OK;
}
