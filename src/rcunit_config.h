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

