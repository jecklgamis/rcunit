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

#ifndef RCUNIT_ERROR_H
#define RCUNIT_ERROR_H

/* The RCUNIT error codes */
enum {
    RCU_E_OK = 0,
    RCU_E_NG,
    RCU_E_NOMEM,
    RCU_E_INVPARAM,
    RCU_E_MACHINIT,
    RCU_E_MACHINITDONE,
    RCU_E_MACHNOINIT,
    RCU_E_INVMACH,
    RCU_E_ENTITYDISABLED,
    RCU_E_EXEC,
    RCU_E_REGNOINIT,
    RCU_E_INVREGNAME,
    RCU_E_INVREG,
    RCU_E_INVREGTABLE,
    RCU_E_UNKNOWNREG,
    RCU_E_REGEXISTS,
    RCU_E_MODNOINIT,
    RCU_E_INVMOD,
    RCU_E_INVMODNAME,
    RCU_E_INVMODTABLE,
    RCU_E_UNKNOWNMOD,
    RCU_E_MODEXISTS,
    RCU_E_FUNCNOINIT,
    RCU_E_INVFUNCNAME,
    RCU_E_INVFUNC,
    RCU_E_INVFUNCTABLE,
    RCU_E_FUNCEXISTS,
    RCU_E_INVFUNCENTRY
};

/* Sets the error code of the last operation that failed */
#define RCU_SET_ERCD(ercd) { g_ercd = ercd; }

/* Retrieves the error string of the last error code */
#define RCU_GET_ERR_MSG() \
    (rcu_get_err_msg(g_ercd))

/* Retrieves the error string of the last error code */
#define RCU_GET_ERR_MSG_OF(ercd) \
    (rcu_get_err_msg((ercd)))

/** Error management function prototypes */
int rcu_get_err();

const char *rcu_get_err_msg(int ercd);

#endif /* RCUNIT_ERROR_H */

