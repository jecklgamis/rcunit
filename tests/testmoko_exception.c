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

#include <testmoko_exception.h>

/* Current exception stack frame */
tmk_exception_frame *g_tmk_curr_excp_frame = NULL;
int g_tmk_excp_initialized = 0;

/* Defines an exception object table entry */
#define TMK_DEFINE_EXCEPTION(id, name) \
    { id, name },

/* Global exception object table */
tmk_exception
g_tmk_excp_tbl[] = {
    TMK_DEFINE_EXCEPTION(TMK_EXCP_ID_START, "Reserved (TMK_EXCP_ID_START)")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_ABORTTESTRUN, "test function aborted")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_ABORTMODRUN, "test module aborted")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_NULLPOINTER, "null pointer")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_INVPARAM, "invalid parameter")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_SIGSEGV, "segmentation violation")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_SIGILL, "illegal instruction")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_SIGFPE, "floating point")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_SIGBUS, "bus error")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_ASSERTIONFAILURE, "assertion failure")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_ID_END, "Reserved (TMK_EXCP_ID_END)")
    TMK_DEFINE_EXCEPTION(TMK_EXCP_ID_UNKNOWN, "Unknown")
};

/*  Retrieves the exception object of the given exception id */
tmk_exception *tmk_lookup_excp_by_id(tmk_exception_id id) {
    tmk_exception *excp = &g_tmk_excp_tbl[TMK_EXCP_ID_UNKNOWN];
    if (id > TMK_EXCP_ID_START && id < TMK_EXCP_ID_END) {
        excp = &g_tmk_excp_tbl[id];
    }
    return excp;
}

/*  Initializes the exception mechanism */
void tmk_init_exception() {
    if (!g_tmk_excp_initialized) {
        g_tmk_curr_excp_frame = NULL;
    }
}

/* Destroys the exception mechanism */

void tmk_destroy_exception() {
    if (g_tmk_excp_initialized) {
    }
}

/* Handles SIGSEGV, SIGILL, SIGFPE, and SIGBUS signals when executing tests */
void tmk_sig_handler(int signo) {
    switch (signo) {
        case SIGSEGV:
            g_tmk_curr_excp_frame->excp = tmk_lookup_excp_by_id(TMK_EXCP_SIGSEGV);
            break;
        case SIGILL:
            g_tmk_curr_excp_frame->excp = tmk_lookup_excp_by_id(TMK_EXCP_SIGILL);
            break;
        case SIGFPE:
            g_tmk_curr_excp_frame->excp = tmk_lookup_excp_by_id(TMK_EXCP_SIGFPE);
            break;
        case SIGBUS:
            g_tmk_curr_excp_frame->excp = tmk_lookup_excp_by_id(TMK_EXCP_SIGBUS);
            break;
        default:
            g_tmk_curr_excp_frame->excp = tmk_lookup_excp_by_id(TMK_EXCP_ID_UNKNOWN);
            break;
    }
#if 0
    fprintf(stdout, "Received SIGNO=%d (%s)\n", signo, g_tmk_curr_excp_frame->excp->name);
#endif
    siglongjmp(*(g_tmk_curr_excp_frame->env), TMK_EXCEPTION_TYPE_CORE);
}

/* Saves signal context */
void tmk_sig_save(tmk_exception_frame *frame) {
    sigaction(SIGSEGV, NULL, &frame->segv_act_old);
    sigaction(SIGILL, NULL, &frame->ill_act_old);
    sigaction(SIGFPE, NULL, &frame->fpe_act_old);
    sigaction(SIGBUS, NULL, &frame->bus_act_old);
}

/* Restores signal context */
void tmk_sig_restore(tmk_exception_frame *frame) {
    sigaction(SIGSEGV, &frame->segv_act_old, NULL);
    sigaction(SIGILL, &frame->ill_act_old, NULL);
    sigaction(SIGFPE, &frame->fpe_act_old, NULL);
    sigaction(SIGBUS, &frame->bus_act_old, NULL);
}

/* Catches and handles some core dumping signals */
void tmk_sig_catch() {
    struct sigaction __segv_act, __ill_act, __fpe_act, __bus_act;

    __segv_act.sa_flags = 0;
    sigemptyset(&__segv_act.sa_mask);
    sigaddset(&__segv_act.sa_mask, SIGINT);
    sigaddset(&__segv_act.sa_mask, SIGALRM);
    sigaddset(&__segv_act.sa_mask, SIGTERM);
    __segv_act.sa_handler = &tmk_sig_handler;
    sigaction(SIGSEGV, &__segv_act, NULL);

    __ill_act.sa_flags = 0;
    sigemptyset(&__ill_act.sa_mask);
    sigaddset(&__ill_act.sa_mask, SIGINT);
    sigaddset(&__ill_act.sa_mask, SIGALRM);
    sigaddset(&__ill_act.sa_mask, SIGTERM);
    __ill_act.sa_handler = &tmk_sig_handler;
    sigaction(SIGILL, &__ill_act, NULL);

    __fpe_act.sa_flags = 0;
    sigemptyset(&__fpe_act.sa_mask);
    sigaddset(&__fpe_act.sa_mask, SIGINT);
    sigaddset(&__fpe_act.sa_mask, SIGALRM);
    sigaddset(&__fpe_act.sa_mask, SIGTERM);
    __fpe_act.sa_handler = &tmk_sig_handler;
    sigaction(SIGFPE, &__fpe_act, NULL);

    __bus_act.sa_flags = 0;
    sigemptyset(&__bus_act.sa_mask);
    sigaddset(&__bus_act.sa_mask, SIGINT);
    sigaddset(&__bus_act.sa_mask, SIGALRM);
    sigaddset(&__bus_act.sa_mask, SIGTERM);
    __bus_act.sa_handler = &tmk_sig_handler;
    sigaction(SIGBUS, &__bus_act, NULL);
}

