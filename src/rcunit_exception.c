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

#include "rcunit_exception.h"

/* Current exception stack frame */
rcu_exception_frame *g_curr_excp_frame = NULL;
int g_excp_initialized = 0;

/* Defines an exception object table entry */
#define RCU_DEFINE_EXCEPTION(id, name) \
    { id, name },

/* Global exception object table */
rcu_exception g_rcu_excp_tbl[] = {
        RCU_DEFINE_EXCEPTION(RCU_EXCP_ID_START, "Reserved (RCU_EXCP_ID_START)")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_ABORTTESTRUN, "test function aborted")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_ABORTMODRUN, "test module aborted")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_NULLPOINTER, "null pointer")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_INVPARAM, "invalid parameter")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_SIGSEGV, "segmentation violation")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_SIGILL, "illegal instruction")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_SIGFPE, "floating point")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_SIGBUS, "bus error")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_ASSERTIONFAILURE, "assertion failure")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_ID_END, "Reserved (RCU_EXCP_ID_END)")
        RCU_DEFINE_EXCEPTION(RCU_EXCP_ID_UNKNOWN, "Unknown")
};

RCU_API rcu_exception *rcu_lookup_excp_by_id(rcu_exception_id id) {
    rcu_exception *excp = &g_rcu_excp_tbl[RCU_EXCP_ID_UNKNOWN];
    if (id > RCU_EXCP_ID_START && id < RCU_EXCP_ID_END) {
        excp = &g_rcu_excp_tbl[id];
    }
    return excp;
}

void rcu_init_exception() {
    if (!g_excp_initialized) {
        g_curr_excp_frame = NULL;
    }
}

void rcu_destroy_exception() {
    if (g_excp_initialized) {
    }
}

void rcu_sig_handler(int signo) {
    switch (signo) {
        case SIGSEGV:
            g_curr_excp_frame->excp = rcu_lookup_excp_by_id(RCU_EXCP_SIGSEGV);
            break;
        case SIGILL:
            g_curr_excp_frame->excp = rcu_lookup_excp_by_id(RCU_EXCP_SIGILL);
            break;
        case SIGFPE:
            g_curr_excp_frame->excp = rcu_lookup_excp_by_id(RCU_EXCP_SIGFPE);
            break;
        case SIGBUS:
            g_curr_excp_frame->excp = rcu_lookup_excp_by_id(RCU_EXCP_SIGBUS);
            break;
        default:
            g_curr_excp_frame->excp = rcu_lookup_excp_by_id(RCU_EXCP_ID_UNKNOWN);
            break;
    }
#if RCU_DEBUG_EXCEPTION
    fprintf(stdout, "Received SIGNO = %d (%s)\n", signo,
            g_curr_excp_frame->excp->name);
#endif
    siglongjmp(*(g_curr_excp_frame->env), RCU_EXCEPTION_TYPE_CORE);
}

/*
 * Saves signal context 
 */
void rcu_sig_save(rcu_exception_frame *frame) {
    sigaction(SIGSEGV, NULL, &frame->segv_act_old);
    sigaction(SIGILL, NULL, &frame->ill_act_old);
    sigaction(SIGFPE, NULL, &frame->fpe_act_old);
    sigaction(SIGBUS, NULL, &frame->bus_act_old);
}

/*
 * Restores signal context
 */
void rcu_sig_restore(rcu_exception_frame *frame) {
    sigaction(SIGSEGV, &frame->segv_act_old, NULL);
    sigaction(SIGILL, &frame->ill_act_old, NULL);
    sigaction(SIGFPE, &frame->fpe_act_old, NULL);
    sigaction(SIGBUS, &frame->bus_act_old, NULL);
}

/*
 * Catches and handles some core dumping signals.
 *
 */
void rcu_sig_catch() {
    struct sigaction __segv_act, __ill_act, __fpe_act, __bus_act;

    __segv_act.sa_flags = 0;
    sigemptyset(&__segv_act.sa_mask);
    sigaddset(&__segv_act.sa_mask, SIGINT);
    sigaddset(&__segv_act.sa_mask, SIGALRM);
    sigaddset(&__segv_act.sa_mask, SIGTERM);
    __segv_act.sa_handler = &rcu_sig_handler;
    sigaction(SIGSEGV, &__segv_act, NULL);

    __ill_act.sa_flags = 0;
    sigemptyset(&__ill_act.sa_mask);
    sigaddset(&__ill_act.sa_mask, SIGINT);
    sigaddset(&__ill_act.sa_mask, SIGALRM);
    sigaddset(&__ill_act.sa_mask, SIGTERM);
    __ill_act.sa_handler = &rcu_sig_handler;
    sigaction(SIGILL, &__ill_act, NULL);

    __fpe_act.sa_flags = 0;
    sigemptyset(&__fpe_act.sa_mask);
    sigaddset(&__fpe_act.sa_mask, SIGINT);
    sigaddset(&__fpe_act.sa_mask, SIGALRM);
    sigaddset(&__fpe_act.sa_mask, SIGTERM);
    __fpe_act.sa_handler = &rcu_sig_handler;
    sigaction(SIGFPE, &__fpe_act, NULL);

    __bus_act.sa_flags = 0;
    sigemptyset(&__bus_act.sa_mask);
    sigaddset(&__bus_act.sa_mask, SIGINT);
    sigaddset(&__bus_act.sa_mask, SIGALRM);
    sigaddset(&__bus_act.sa_mask, SIGTERM);
    __bus_act.sa_handler = &rcu_sig_handler;
    sigaction(SIGBUS, &__bus_act, NULL);
}

