/*
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "rcunit.h"

extern const char *rcu_get_stat_str(int stat);

static void rcu_json_indent(FILE *fp, int level) {
    int a = 0;
    for (a = 0; a < level; a++) {
        fprintf(fp, "  ");
    }
}

static void rcu_json_write_string(FILE *fp, const char *value) {
    const unsigned char *cursor = (const unsigned char *) (value ? value : "");
    fprintf(fp, "\"");
    while (*cursor) {
        switch (*cursor) {
            case '"':
                fprintf(fp, "\\\"");
                break;
            case '\\':
                fprintf(fp, "\\\\");
                break;
            case '\b':
                fprintf(fp, "\\b");
                break;
            case '\f':
                fprintf(fp, "\\f");
                break;
            case '\n':
                fprintf(fp, "\\n");
                break;
            case '\r':
                fprintf(fp, "\\r");
                break;
            case '\t':
                fprintf(fp, "\\t");
                break;
            default:
                if (*cursor < 0x20) {
                    fprintf(fp, "\\u%04x", *cursor);
                } else {
                    fprintf(fp, "%c", *cursor);
                }
                break;
        }
        cursor++;
    }
    fprintf(fp, "\"");
}

static void rcu_json_write_failure_records(FILE *fp, struct rcu_list *fail_rec_list, int indent_level) {
    int fail_rec_no = 0;

    fprintf(fp, "[");
    RCU_FOR_EACH_ENTRY_WITH_CURSOR(fail_rec_list, fail_rec_cursor) {
        struct rcu_failure_record *fail_rec = (struct rcu_failure_record *) fail_rec_cursor;
        if (fail_rec_no > 0) {
            fprintf(fp, ",");
        }
        fprintf(fp, "\n");
        rcu_json_indent(fp, indent_level + 1);
        rcu_json_write_string(fp, fail_rec->info);
        fail_rec_no++;
    }
    if (fail_rec_no > 0) {
        fprintf(fp, "\n");
        rcu_json_indent(fp, indent_level);
    }
    fprintf(fp, "]");
}

static int rcu_json_count_tests_in_module(struct rcu_module *module, int run_stat) {
    int nr_tests = 0;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&module->func_list, func_cursor) {
        struct rcu_test *func = (struct rcu_test *) func_cursor;
        if (func->run_stat == run_stat) {
            nr_tests++;
        }
    }
    return nr_tests;
}

static int rcu_json_count_tests_in_registry(struct rcu_registry *reg, int run_stat) {
    int nr_tests = 0;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
        struct rcu_module *module = (struct rcu_module *) mod_cursor;
        nr_tests += rcu_json_count_tests_in_module(module, run_stat);
    }
    return nr_tests;
}

static int rcu_json_count_modules_in_registry(struct rcu_registry *reg, int run_stat) {
    int nr_modules = 0;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
        struct rcu_module *module = (struct rcu_module *) mod_cursor;
        if (module->run_stat == run_stat) {
            nr_modules++;
        }
    }
    return nr_modules;
}

int rcu_gen_json_report(struct rcu_test_engine *engine) {
    char ts_buff[RCU_TSTAMP_BUFF_SIZE];
    FILE *fp = NULL;
    struct rcu_registry *reg = &engine->def_reg;
    int nr_passed_tests = rcu_json_count_tests_in_registry(reg, RCU_RUN_STAT_TEST_SUCC);
    int nr_failed_tests = rcu_json_count_tests_in_registry(reg, RCU_RUN_STAT_TEST_FAILED);
    int nr_passed_modules = rcu_json_count_modules_in_registry(reg, RCU_RUN_STAT_TEST_SUCC);
    int nr_failed_modules = rcu_json_count_modules_in_registry(reg, RCU_RUN_STAT_TEST_FAILED);

    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);

    RCU_LOG_DEBUG("Generating test run report : %s", RCU_TEST_RUN_REPORT_FILENAME_JSON);
    if ((!(fp = fopen(RCU_TEST_RUN_REPORT_FILENAME_JSON, "wt")))) {
        return RCU_E_NG;
    }

    fprintf(fp, "{\n");

    rcu_json_indent(fp, 1);
    fprintf(fp, "\"generated_at\": ");
    rcu_json_write_string(fp, ts_buff);
    fprintf(fp, ",\n");

    rcu_json_indent(fp, 1);
    fprintf(fp, "\"rcunit_version\": ");
    rcu_json_write_string(fp, RCU_VERSION_STRING);
    fprintf(fp, ",\n");

    rcu_json_indent(fp, 1);
    fprintf(fp, "\"engine\": {\n");
    rcu_json_indent(fp, 2);
    fprintf(fp, "\"name\": ");
    rcu_json_write_string(fp, engine->name);
    fprintf(fp, ",\n");
    rcu_json_indent(fp, 2);
    fprintf(fp, "\"summary\": {\n");
    rcu_json_indent(fp, 3);
    fprintf(fp, "\"passed_tests\": %d,\n", nr_passed_tests);
    rcu_json_indent(fp, 3);
    fprintf(fp, "\"failed_tests\": %d,\n", nr_failed_tests);
    rcu_json_indent(fp, 3);
    fprintf(fp, "\"passed_registries\": %d,\n", nr_failed_modules == 0 ? 1 : 0);
    rcu_json_indent(fp, 3);
    fprintf(fp, "\"failed_registries\": %d\n", nr_failed_modules > 0 ? 1 : 0);
    rcu_json_indent(fp, 2);
    fprintf(fp, "}\n");
    rcu_json_indent(fp, 1);
    fprintf(fp, "},\n");

    rcu_json_indent(fp, 1);
    fprintf(fp, "\"registry\": {\n");
    rcu_json_indent(fp, 2);
    fprintf(fp, "\"name\": ");
    rcu_json_write_string(fp, reg->name);
    fprintf(fp, ",\n");
    rcu_json_indent(fp, 2);
    fprintf(fp, "\"module_count\": %d,\n", reg->nr_module);
    rcu_json_indent(fp, 2);
    fprintf(fp, "\"summary\": {\n");
    rcu_json_indent(fp, 3);
    fprintf(fp, "\"passed_modules\": %d,\n", nr_passed_modules);
    rcu_json_indent(fp, 3);
    fprintf(fp, "\"failed_modules\": %d,\n", nr_failed_modules);
    rcu_json_indent(fp, 3);
    fprintf(fp, "\"passed_tests\": %d,\n", nr_passed_tests);
    rcu_json_indent(fp, 3);
    fprintf(fp, "\"failed_tests\": %d\n", nr_failed_tests);
    rcu_json_indent(fp, 2);
    fprintf(fp, "},\n");

    rcu_json_indent(fp, 2);
    fprintf(fp, "\"modules\": [");
    RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&reg->mod_list, mod_cursor, mod_no) {
        struct rcu_module *module = (struct rcu_module *) mod_cursor;
        int nr_module_passed_tests = rcu_json_count_tests_in_module(module, RCU_RUN_STAT_TEST_SUCC);
        int nr_module_failed_tests = rcu_json_count_tests_in_module(module, RCU_RUN_STAT_TEST_FAILED);
        if (mod_no > 0) {
            fprintf(fp, ",");
        }
        fprintf(fp, "\n");
        rcu_json_indent(fp, 3);
        fprintf(fp, "{\n");
        rcu_json_indent(fp, 4);
        fprintf(fp, "\"name\": ");
        rcu_json_write_string(fp, module->name);
        fprintf(fp, ",\n");
        rcu_json_indent(fp, 4);
        fprintf(fp, "\"result\": ");
        rcu_json_write_string(fp, rcu_get_stat_str(module->run_stat));
        fprintf(fp, ",\n");
        rcu_json_indent(fp, 4);
        fprintf(fp, "\"test_count\": %d,\n", module->nr_test);
        rcu_json_indent(fp, 4);
        fprintf(fp, "\"passed_tests\": %d,\n", nr_module_passed_tests);
        rcu_json_indent(fp, 4);
        fprintf(fp, "\"failed_tests\": %d,\n", nr_module_failed_tests);
        rcu_json_indent(fp, 4);
        fprintf(fp, "\"init_failed\": %s,\n", module->init_failed ? "true" : "false");
        rcu_json_indent(fp, 4);
        fprintf(fp, "\"destroy_failed\": %s,\n", module->destroy_failed ? "true" : "false");
        rcu_json_indent(fp, 4);
        fprintf(fp, "\"failures\": ");
        rcu_json_write_failure_records(fp, &module->fail_rec_list, 4);
        fprintf(fp, ",\n");
        rcu_json_indent(fp, 4);
        fprintf(fp, "\"tests\": [");
        RCU_FOR_EACH_ENTRY_WITH_CURSOR_INDEX(&module->func_list, func_cursor, func_no) {
            struct rcu_test *func = (struct rcu_test *) func_cursor;
            if (func_no > 0) {
                fprintf(fp, ",");
            }
            fprintf(fp, "\n");
            rcu_json_indent(fp, 5);
            fprintf(fp, "{\n");
            rcu_json_indent(fp, 6);
            fprintf(fp, "\"name\": ");
            rcu_json_write_string(fp, func->name);
            fprintf(fp, ",\n");
            rcu_json_indent(fp, 6);
            fprintf(fp, "\"result\": ");
            rcu_json_write_string(fp, rcu_get_stat_str(func->run_stat));
            fprintf(fp, ",\n");
            rcu_json_indent(fp, 6);
            fprintf(fp, "\"successful_assertions\": %d,\n", func->nr_succ_assert);
            rcu_json_indent(fp, 6);
            fprintf(fp, "\"failed_assertions\": %d,\n", func->nr_fail_assert);
            rcu_json_indent(fp, 6);
            fprintf(fp, "\"init_failed\": %s,\n", func->init_failed ? "true" : "false");
            rcu_json_indent(fp, 6);
            fprintf(fp, "\"destroy_failed\": %s,\n", func->destroy_failed ? "true" : "false");
            rcu_json_indent(fp, 6);
            fprintf(fp, "\"failures\": ");
            rcu_json_write_failure_records(fp, &func->fail_rec_list, 6);
            fprintf(fp, "\n");
            rcu_json_indent(fp, 5);
            fprintf(fp, "}");
        }
        if (module->nr_test > 0) {
            fprintf(fp, "\n");
            rcu_json_indent(fp, 4);
        }
        fprintf(fp, "]\n");
        rcu_json_indent(fp, 3);
        fprintf(fp, "}");
    }
    if (reg->nr_module > 0) {
        fprintf(fp, "\n");
        rcu_json_indent(fp, 2);
    }
    fprintf(fp, "]\n");
    rcu_json_indent(fp, 1);
    fprintf(fp, "}\n");

    fprintf(fp, "}\n");

    if (fp) {
        fclose(fp);
    }
    RCU_LOG_INFO("Test run report generated : %s", RCU_TEST_RUN_REPORT_FILENAME_JSON);
    return RCU_E_OK;
}
