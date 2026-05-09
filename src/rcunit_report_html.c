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

static void rcu_html_write_escaped(FILE *fp, const char *value) {
    const unsigned char *cursor = (const unsigned char *) (value ? value : "");

    while (*cursor) {
        switch (*cursor) {
            case '&':
                fprintf(fp, "&amp;");
                break;
            case '<':
                fprintf(fp, "&lt;");
                break;
            case '>':
                fprintf(fp, "&gt;");
                break;
            case '"':
                fprintf(fp, "&quot;");
                break;
            case '\'':
                fprintf(fp, "&#39;");
                break;
            default:
                fprintf(fp, "%c", *cursor);
                break;
        }
        cursor++;
    }
}

static int rcu_html_count_tests_in_module(struct rcu_module *module, int run_stat) {
    int nr_tests = 0;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&module->func_list, func_cursor) {
        struct rcu_test *func = (struct rcu_test *) func_cursor;
        if (func->run_stat == run_stat) {
            nr_tests++;
        }
    }
    return nr_tests;
}

static int rcu_html_count_tests_in_registry(struct rcu_registry *reg, int run_stat) {
    int nr_tests = 0;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
        struct rcu_module *module = (struct rcu_module *) mod_cursor;
        nr_tests += rcu_html_count_tests_in_module(module, run_stat);
    }
    return nr_tests;
}

static int rcu_html_count_modules_in_registry(struct rcu_registry *reg, int run_stat) {
    int nr_modules = 0;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
        struct rcu_module *module = (struct rcu_module *) mod_cursor;
        if (module->run_stat == run_stat) {
            nr_modules++;
        }
    }
    return nr_modules;
}

static void rcu_html_write_failures(FILE *fp, struct rcu_list *fail_rec_list) {
    int nr_failures = 0;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(fail_rec_list, fail_rec_cursor) {
        struct rcu_failure_record *fail_rec = (struct rcu_failure_record *) fail_rec_cursor;
        if (nr_failures == 0) {
            fprintf(fp, "<ul class=\"failures\">");
        }
        fprintf(fp, "<li>");
        rcu_html_write_escaped(fp, fail_rec->info);
        fprintf(fp, "</li>");
        nr_failures++;
    }
    if (nr_failures > 0) {
        fprintf(fp, "</ul>");
    } else {
        fprintf(fp, "<span class=\"muted\">None</span>");
    }
}

static const char *rcu_html_status_class(int run_stat) {
    switch (run_stat) {
        case RCU_RUN_STAT_TEST_SUCC:
            return "passed";
        case RCU_RUN_STAT_TEST_FAILED:
            return "failed";
        default:
            return "ignored";
    }
}

