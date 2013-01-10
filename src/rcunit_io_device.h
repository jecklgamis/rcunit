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

#ifndef RCUNIT_IO_DEVICE_H
#define RCUNIT_IO_DEVICE_H

/** @brief Output record */
typedef struct rcu_output_record {
RCU_VOID *data;                     /**< Output data */
}RCU_OUTPUT_RECORD;

/** @brief Output handler data structure */
typedef struct rcu_output_handler{
RCU_GENERIC_LIST link;              /**< Link to other handlers (Used by the framework) */
RCU_GENERIC_FUNCTION init;          /**< Initialization function */
RCU_GENERIC_FUNCTION destroy;       /**< Destruction function */
RCU_GENERIC_FUNCTION out_impl;      /**< Output implementation */
}RCU_OUTPUT_HANDLER;

/** @brief I/O device */
typedef struct rcu_io_device {
RCU_INT init_done;
RCU_GENERIC_LIST out_hnd_list;      /**< List of output handlers */
}RCU_IO_DEVICE;

RCU_INT rcu_init_io_device(RCU_IO_DEVICE *io_dev);
RCU_INT rcu_destroy_io_device(RCU_IO_DEVICE *io_dev);
RCU_INT rcu_out_impl(RCU_IO_DEVICE *io_dev,RCU_VOID *data);
RCU_INT rcu_add_dev_out_hnd(RCU_IO_DEVICE *io_dev,RCU_OUTPUT_HANDLER *out_hnd);
RCU_INT rcu_del_dev_out_hnd(RCU_IO_DEVICE *io_dev,RCU_OUTPUT_HANDLER *out_hnd);

#endif /* RCUNIT_IO_DEVICE_H */



