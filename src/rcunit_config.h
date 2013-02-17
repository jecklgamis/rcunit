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

#ifndef RCUNIT_CONFIG_H
#define RCUNIT_CONFIG_H

#ifdef HAVE_CONFIG_H
#include "rcunit_config_env.h"
#endif

/* RCUNIT version string */
#ifdef HAVE_CONFIG_H
#define RCU_VERSION_STRING  PACKAGE_VERSION
#else
#define RCU_VERSION_STRING  "1.0"
#endif /** HAVE_CONFIG_H */

/* Enables file log handler */
#define RCU_ENABLE_FILE_LOG_HANDLER 1

/* Enables stdout log handler */
#define RCU_ENABLE_CONSOLE_LOG_HANDLER 1

/* Enable plain text logs instead of HTML */
#define RCU_ENABLE_EMIT_PLAINTEXT_LOG 1

/* Enable plain text test run report instead of HTML */
#define RCU_ENABLE_EMIT_PLAINTEXT_REPORT 1

/* Enables memory allocation alignment */
#define RCU_ENABLE_MALLOC_ALIGNMENT 1

/* Enables memory tracing facility */
#define RCU_ENABLE_MTRACE 1

/*******************************************************************
 * Oops: There are no user configurable switches beyond this point.
 *  These are used for testing and debugging. Edit at your
 *  own risk.
 *
 *******************************************************************/
//#define RCU_DEBUG 1

#ifdef RCU_DEBUG

/* Enables memory allocation tracing */
#define RCU_ENABLE_TRACE_ALLOC 1

/* Enable random malloc failure */
#define RCU_ENABLE_RANDOM_MALLOC_FAILURE 0

#define RCU_ENABLE_FORCE_MALLOC_FAILURE 0

#endif /** RCU_DEBUG */

#endif /* RCUNIT_CONFIG_H */

