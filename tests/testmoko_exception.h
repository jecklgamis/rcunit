/*
 * Copyright 2013 Jerrico Gamis <jecklgamis@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TESTMOKO_EXCEPTION_H
#define TESTMOKO_EXCEPTION_H

#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

/** @brief Exception identifier */
typedef enum tmk_exception_id {
    TMK_EXCP_ID_START = 0 /**< Reserved exception */
    , TMK_EXCP_ABORTTESTRUN /**< Test function abort */
    , TMK_EXCP_ABORTMODRUN /**< Test module abort */
    , TMK_EXCP_NULLPOINTER /**< Null pointer */
    , TMK_EXCP_INVPARAM /**< Invalid parameter */
    , TMK_EXCP_SIGSEGV /**< Segmentation violation */
    , TMK_EXCP_SIGILL /**< Illegal instruction */
    , TMK_EXCP_SIGFPE /**< Floating pointer exception */
    , TMK_EXCP_SIGBUS /**< Bus error */
    , TMK_EXCP_ASSERTIONFAILURE /** <Assertion failure */
    /** Start of user-defined exceptions here */
    /** End of user-defined exceptions here */
    , TMK_EXCP_ID_END /**< Reserved exception */
    , TMK_EXCP_ID_UNKNOWN
    /**< Reserved exception */
} tmk_exception_id;

/** @brief Exception runtime data structure */
typedef struct tmk_exception {
    tmk_exception_id id; /**< Exception id */
    const char *name; /**< Exception name */
} tmk_exception;

/** @brief Exception frame runtime data structure */
typedef struct tmk_exception_frame {
    tmk_exception *excp; /**< Exception */
    sigjmp_buf *env; /**< native exception context */
    struct tmk_exception_frame *outer; /**< outer exception frame */
    struct sigaction segv_act_old; /**< SIGSEGV action to be restored */
    struct sigaction ill_act_old; /**< SIGILL action to be restored */
    struct sigaction fpe_act_old; /**< SIGFPE action to be restored */
    struct sigaction bus_act_old; /**< SIGBUGS action to be restored */
} tmk_exception_frame;

typedef struct tmk_exception_context {
    tmk_exception_frame *excp_root;
} tmk_exception_context;

#define TMK_EXCEPTION_TYPE_GET() (__ret_code)
#define TMK_EXCEPTION_TYPE_NONE  (0)
#define TMK_EXCEPTION_TYPE_THROW (1)
#define TMK_EXCEPTION_TYPE_CORE  (10)

/** @brief Saves an exception context */
#define TMK_TRY                               \
    {                                         \
        int __retcode = -1;                    \
        sigjmp_buf __env;                     \
        tmk_exception_frame  __frame;          \
        __frame.outer = g_tmk_curr_excp_frame;    \
        __frame.excp = NULL;              \
		g_tmk_curr_excp_frame = &__frame;         \
        g_tmk_curr_excp_frame->env = &__env;      \
        tmk_sig_save(&__frame);             \
        __retcode = sigsetjmp(__env, 1);       \
        if (__retcode == 0) {                 \
            tmk_sig_catch();                \

/** @brief Catches an exception and store in the given variable */
#define TMK_CATCH(__caught_excp)             \
        }                                     \
        g_tmk_curr_excp_frame = __frame.outer;    \
        tmk_sig_restore(&__frame); \
        if (__retcode != TMK_EXCEPTION_TYPE_NONE ) {            \
            tmk_exception *__caught_excp = __frame.excp;       \
            int __ret_code = __retcode;                     \

/** @brief Terminates TMK_CATCH */
#define TMK_END_CATCH                                          \
       }                                                       \
    }

/** @brief Throws the given exception object */
#define TMK_THROW(__thrown_excp__)                \
    {                                           \
        g_tmk_curr_excp_frame->excp = __thrown_excp__;\
        siglongjmp(*(g_tmk_curr_excp_frame->env), TMK_EXCEPTION_TYPE_THROW);\
    }

/** @brief Retrieves the exception object of the given exception id*/
#define TMK_GET_EXCP(id)  \
    (tmk_lookup_excp_by_id((id)))

/** @brief Exception mechanism function prototypes */
void tmk_init_exception();
void tmk_destroy_exception();
tmk_exception *tmk_lookup_excp_by_id(tmk_exception_id id);

/** External variable declarations */
extern tmk_exception_frame *g_tmk_curr_excp_frame;

/** Exception signal handlers */
extern void tmk_sig_handler(int signo);
extern void tmk_sig_catch(void);
extern void tmk_sig_restore(tmk_exception_frame *pf);
extern void tmk_sig_save(tmk_exception_frame *pf);

#endif

