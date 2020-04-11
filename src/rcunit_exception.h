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

#ifndef RCUNIT_EXCEPTION_H
#define RCUNIT_EXCEPTION_H

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include "rcunit_types.h"

#define RCU_DEBUG_EXCEPTION 0

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
typedef enum rcu_exception_id_tag {
    RCU_EXCP_ID_START = 0 /* reserved exception */
    , RCU_EXCP_ABORTTESTRUN /* rest function abort */
    , RCU_EXCP_ABORTMODRUN /* test module abort */
    , RCU_EXCP_NULLPOINTER /* null pointer */
    , RCU_EXCP_INVPARAM /* invalid parameter */
    , RCU_EXCP_SIGSEGV /* segmentation violation */
    , RCU_EXCP_SIGILL /* illegal instruction */
    , RCU_EXCP_SIGFPE /* floating pointer exception */
    , RCU_EXCP_SIGBUS /* bus error */
    , RCU_EXCP_ASSERTIONFAILURE /** Assertion failure */
    /** Start of user-defined exceptions here */
    /** End of user-defined exceptions here */
    , RCU_EXCP_ID_END /* Reserved exception */
    , RCU_EXCP_ID_UNKNOWN
    /* Reserved exception */
} rcu_exception_id;

/* Exception runtime data structure */
typedef struct rcu_exception {
    rcu_exception_id id;
    const char *name;
} rcu_exception;

/* Exception frame runtime data structure */
typedef struct rcu_exception_frame {
    rcu_exception *excp; /* exception */
    sigjmp_buf *env; /* native exception context */
    struct rcu_exception_frame *outer; /* outer exception frame */
    struct sigaction segv_act_old; /* SIGSEGV action to be restored */
    struct sigaction ill_act_old; /* SIGILL action to be restored */
    struct sigaction fpe_act_old; /* SIGFPE action to be restored */
    struct sigaction bus_act_old; /* SIGBUGS action to be restored */
} rcu_exception_frame;

typedef struct rcu_exception_context {
    rcu_exception_frame *excp_root;
} rcu_exception_context;

#define RCU_EXCEPTION_TYPE_GET() (__ret_code)

#define RCU_EXCEPTION_TYPE_NONE  (0)
#define RCU_EXCEPTION_TYPE_THROW (1)
#define RCU_EXCEPTION_TYPE_CORE  (10)


#if RCU_DEBUG_EXCEPTION
#define TRACE_FRAME_HERE \
    fprintf(stdout, "[%s:%d]g_curr_excp_frame = %p\n", __FILE__, __LINE__, g_curr_excp_frame); \
    fprintf(stdout, "[%s:%d]g_curr_excp_frame->env = %p\n", __FILE__, __LINE__,g_curr_excp_frame->env);
#else
#define TRACE_FRAME_HERE
#endif

/* Saves an exception context */
#define RCU_TRY                               \
    {                                         \
        int __retcode;                        \
        sigjmp_buf __env;                     \
        rcu_exception_frame __frame;          \
        __frame.outer = g_curr_excp_frame;    \
        __frame.excp = NULL;              \
    g_curr_excp_frame = &__frame;         \
        g_curr_excp_frame->env = &__env;      \
        TRACE_FRAME_HERE \
        rcu_sig_save(&__frame);             \
        __retcode = sigsetjmp(__env, 1);       \
        if (__retcode == 0) {                 \
            rcu_sig_catch();                \

/* Catches an exception and store in the given variable */
#define RCU_CATCH(__caught_excp)             \
        }                                     \
        g_curr_excp_frame = __frame.outer;    \
        rcu_sig_restore(&__frame); \
        if (__retcode != RCU_EXCEPTION_TYPE_NONE ) {            \
            rcu_exception *__caught_excp = __frame.excp;       \
            int __ret_code = __retcode;                     \

/* Terminates RCU_CATCH */
#define RCU_END_CATCH                                          \
       }                                                       \
    }

/* Throws the given exception object */
#define RCU_THROW(__thrown_excp__)                \
    {                                           \
        g_curr_excp_frame->excp = __thrown_excp__;\
        siglongjmp(*(g_curr_excp_frame->env), RCU_EXCEPTION_TYPE_THROW);\
    }

/* Retrieves the exception object of the given exception id */
#define RCU_GET_EXCP(id)  \
    (rcu_lookup_excp_by_id((id)))

/* Exception mechanism function prototypes */
void rcu_init_excp();

void rcu_destroy_excp();

rcu_exception *rcu_lookup_excp_by_id(rcu_exception_id id);

/** External variable declarations */
extern rcu_exception_frame *g_curr_excp_frame;

/** Exception signal handlers */
extern void rcu_sig_handler(int signo);

extern void rcu_sig_catch(void);

extern void rcu_sig_restore(rcu_exception_frame *pf);

extern void rcu_sig_save(rcu_exception_frame *pf);

#endif /* RCUNIT_EXCEPTION_H */
