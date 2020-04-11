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

int g_ercd = RCU_E_NG;

const char *g_error_msg_tbl[] = {
        /* RCU_E_OK            */"Successful"
        /* RCU_E_NG            */, "Failed"
        /* RCU_E_NOMEM         */, "Memory allocation failed"
        /* RCU_E_INVPARAM      */, "Invalid parameter"
        /* RCU_E_MACHINIT      */, "Machine initialization failed"
        /* RCU_E_MACHINITDONE  */, "Machine already initialized "
        /* RCU_E_MACHNOINIT    */, "Machine not initialized"
        /* RCU_E_INVMACH       */, "Invalid machine"
        /* RCU_E_ENTITYDISABLED */, "Test entity is disabled"
        /* RCU_E_EXEC          */, "Execution failed"
        /* RCU_E_REGNOINIT     */, "Registry not initialized"
        /* RCU_E_INVREGNAME    */, "Invalid registry name"
        /* RCU_E_INVREG        */, "Invalid registry"
        /* RCU_E_INVREGTABLE   */, "Invalid registry table"
        /* RCU_E_UNKNOWNREG    */, "Unknown registry"
        /* RCU_E_REGEXISTS     */, "Registry exists"
        /* RCU_E_MODNOINIT     */, "Module not initialized"
        /* RCU_E_INVMOD        */, "Invalid module"
        /* RCU_E_INVMODNAME    */, "Invalid module name"
        /* RCU_E_INVMODTABLE   */, "Invalid module table"
        /* RCU_E_UNKNOWNMOD    */, "Unknown module"
        /* RCU_E_MODEXISTS     */, "Module exists"
        /* RCU_E_FUNCNOINIT    */, "Function not initialized"
        /* RCU_E_INVFUNCNAME   */, "Invalid function name"
        /* RCU_E_INVFUNC       */, "Invalid function"
        /* RCU_E_INVFUNCTABLE  */, "Invalid function table"
        /* RCU_E_FUNCEXISTS    */, "Function exists"
        /* RCU_E_INVFUNCENTRY  */, "Invalid function entry point"
};

RCU_API int rcu_get_err() {
    rcu_init();
    return g_ercd;
}

RCU_API const char *rcu_get_err_msg(int ercd) {
    rcu_init();
    if (ercd >= RCU_E_OK && ercd <= RCU_E_INVFUNCENTRY) {
        return (g_error_msg_tbl[ercd]);
    }
    return "Unknown error code";
}
