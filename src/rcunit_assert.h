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

#ifndef RCUNIT_ASSERT_H
#define RCUNIT_ASSERT_H

/*  RCUNIT Assertion Macro Facility
 *  The following are the commonly used assertion macros. A user-defined
 *  macro can be created.
 *  Considerations in creating a used-defined assertion macro.
 *  1. The assertion implementation function (rcu_assert_impl) has the
 *     following parameter descriptions.
 *     Parameter 1: Asserted condition
 *     Parameter 2: Abort flag (RCU_TRUE or RCU_FALSE)
 *     Parameter 3: Information (stringified)
 *     Parameter 4: Information (stringified)
 *     Parameter 5: __FILE__ (ANSI standard, string)
 *     Parameter 6: __LINE__ (ANSI standard, integer)
 *  2. The abort flag can be set only for failed condition.
 *  3. True condition is depedent on the C language convention :
 *     A non-zero is considered true, and false otherwise.
 */

/** @brief Asserts a condition */
#define RCU_ASSERT(cond) \
 { rcu_assert_impl((cond), RCU_FALSE,("RCU_ASSERT : " #cond),"",__FILE__, __LINE__); }

/** @brief Asserts a condition (fatal) */
#define RCU_ASSERT_FATAL(cond) \
 { rcu_assert_impl((cond), RCU_TRUE,("RCU_ASSERT_FATAL : " #cond),"",__FILE__, __LINE__); }

/** @brief Records an implicitly failed condition described by msg */
#define RCU_FAIL(msg) \
 { rcu_assert_impl(RCU_FALSE, RCU_FALSE,("RCU_FAIL : " #msg),"",__FILE__, __LINE__); }

/** @brief Records an implicitly failed condition descibed by msg (fatal) */
#define RCU_FAIL_FATAL(msg) \
 { rcu_assert_impl(RCU_FALSE, RCU_TRUE,("RCU_FAIL_FATAL : " #msg),"",__FILE__, __LINE__); }

/** @brief Asserts a true condition */
#define RCU_ASSERT_TRUE(cond) \
 { rcu_assert_impl((cond),RCU_FALSE, ("RCU_ASSERT_TRUE : " #cond),"", __FILE__,__LINE__); }

/** @brief Asserts a true condition (fatal) */
#define RCU_ASSERT_TRUE_FATAL(cond) \
 { rcu_assert_impl((cond),RCU_TRUE, ("RCU_ASSERT_TRUE_FATAL : " #cond),"", __FILE__,__LINE__); }

/** @brief Asserts a false condition */
#define RCU_ASSERT_FALSE(cond) \
 { rcu_assert_impl(!(cond),RCU_FALSE, ("RCU_ASSERT_FALSE : " #cond),"", __FILE__,__LINE__); }

/** @brief Asserts a false condition (fatal) */
#define RCU_ASSERT_FALSE_FATAL(cond) \
 { rcu_assert_impl(!(cond),RCU_TRUE, ("RCU_ASSERT_FALSE_FATAL:" #cond),"", __FILE__,__LINE__); }

/** @brief Asserts a null pointer */
#define RCU_ASSERT_NULL(ptr) \
 { rcu_assert_impl(((ptr)==RCU_NULL),RCU_FALSE, ("RCU_ASSERT_NULL:" #ptr),"", __FILE__,__LINE__); }

/** @brief Asserts a null pointer (fatal) */
#define RCU_ASSERT_NULL_FATAL(ptr) \
 { rcu_assert_impl(((ptr)==RCU_NULL),RCU_TRUE, ("RCU_ASSERT_NULL_FATAL:" #ptr),"", __FILE__,__LINE__); }

/** @brief Asserts a non-null pointer */
#define RCU_ASSERT_NOT_NULL(ptr) \
 { rcu_assert_impl(((ptr)!=RCU_NULL),RCU_FALSE, ("RCU_ASSERT_NOT_NULL:" #ptr),"", __FILE__,__LINE__); }

/** @brief Asserts a non-null pointer (fatal) */
#define RCU_ASSERT_NOT_NULL_FATAL(ptr) \
 { rcu_assert_impl(((ptr)!=RCU_NULL),RCU_TRUE, ("RCU_ASSERT_NOT_NULL_FATAL:" #ptr),"", __FILE__,__LINE__); }

#endif /* RCUNIT_ASSERT_H */



