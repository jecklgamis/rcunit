/*
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
