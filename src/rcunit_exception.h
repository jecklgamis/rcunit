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

#ifndef RCUNIT_EXCEPTION_H
#define RCUNIT_EXCEPTION_H

/*  The RCUNIT Exception Facility
 *  This is a simple implementation of an exception mechanism. The mechanism
 *  relies on the non-local jump routines ,setjmp and longjmp, and is
 *  primarily used for aborting test function execution (i.e. triggered by
 *  RCU_XXX_FATAL assertion macros. However, simple try-catch statements
 *  can be created using the macro interfaces.
 *
 *  How to add new exception objects:
 *  1. Add a new exception id (RCU_EXCEPTION_ID type)
 *  2. Add a new exception object corresponding to the id in the exception
 *     table (g_excp_tbl).
 */

/** @brief Exception identifier */
typedef enum rcu_exception_id {
     RCU_EXCP_ID_START=0            /**< Reserved exception */
    ,RCU_EXCP_ABORTTESTRUN          /**< Test function abort */
    ,RCU_EXCP_ABORTMODRUN           /**< Test module abort */
    ,RCU_EXCP_NULLPOINTER           /**< Null pointer */
    ,RCU_EXCP_INVPARAM              /**< Invalid parameter */
    /** Start of user-defined exceptions here */
    /** End of user-defined exceptions here */
    ,RCU_EXCP_ID_END                /**< Reserved exception */
    ,RCU_EXCP_ID_UNKNOWN            /**< Reserved exception */
}RCU_EXCEPTION_ID;

/** @brief Exception runtime data structure */
typedef struct rcu_exception {
RCU_EXCEPTION_ID id;                /**< exception id */
const RCU_CHAR *name;               /**< exception name */
}RCU_EXCEPTION;

/** @brief Exception frame runtime data structure */
typedef struct rcu_exception_frame {
RCU_EXCEPTION *excp;                /**< exception */
jmp_buf *env;                       /**< native exception context */
struct  rcu_exception_frame *outer; /**< outer exception frame */
}RCU_EXCEPTION_FRAME;

/** @brief Saves an exception context */
#define RCU_TRY                               \
    {                                         \
    RCU_INT __retcode;                            \
    jmp_buf __env;                            \
    RCU_EXCEPTION_FRAME __frame;              \
        __frame.outer = g_curr_excp_frame;    \
        __frame.excp = RCU_NULL;                  \
        g_curr_excp_frame = &__frame;         \
        g_curr_excp_frame->env = &__env;      \
        __retcode = setjmp(__env);            \
        if (__retcode == 0) {

/** @brief Catches an exception and store in the given variable */
#define RCU_CATCH(__catched_excp)             \
        }                                     \
        g_curr_excp_frame = __frame.outer;    \
        if (__retcode !=0) {                  \
            RCU_EXCEPTION *__catched_excp = __frame.excp; \

/** @brief Terminates RCU_CATCH */
#define RCU_END_CATCH                                           \
        }                                                       \
    }

/** @brief Throws the given exception object */
#define RCU_THROW(__thrown_excp)                \
    {                                           \
        g_curr_excp_frame->excp = __thrown_excp;\
        longjmp(*(g_curr_excp_frame->env),1);   \
    }

/** @brief Retrieve the exception object of the given exception id*/
#define RCU_GET_EXCP(id)  \
    (rcu_lookup_excp_by_id((id)))

/** @brief Exception mechanism function prototypes */
 RCU_INT rcu_init_excp();
 RCU_INT rcu_destroy_excp();
 RCU_EXCEPTION *rcu_lookup_excp_by_id(RCU_EXCEPTION_ID id);

/** External variable declarations */
extern RCU_EXCEPTION_FRAME *g_curr_excp_frame;

#endif /* RCUNIT_EXCEPTION_H */

