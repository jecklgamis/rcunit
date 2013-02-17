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
const char* rcu_get_err_msg(int ercd);

#endif /* RCUNIT_ERROR_H */

