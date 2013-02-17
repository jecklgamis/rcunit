
/*
 * The MIT License (MIT)
 *
 * RCUNIT - A unit testing framework for C
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

extern rcu_test *rcu_srch_test_func_entry(rcu_module *mod,
        rcu_generic_function entry);
extern rcu_test *rcu_srch_test_func_by_name(rcu_module *mod,
        const char *name);

int rcu_get_timestamp(char *ts_buff, int ts_buff_len) {
    time_t t;
    int len;

    if (ts_buff == NULL || ts_buff_len < RCU_TSTAMP_BUFF_SIZE) {
        RCU_LOG_WARN("Null timestamp buffer or insufficient buffer length");
        return RCU_E_NG;
    }

    memset(ts_buff, 0x00, ts_buff_len);
    time(&t);
    sprintf(ts_buff, "%s", ctime(&t));

    len = strlen(ts_buff);
    memset(&ts_buff[len - 1], 0x00, 1);

    return RCU_E_OK;
}

void rcu_dump_test_dbase_impl(rcu_test_machine *machine) {
    if (machine != NULL) {
        if (!rcu_is_mach_initialized(machine)) {
            RCU_SET_ERCD(RCU_E_MACHNOINIT);
            RCU_LOG_WARN("%s", RCU_GET_ERR_MSG());
        }
        RCU_LOG_INFO("+- Test Entity Database");

        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&machine->reg_list, cursor1) {
            rcu_registry *reg = NULL;
            reg = (rcu_registry*) cursor1;
            rcu_dump_test_reg(reg);
        }
    }
}

RCU_API void rcu_dump_test_dbase() {
    rcu_init();
    rcu_dump_test_dbase_impl(&the_test_machine);
}

rcu_module *rcu_srch_mod_by_name_global(const char *mod_name,
        rcu_registry **which_reg) {
    rcu_test_machine *machine = &the_test_machine;

    *which_reg = NULL;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&machine->reg_list, reg_cursor) {
        rcu_registry *reg = (rcu_registry*) reg_cursor;

        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
            rcu_module *mod = (rcu_module*) mod_cursor;
            if (!strcmp(mod->name, mod_name)) {
                *which_reg = reg;
                return mod;
            }
        }
    }
    return NULL;
}

rcu_test *rcu_srch_test_func_entry_global(rcu_generic_function entry,
        rcu_module **which_mod, rcu_registry **which_reg) {
    rcu_test_machine *machine = &the_test_machine;

    *which_reg = NULL;
    *which_mod = NULL;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&machine->reg_list, reg_cursor) {
        rcu_registry *reg = (rcu_registry*) reg_cursor;

        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
            rcu_module *mod = (rcu_module*) mod_cursor;
            rcu_test *func = rcu_srch_test_func_entry(mod, entry);
            if (func != NULL) {
                *which_reg = reg;
                *which_mod = mod;
                return func;
            }
        }
    }
    return NULL;
}

rcu_test *rcu_srch_test_func_by_name_global(const char *name,
        rcu_module **which_mod, rcu_registry **which_reg) {
    rcu_test_machine *machine = &the_test_machine;

    *which_reg = NULL;
    *which_mod = NULL;

    if (name == NULL) {
        RCU_LOG_WARN("%s (null)", RCU_GET_ERR_MSG_OF(RCU_E_INVFUNCNAME));
        return NULL;
    }

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&machine->reg_list, cursor1) {
        rcu_registry *reg = (rcu_registry*) cursor1;

        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, cursor2) {
            rcu_module *mod = (rcu_module*) cursor2;
            rcu_test *func = rcu_srch_test_func_by_name(mod, name);
            if (func != NULL) {
                *which_reg = reg;
                *which_mod = mod;
                return func;
            }
        }
    }
    return NULL;
}

int rcu_srch_mod_by_ref_global(rcu_module *srch_mod,
        rcu_registry **which_reg) {
    rcu_test_machine *machine = &the_test_machine;
    ;

    *which_reg = NULL;

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&machine->reg_list, reg_cursor) {
        rcu_registry *reg = (rcu_registry*) reg_cursor;

        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
            rcu_module *mod = (rcu_module*) mod_cursor;
            if (mod == srch_mod) {
                return RCU_TRUE;
            }
        }
    }
    return RCU_FALSE;
}

int rcu_get_nr_tests() {
    int nr_tests = 0;
    rcu_test_machine *machine = &the_test_machine;

    rcu_init();

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&machine->reg_list, reg_cursor) {
        rcu_registry *reg = (rcu_registry*) reg_cursor;

        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
            rcu_module *mod = (rcu_module *) mod_cursor;

            RCU_FOR_EACH_ENTRY_WITH_CURSOR(&mod->func_list, test_cursor) {
                nr_tests++;
            }
        }
    }
    return nr_tests;
}

int rcu_get_nr_mods() {
    int nr_mods = 0;
    rcu_test_machine *machine = &the_test_machine;

    rcu_init();

    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&machine->reg_list, reg_cursor) {
        rcu_registry *reg = (rcu_registry*) reg_cursor;

        RCU_FOR_EACH_ENTRY_WITH_CURSOR(&reg->mod_list, mod_cursor) {
            nr_mods++;
        }
    }
    return nr_mods;
}

int rcu_get_nr_regs() {
    int nr_regs = 0;
    rcu_test_machine *machine = &the_test_machine;

    rcu_init();
    RCU_FOR_EACH_ENTRY_WITH_CURSOR(&machine->reg_list, reg_cursor) {
        rcu_registry *reg = (rcu_registry*) reg_cursor;
            nr_regs++;
    }
    return nr_regs;
}

