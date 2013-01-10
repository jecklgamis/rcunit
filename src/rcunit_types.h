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

#ifndef RCUNIT_TYPES_H
#define RCUNIT_TYPES_H

/** @brief 1-byte unsigned value */
typedef unsigned char RCU_U1;

/** @brief 2-byte unsigned value */
typedef unsigned short RCU_U2;

/** @brief 4-byte unsigned value */
typedef unsigned long RCU_U4;

/** @brief Character data type */
typedef char RCU_CHAR;

/** @brief Native integer data type */
typedef int RCU_INT;

/** @brief void data type */
typedef void RCU_VOID;

/** @brief NULL data type */
#define RCU_NULL NULL

/** @brief The common function format */
typedef RCU_VOID(*RCU_GENERIC_FUNCTION)(RCU_VOID *);

/** @brief The memory cell */
typedef RCU_INT RCU_MEMCELL;

/** @brief Size in bytes of a memory cell (e.g. 32 bits in a 32-bit platform) */
#define RCU_SIZEOF_MEMCELL (sizeof(RCU_INT))

/** @brief Calculates the number of memory cells required to store nbytes */
#define RCU_SIZEIN_MEMCELL(nbytes) (((nbytes)+3) >> 2)

#endif /* RCUNIT_TYPES_H */



