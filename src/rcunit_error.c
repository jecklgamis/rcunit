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

#include <rcunit.h>

/** @brief Global error code */
RCU_INT g_ercd=RCU_E_NG;

/** @brief Global error string table */
const RCU_CHAR *g_error_msg_tbl[]={
/* RCU_E_OK            */ "Successful"
/* RCU_E_NG            */,"Failed"
/* RCU_E_NOMEM         */,"Memory allocation failed"
/* RCU_E_INVPARAM      */,"Invalid parameter"
/* RCU_E_MACHINIT      */,"Machine initialization failed"
/* RCU_E_MACHINITDONE  */,"Machine already initialized "
/* RCU_E_MACHNOINIT    */,"Machine not initialized"
/* RCU_E_INVMACH       */,"Invalid machine"
/* RCU_E_ENTITYDISABLED*/,"Test entity is disabled"
/* RCU_E_EXEC          */,"Execution failed"
/* RCU_E_REGNOINIT     */,"Registry not initialized"
/* RCU_E_INVREGNAME    */,"Invalid registry name"
/* RCU_E_INVREG        */,"Invalid registry"
/* RCU_E_INVREGTABLE   */,"Invalid registry table"
/* RCU_E_UNKNOWNREG    */,"Unknown registry"
/* RCU_E_REGEXISTS     */,"Registry exists"
/* RCU_E_MODNOINIT     */,"Module not initialized"
/* RCU_E_INVMOD        */,"Invalid module"
/* RCU_E_INVMODNAME    */,"Invalid module name"
/* RCU_E_INVMODTABLE   */,"Invalid module table"
/* RCU_E_UNKNOWNMOD    */,"Unknown module"
/* RCU_E_MODEXISTS     */,"Module exists"
/* RCU_E_FUNCNOINIT    */,"Function not initialized"
/* RCU_E_INVFUNCNAME   */,"Invalid function name"
/* RCU_E_INVFUNC       */,"Invalid function"
/* RCU_E_INVFUNCTABLE  */,"Invalid function table"
/* RCU_E_FUNCEXISTS    */,"Function exists"
/* RCU_E_INVFUNCENTRY  */,"Invalid function entry point"
};


/**
 *  @brief Returns the error code of the last operation that failed
 *  @return Error code
 */

RCU_INT rcu_get_err(){
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_get_err");
#endif
    return(g_ercd);
}

/**
 *  @brief Returns the error message corresponding to an error code
 *  @param[in] ercd Error code
 *  @return Error message
 */

const RCU_CHAR* rcu_get_err_msg(RCU_INT ercd){
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_get_err_msg");
#endif
    if (ercd >= RCU_E_OK && ercd <= RCU_E_INVFUNCENTRY){
        return(g_error_msg_tbl[ercd]);
    }
    return("Unknown error code");
}
