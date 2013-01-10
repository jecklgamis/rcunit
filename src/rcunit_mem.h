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

#ifndef RCUNIT_MEM_H
#define RCUNIT_MEM_H

RCU_INTERFACE RCU_VOID *rcu_malloc(RCU_U4 size);
RCU_INTERFACE RCU_VOID rcu_free(RCU_VOID *addr);
RCU_VOID *rcu_native_malloc(RCU_U4 size);
RCU_VOID rcu_native_free(RCU_VOID *addr);
RCU_MEMCELL *rcu_alloc_mem_cell(RCU_U4 nr_mem_cell);
RCU_VOID rcu_free_mem_cell(RCU_MEMCELL **mem_cell);

#endif /* RCUNIT_MEM_H */



