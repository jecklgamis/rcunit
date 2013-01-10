/*
 * RCUNIT - A unit testing framework for C.
 * Copyright (C) 2006 Jerrico L. Gamis
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA
 *
 */

#ifndef RCUNIT_ERROR_H
#define RCUNIT_ERROR_H

/** @brief The RCUNIT error codes */
enum {
 RCU_E_OK
,RCU_E_NG
,RCU_E_NOMEM
,RCU_E_INVPARAM
,RCU_E_MACHINIT
,RCU_E_MACHINITDONE
,RCU_E_MACHNOINIT
,RCU_E_INVMACH
,RCU_E_ENTITYDISABLED
,RCU_E_EXEC
,RCU_E_REGNOINIT
,RCU_E_INVREGNAME
,RCU_E_INVREG
,RCU_E_INVREGTABLE
,RCU_E_UNKNOWNREG
,RCU_E_REGEXISTS
,RCU_E_MODNOINIT
,RCU_E_INVMOD
,RCU_E_INVMODNAME
,RCU_E_INVMODTABLE
,RCU_E_UNKNOWNMOD
,RCU_E_MODEXISTS
,RCU_E_FUNCNOINIT
,RCU_E_INVFUNCNAME
,RCU_E_INVFUNC
,RCU_E_INVFUNCTABLE
,RCU_E_FUNCEXISTS
,RCU_E_INVFUNCENTRY
};

/** @brief Sets the error code of the last operation that failed */
#if 0
RCU_LOG_INFO_P3("%s set here : %s(%lu)",rcu_get_err_msg(ercd),__FILE__,__LINE__);
#endif

#define RCU_SET_ERCD(ercd) \
    { \
    g_ercd = ercd; \
    }

/** @brief Retrieves the error string of the last error code */
#define RCU_GET_ERR_MSG() \
    (rcu_get_err_msg(g_ercd))


/** @brief Retrieves the error string of the last error code */
#define RCU_GET_ERR_MSG_OF(ercd) \
    (rcu_get_err_msg((ercd)))

/** Error management function prototypes */
RCU_INT rcu_get_err();
const RCU_CHAR* rcu_get_err_msg(RCU_INT ercd);

#endif /* RCUNIT_ERROR_H */



