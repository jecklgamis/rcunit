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

/** @brief Current exception stack frame */
RCU_EXCEPTION_FRAME *g_curr_excp_frame=RCU_NULL;

/** @brief Defines an exception object table entry */
#define RCU_DEF_EXCP(id,name) \
    ,{id,name}

/** @brief Global exception object table */
RCU_EXCEPTION g_rcu_excp_tbl[]={
    RCU_EXCP_ID_START  ,"Reserved (RCU_EXCP_ID_START)"
    RCU_DEF_EXCP(RCU_EXCP_ABORTTESTRUN,"Test run abort")
    RCU_DEF_EXCP(RCU_EXCP_ABORTMODRUN,"Test module abort")
    RCU_DEF_EXCP(RCU_EXCP_NULLPOINTER,"Null pointer")
    RCU_DEF_EXCP(RCU_EXCP_INVPARAM,"Invalid parameter")
    /* Start of user-defined exceptions here */
    /* End of user-defined exceptions here */
    RCU_DEF_EXCP(RCU_EXCP_ID_END,"Reserved (RCU_EXCP_ID_END)")
    RCU_DEF_EXCP(RCU_EXCP_ID_UNKNOWN,"Unknown")
};

/**
 *  @brief Retrieves the exception object of the given exception id
 *  @param[in] id Exception identifier
 *  @return Exception object
 */

 RCU_EXCEPTION *rcu_lookup_excp_by_id(RCU_EXCEPTION_ID id){
    if (id >RCU_EXCP_ID_START && id < RCU_EXCP_ID_END){
        return(&g_rcu_excp_tbl[id]);
    }
    return(&g_rcu_excp_tbl[RCU_EXCP_ID_UNKNOWN]);
}

/**
 *  @brief Initializes the exception mechanism
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_init_excp(){
    RCU_LOG_INFO("Initializing exception mechanism");
    g_curr_excp_frame=RCU_NULL;
    return(RCU_E_OK);
}

/**
 *  @brief Destroys the exception mechanism
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_excp(){
    RCU_LOG_INFO("Destroying exception mechanism");
    return(RCU_E_OK);
}


