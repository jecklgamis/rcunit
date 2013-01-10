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

#ifndef RCUNIT_CONFIG_H
#define RCUNIT_CONFIG_H

#ifdef HAVE_CONFIG_H
#include <rcunit_config_env.h>
#endif

/** @brief RCUNIT version string */
#ifdef HAVE_CONFIG_H
#define RCU_VERSION_STRING  PACKAGE_VERSION
#else
#define RCU_VERSION_STRING  "0.1"
#endif /** HAVE_CONFIG_H */

/** @brief Enables ANSI I/O facility */
#define RCU_ENABLE_ANSI_FILE_IO 1

#if RCU_ENABLE_ANSI_FILE_IO

/** @brief Enables file log handler */
#define RCU_ENABLE_FILE_LOG_HANDLER 1

/** @brief Enables stdout log handler */
#define RCU_ENABLE_STDOUT_LOG_HANDLER 1

/** @brief Enables report redirection */
#define RCU_ENABLE_REDIRECT_REPORT 0

/** @brief Enables log report redirection */
#define RCU_ENABLE_REDIRECT_LOG 0

#else

/** @brief Enables file log handler */
#define RCU_ENABLE_FILE_LOG_HANDLER 0

/** @brief Enables stdout log handler */
#define RCU_ENABLE_STDOUT_LOG_HANDLER 0

/** @brief Enables report redirection */
#define RCU_ENABLE_REDIRECT_REPORT 1

/** @brief Enables log report redirection */
#define RCU_ENABLE_REDIRECT_LOG 1

#endif /** RCU_ENABLE_ANSI_FILE_IO */

/** @brief Enable plain text logs instead of HTML */
#define RCU_ENABLE_EMIT_PLAINTEXT_LOG 0

/** @brief Enable plain text test run report instead of HTML */
#define RCU_ENABLE_EMIT_PLAINTEXT_REPORT 0

/** @brief Enables memory allocation alignment */
#define RCU_ENABLE_MALLOC_ALIGNMENT 1

/** @brief Enables memory tracing facility */
#define RCU_ENABLE_MTRACE 1


/********************************************************************/
/** Oopss: Thre are no user configurable switches beyond this point.
 *  These are used for debugging and testing purposes. Edit at your
 *  own risk.
 */
/********************************************************************/
#ifdef RCU_DEBUG

/** @brief Enables memory allocation tracing */
#define RCU_ENABLE_TRACE_ALLOC 1

/** @brief Enables function entry point tracing. This applies to test
 *  functions, test init and destroy functions, test run hook, and
 *  assertion hook.
 */
#define RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS 1

/** @brief Enables tracing of RCUNIT function interfaces */
#define RCU_ENABLE_TRACE_IFACE_ENTRY 1

/** @brief Enables mtrace test run report debugging */
#define RCU_ENABLE_DEBUG_REPORT 1

/** @brief Enables mtrace debugging */
#define RCU_ENABLE_DEBUG_MTRACE 1

/** @brief Enables file log handler debugging */
#define RCU_ENABLE_DEBUG_FILE_LOG_HANDLER 1

#endif /** RCU_DEBUG */


#endif /* RCUNIT_CONFIG_H */