int rcu_gen_html_report(struct rcu_test_engine *engine) {
    char ts_buff[RCU_TSTAMP_BUFF_SIZE];
    FILE *fp = NULL;
    struct rcu_registry *reg = &engine->def_reg;
    int nr_passed_tests = rcu_html_count_tests_in_registry(reg, RCU_RUN_STAT_TEST_SUCC);
    int nr_failed_tests = rcu_html_count_tests_in_registry(reg, RCU_RUN_STAT_TEST_FAILED);
    int nr_passed_modules = rcu_html_count_modules_in_registry(reg, RCU_RUN_STAT_TEST_SUCC);
    int nr_failed_modules = rcu_html_count_modules_in_registry(reg, RCU_RUN_STAT_TEST_FAILED);

    rcu_get_timestamp(ts_buff, RCU_TSTAMP_BUFF_SIZE);

    RCU_LOG_DEBUG("Generating test run report : %s", RCU_TEST_RUN_REPORT_FILENAME_HTML);
    if ((!(fp = fopen(RCU_TEST_RUN_REPORT_FILENAME_HTML, "wt")))) {
        return RCU_E_NG;
    }

    fprintf(fp, "<!doctype html>\n");
    fprintf(fp, "<html lang=\"en\">\n");
    fprintf(fp, "<head>\n");
    fprintf(fp, "<meta charset=\"utf-8\">\n");
    fprintf(fp, "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n");
    fprintf(fp, "<title>rcunit Test Run Report</title>\n");
    fprintf(fp, "<style>\n");
    fprintf(fp, ":root{color-scheme:light;--bg:#f7f8fa;--panel:#fff;--line:#d8dee8;--text:#1d2430;--muted:#667085;--pass:#0f7b45;--fail:#b42318;--ignore:#6941c6;}\n");
    fprintf(fp, "body{margin:0;background:var(--bg);color:var(--text);font-family:-apple-system,BlinkMacSystemFont,\"Segoe UI\",sans-serif;font-size:14px;line-height:1.5;}\n");
    fprintf(fp, "main{max-width:1120px;margin:0 auto;padding:32px 20px 48px;}\n");
    fprintf(fp, "header{display:flex;align-items:flex-end;justify-content:space-between;gap:20px;border-bottom:1px solid var(--line);padding-bottom:20px;margin-bottom:24px;}\n");
    fprintf(fp, "h1{font-size:28px;line-height:1.2;margin:0 0 6px;}h2{font-size:18px;margin:28px 0 12px;}h3{font-size:16px;margin:22px 0 10px;}\n");
    fprintf(fp, ".meta{color:var(--muted);}.summary{display:grid;grid-template-columns:repeat(auto-fit,minmax(160px,1fr));gap:12px;margin-bottom:28px;}\n");
    fprintf(fp, ".metric{background:var(--panel);border:1px solid var(--line);border-radius:6px;padding:14px 16px;}.metric strong{display:block;font-size:24px;line-height:1.2;}.metric span{color:var(--muted);}\n");
    fprintf(fp, "table{width:100%%;border-collapse:collapse;background:var(--panel);border:1px solid var(--line);border-radius:6px;overflow:hidden;}th,td{border-bottom:1px solid var(--line);padding:10px 12px;text-align:left;vertical-align:top;}th{background:#eef2f6;font-weight:600;}tr:last-child td{border-bottom:0;}\n");
    fprintf(fp, ".status{display:inline-block;border-radius:999px;padding:2px 8px;font-size:12px;font-weight:600;}.status.passed{background:#dcfae6;color:var(--pass);}.status.failed{background:#fee4e2;color:var(--fail);}.status.ignored{background:#ebe9fe;color:var(--ignore);}\n");
    fprintf(fp, ".module{margin-top:22px;}.failures{margin:0;padding-left:18px;color:var(--fail);}.muted{color:var(--muted);}.nowrap{white-space:nowrap;}\n");
    fprintf(fp, "@media (max-width:700px){header{display:block;}table{font-size:13px;}th,td{padding:8px;}}\n");
    fprintf(fp, "</style>\n");
    fprintf(fp, "</head>\n");
    fprintf(fp, "<body>\n");
    fprintf(fp, "<main>\n");
    fprintf(fp, "<header><div><h1>rcunit Test Run Report</h1><div class=\"meta\">Registry ");
    rcu_html_write_escaped(fp, reg->name);
    fprintf(fp, "</div></div><div class=\"meta\">Generated ");
    rcu_html_write_escaped(fp, ts_buff);
    fprintf(fp, "<br>rcunit ");
    rcu_html_write_escaped(fp, RCU_VERSION_STRING);
    fprintf(fp, "</div></header>\n");

    fprintf(fp, "<section class=\"summary\" aria-label=\"Summary\">\n");
    fprintf(fp, "<div class=\"metric\"><strong>%d</strong><span>Passed tests</span></div>\n", nr_passed_tests);
    fprintf(fp, "<div class=\"metric\"><strong>%d</strong><span>Failed tests</span></div>\n", nr_failed_tests);
    fprintf(fp, "<div class=\"metric\"><strong>%d</strong><span>Passed modules</span></div>\n", nr_passed_modules);
    fprintf(fp, "<div class=\"metric\"><strong>%d</strong><span>Failed modules</span></div>\n", nr_failed_modules);
    fprintf(fp, "</section>\n");

    fprintf(fp, "<h2>Modules</h2>\n");
    fprintf(fp, "<table><thead><tr><th>Module</th><th>Result</th><th>Tests</th><th>Passed</th><th>Failed</th><th>Failures</th></tr></thead><tbody>\n");
    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
        struct rcu_module *module = (struct rcu_module *) mod_cursor;
        int nr_module_passed_tests = rcu_html_count_tests_in_module(module, RCU_RUN_STAT_TEST_SUCC);
        int nr_module_failed_tests = rcu_html_count_tests_in_module(module, RCU_RUN_STAT_TEST_FAILED);
        fprintf(fp, "<tr><td>");
        rcu_html_write_escaped(fp, module->name);
        fprintf(fp, "</td><td><span class=\"status %s\">", rcu_html_status_class(module->run_stat));
        rcu_html_write_escaped(fp, rcu_get_stat_str(module->run_stat));
        fprintf(fp, "</span></td><td>%d</td><td>%d</td><td>%d</td><td>", module->nr_test, nr_module_passed_tests, nr_module_failed_tests);
        rcu_html_write_failures(fp, &module->fail_rec_list);
        fprintf(fp, "</td></tr>\n");
    }
    fprintf(fp, "</tbody></table>\n");

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, module_cursor) {
        struct rcu_module *module = (struct rcu_module *) module_cursor;
        fprintf(fp, "<section class=\"module\">\n");
        fprintf(fp, "<h3>");
        rcu_html_write_escaped(fp, module->name);
        fprintf(fp, "</h3>\n");
        fprintf(fp, "<table><thead><tr><th>Test</th><th>Result</th><th>Assertions</th><th>Fixtures</th><th>Failures</th></tr></thead><tbody>\n");
        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&module->func_list, func_cursor) {
            struct rcu_test *func = (struct rcu_test *) func_cursor;
            fprintf(fp, "<tr><td>");
            rcu_html_write_escaped(fp, func->name);
            fprintf(fp, "</td><td><span class=\"status %s\">", rcu_html_status_class(func->run_stat));
            rcu_html_write_escaped(fp, rcu_get_stat_str(func->run_stat));
            fprintf(fp, "</span></td><td class=\"nowrap\">%d passed, %d failed</td><td>", func->nr_succ_assert, func->nr_fail_assert);
            if (func->init_failed || func->destroy_failed) {
                fprintf(fp, "%s%s%s",
                        func->init_failed ? "init failed" : "",
                        func->init_failed && func->destroy_failed ? ", " : "",
                        func->destroy_failed ? "destroy failed" : "");
            } else {
                fprintf(fp, "<span class=\"muted\">OK</span>");
            }
            fprintf(fp, "</td><td>");
            rcu_html_write_failures(fp, &func->fail_rec_list);
            fprintf(fp, "</td></tr>\n");
        }
        if (module->nr_test == 0) {
            fprintf(fp, "<tr><td colspan=\"5\" class=\"muted\">No tests</td></tr>\n");
        }
        fprintf(fp, "</tbody></table>\n");
        fprintf(fp, "</section>\n");
    }

    fprintf(fp, "</main>\n");
    fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");

    if (fp) {
        fclose(fp);
    }
    RCU_LOG_INFO("Test run report generated : %s", RCU_TEST_RUN_REPORT_FILENAME_HTML);
    return RCU_E_OK;
}
