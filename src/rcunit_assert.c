/*
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rcunit.h"
#include <unistd.h>

#define RCU_TEMP_BUFF_SIZE 1024
#define RCU_FILE_PATH_BUFF_SIZE 2048
static char g_temp_buff[RCU_TEMP_BUFF_SIZE];

static int rcu_format_file_path(char *buff, size_t buff_len, const char *filename, int line_no) {
    char cwd[RCU_FILE_PATH_BUFF_SIZE];
    char filepath[RCU_FILE_PATH_BUFF_SIZE];
    char relpath[RCU_FILE_PATH_BUFF_SIZE];
    const char *path = filename;
    int nr_written;

    if (!buff || buff_len == 0 || !filename) {
        return RCU_E_NG;
    }

    if (filename[0] != '/') {
        if (!getcwd(cwd, sizeof(cwd))) {
            return RCU_E_NG;
        }
        while (path[0] == '.' && path[1] == '/') {
            path += 2;
        }

        nr_written = snprintf(filepath, sizeof(filepath), "%s/%s", cwd, path);
        if (nr_written < 0 || (size_t) nr_written >= sizeof(filepath)) {
            return RCU_E_NG;
        }
        if (access(filepath, F_OK) != 0) {
            const char *source_dirs[] = {"examples", "tests", "src"};
            unsigned int a;

            for (a = 0; a < sizeof(source_dirs) / sizeof(source_dirs[0]); a++) {
                nr_written = snprintf(filepath, sizeof(filepath), "%s/%s/%s", cwd, source_dirs[a], path);
                if (nr_written < 0 || (size_t) nr_written >= sizeof(filepath)) {
                    return RCU_E_NG;
                }
                if (access(filepath, F_OK) == 0) {
                    nr_written = snprintf(relpath, sizeof(relpath), "%s/%s", source_dirs[a], path);
                    if (nr_written < 0 || (size_t) nr_written >= sizeof(relpath)) {
                        return RCU_E_NG;
                    }
                    path = relpath;
                    break;
                }
            }
        }
        nr_written = snprintf(buff, buff_len, "%s:%d", path, line_no);
    } else {
        nr_written = snprintf(buff, buff_len, "%s:%d", path, line_no);
    }

    if (nr_written < 0 || (size_t) nr_written >= buff_len) {
        return RCU_E_NG;
    }
    return RCU_E_OK;
}

static void rcu_log_assert_failure(const char *func_name, const char *filename, int line_no,
                                   const char *assert_msg) {
    char file_path[RCU_FILE_PATH_BUFF_SIZE];

    if (rcu_format_file_path(file_path, sizeof(file_path), filename, line_no) == RCU_E_OK) {
        RCU_LOG_ERROR("Assert failed in %s (%s) : %s", func_name, file_path, assert_msg);
    } else {
        RCU_LOG_ERROR("Assert failed in %s (%s:%d) : %s", func_name, filename, line_no, assert_msg);
    }
}

static void rcu_log_non_test_assert_failure(const char *func_name, const char *filename, int line_no) {
    char file_path[RCU_FILE_PATH_BUFF_SIZE];

    if (rcu_format_file_path(file_path, sizeof(file_path), filename, line_no) == RCU_E_OK) {
        RCU_LOG_ERROR("Assert failed in non-test function %s (%s)", func_name, file_path);
    } else {
        RCU_LOG_ERROR("Assert failed in non-test function %s (%s:%d)", func_name, filename, line_no);
    }
}

void rcu_assert_impl(int cond, const char *filename, const char *func_name,
                     int line_no, const char *format, ...) {
    struct rcu_test_engine *engine = NULL;
    struct rcu_test *func = NULL;
    struct rcu_module *module = NULL;
    int run_ctx;
    char assert_msg_buff[RCU_TEMP_BUFF_SIZE];

    va_list ap;
    memset(&assert_msg_buff[0], 0, RCU_TEMP_BUFF_SIZE);
    va_start(ap, format);
    vsnprintf(assert_msg_buff, RCU_TEMP_BUFF_SIZE, format, ap);
    va_end(ap);

    rcu_init();
    engine = &the_test_engine;

    func = RCU_GET_CURRENT_FUNC(engine);
    module = RCU_GET_CURRENT_MODULE(engine);
    run_ctx = RCU_GET_RUN_CTX(engine);

    switch (run_ctx) {
        case RCU_RUN_CTX_MODULE_INIT:
        case RCU_RUN_CTX_MODULE_DESTROY:
            if (!cond) {
                rcu_log_assert_failure(func_name, filename, line_no, assert_msg_buff);
                if (run_ctx == RCU_RUN_CTX_MODULE_INIT) {
                    RCU_SET_INIT_FAILED(module);
                } else {
                    RCU_SET_DESTROY_FAILED(module);
                }
                rcu_add_fail_rec_to_module(module, assert_msg_buff, filename, line_no, RCU_TRUE);
                RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_UNKNOWN);
                RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ABORTMODRUN));
            }
            break;
        case RCU_RUN_CTX_FUNC_INIT:
        case RCU_RUN_CTX_FUNC_DESTROY:
            if (!cond) {
                rcu_log_assert_failure(func_name, filename, line_no, assert_msg_buff);
                if (run_ctx == RCU_RUN_CTX_MODULE_INIT) {
                    RCU_SET_INIT_FAILED(func);
                } else {
                    RCU_SET_DESTROY_FAILED(func);
                }
                rcu_add_fail_rec_to_func(func, assert_msg_buff, filename, line_no);
                RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_UNKNOWN);
                RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ASSERTIONFAILURE));
            }
            break;
        case RCU_RUN_CTX_FUNC:
            if (!strcmp(func->name, "set_on_run")) {
                memset(func->name, 0, RCU_TEST_FUNCTION_NAME_LENGTH);
                sprintf(func->name, "%s", func_name);
            }
            if (!cond) {
                rcu_log_assert_failure(func_name, filename, line_no, assert_msg_buff);
                RCU_INCR(func->nr_fail_assert);
                func->run_stat = RCU_RUN_STAT_TEST_FAILED;
                rcu_add_fail_rec_to_func(func, assert_msg_buff, filename, line_no);
                RCU_SET_RUN_CTX(engine, RCU_RUN_CTX_UNKNOWN);
                RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ASSERTIONFAILURE));
            } else {
                RCU_INCR(func->nr_succ_assert);
            }
            break;
        default:
            if (!cond) {
                rcu_log_non_test_assert_failure(func_name, filename, line_no);
                rcu_add_fail_rec_impl(&engine->ae.assert_list, assert_msg_buff, filename, func_name, line_no);
            }
    }

}

int rcu_have_asserts() {
    rcu_init();
    struct rcu_test_engine *engine = &the_test_engine;
    return (!rcu_is_list_empty(&engine->ae.assert_list));
}

void rcu_dump_asserts() {
    struct rcu_list *cursor = NULL;
    struct rcu_test_engine *engine = NULL;
    struct rcu_failure_record *fail_rec = NULL;
    rcu_init();
    engine = &the_test_engine;
    if (!rcu_have_asserts()) {
        RCU_LOG_WARN("No assertions to dump found");
    }
    RCU_LOG_WARN("[ NON TEST RUN ASSERTIONS DUMP START]");

    RCU_FOR_EACH_ENTRY(&engine->ae.assert_list, cursor) {
        fail_rec = (struct rcu_failure_record *) cursor;
        RCU_LOG_WARN("%s", fail_rec->info);
    }
    RCU_LOG_WARN("[ NON TEST RUN ASSERTIONS DUMP END]");
}

int rcu_restart_assert_engine(struct rcu_test_engine *engine) {
    memset(&engine->ae, 0, sizeof(struct rcu_assertion_engine));
    RCU_LOG_DEBUG("Assertion engine restarted");
    return RCU_E_OK;
}

int rcu_stop_assert_engine(struct rcu_test_engine *engine) {
    RCU_LOG_DEBUG("Assertion engine stopped");
    return RCU_E_OK;
}

int rcu_set_assert_hook(rcu_generic_function assert_hook) {
    struct rcu_test_engine *engine = &the_test_engine;
    rcu_init();
    if (assert_hook) {
        engine->ae.assert_hook = assert_hook;
        RCU_LOG_DEBUG("Assertion hook set");
    }
    return RCU_E_OK;
}

struct rcu_failure_record *rcu_create_fail_rec(const char *info, const char *filename, const char *func_name, int line_no) {
    struct rcu_failure_record *fail_rec = NULL;
    int info_len;
    const char *error_str = NULL;

    if (!filename) {
        return NULL;
    }
    if (!(fail_rec = rcu_malloc(sizeof(struct rcu_failure_record)))) {
        return NULL;
    }
    memset(g_temp_buff, 0, RCU_TEMP_BUFF_SIZE);
    error_str = "failed";
    snprintf(g_temp_buff, RCU_TEMP_BUFF_SIZE, "%s(%d) : %s", filename, line_no, info);
    info_len = strlen(g_temp_buff) + 1;

    if ((!(fail_rec->info = rcu_malloc(info_len)))) {
        rcu_free(fail_rec);
        return NULL;
    }
    rcu_init_list(&fail_rec->link);
    memcpy(fail_rec->info, g_temp_buff, info_len);
    return fail_rec;
}

int rcu_add_fail_rec_impl(struct rcu_list *fail_rec_list, const char *info, const char *filename, const char *func_name,
                          int line_no) {
    struct rcu_failure_record *fail_rec = NULL;
    struct rcu_test_engine *engine = NULL;

    if (!fail_rec_list) {
        return RCU_E_NG;
    }
    if (!filename) {
        return RCU_E_NG;
    }
    if ((!(fail_rec = rcu_create_fail_rec(info, filename, func_name, line_no)))) {
        return RCU_E_NG;
    }
    rcu_insert_list(fail_rec_list, &fail_rec->link);
    engine = &the_test_engine;
    if (engine->run_ctx == RCU_RUN_CTX_UNKNOWN) {
        if (engine->ae.assert_hook) {
            engine->ae.assert_hook(fail_rec);
        }
    }
    return RCU_E_OK;
}

int rcu_del_all_fail_rec_impl(struct rcu_list *fail_rec_list) {
    struct rcu_failure_record *fail_rec = NULL;
    struct rcu_list *cursor = NULL;

    if (!fail_rec_list) {
        return RCU_E_NG;
    }

    RCU_FOR_EACH_ENTRY(fail_rec_list, cursor) {
        fail_rec = (struct rcu_failure_record *) cursor;
        rcu_free(fail_rec->info);
        RCU_SAVE_CURSOR(cursor)
        rcu_remove_list(cursor);
        rcu_free(fail_rec);
        RCU_RESTORE_CURSOR(cursor)
    }
    return RCU_E_OK;
}

int rcu_del_all_fail_rec(struct rcu_test_engine *engine) {
    struct rcu_list *cursor1 = NULL;
    struct rcu_list *cursor2 = NULL;
    struct rcu_test *func = NULL;
    struct rcu_module *module = NULL;

    RCU_LOG_DEBUG("Destroying all failure records from test runs");
    if (!engine) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVENG));
        return RCU_E_NG;
    }

    RCU_FOR_EACH_ENTRY(&engine->def_reg.mod_list, cursor1) {
        module = (struct rcu_module *) cursor1;

        RCU_FOR_EACH_ENTRY(&module->func_list, cursor2) {
            func = (struct rcu_test *) cursor2;
            rcu_del_all_fail_rec_from_func(func);
        }
        rcu_del_all_fail_rec_from_module(module);
    }
    RCU_LOG_DEBUG("Destroying all failure records from non-test runs");
    rcu_del_all_fail_rec_impl(&engine->ae.assert_list);
    return RCU_E_OK;
}
