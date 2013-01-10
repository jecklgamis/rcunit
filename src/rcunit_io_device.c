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

#include <rcunit.h>

/**
 *  @brief Initializes an output handler
 *  @param[in] out_hnd Output handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */
RCU_INT rcu_init_out_hnd(RCU_OUTPUT_HANDLER *out_hnd){
    if (out_hnd->init!=RCU_NULL){
        out_hnd->init(RCU_NULL);
    }
    rcu_sys_info();
    return(RCU_E_OK);
}

/**
 *  @brief Destroys an output handler
 *  @param[in] out_hnd Output handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_out_hnd(RCU_OUTPUT_HANDLER *out_hnd){
    if (out_hnd->destroy!=RCU_NULL){
        out_hnd->destroy(RCU_NULL);
    }
    return(RCU_E_OK);
}

/**
 *  @brief Output implementation
 *  @param[in] io_dev I/O device
 *  @param[in] data Output data
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */
RCU_INT rcu_out_impl(RCU_IO_DEVICE *io_dev,RCU_VOID *data){
RCU_GENERIC_LIST *cursor;
RCU_OUTPUT_HANDLER *out_hnd;
RCU_OUTPUT_RECORD out_rec;

    if (!io_dev->init_done){
        return(RCU_E_NG);
    }
    if (data == RCU_NULL){
        return(RCU_E_NG);
    }
    RCU_FOR_EACH_ENTRY(&io_dev->out_hnd_list,cursor){
        out_hnd = (RCU_OUTPUT_HANDLER*)cursor;
        out_rec.data = data;
        out_hnd->out_impl(&out_rec);
    }
#if 0
    fprintf(stdout,"%s",(RCU_CHAR*)data);
#endif
    return(RCU_E_OK);
}

/**
 *  @brief Returns true if the given output handler is registered
 *  @param[in] io_dev I/O device
 *  @param[in] out_hnd Output handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */
RCU_INT rcu_is_out_hnd_found(RCU_IO_DEVICE *io_dev,RCU_OUTPUT_HANDLER
    *out_hnd){
RCU_GENERIC_LIST *cursor;
RCU_OUTPUT_HANDLER *srch_out_hnd;
    if (out_hnd == RCU_NULL){
        return(RCU_FALSE);
    }
    RCU_FOR_EACH_ENTRY(&io_dev->out_hnd_list,cursor){
        srch_out_hnd = (RCU_OUTPUT_HANDLER*)cursor;
        if (srch_out_hnd == out_hnd){
            return(RCU_TRUE);
        }
    }
    return(RCU_FALSE);
}

/**
 *  @brief Registers an output handler
 *  @param[in] io_dev I/O device
 *  @param[in] out_hnd Output handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_add_dev_out_hnd(RCU_IO_DEVICE *io_dev,RCU_OUTPUT_HANDLER
    *out_hnd){
    if (out_hnd == RCU_NULL){
        return(RCU_E_NG);
    }
    if (rcu_is_out_hnd_found(io_dev,out_hnd)){
        return(RCU_E_NG);
    }
    if (out_hnd->init == RCU_NULL || out_hnd->destroy == RCU_NULL ||
        out_hnd->out_impl == RCU_NULL){
        return(RCU_E_NG);
    }
    rcu_init_list(&out_hnd->link);
    rcu_insert_list(&io_dev->out_hnd_list,&out_hnd->link);
    rcu_init_out_hnd(out_hnd);
    return(RCU_E_OK);
}

/**
 *  @brief Deletes an output handler
 *  @param[in] io_dev I/O device
 *  @param[in] out_hnd Output handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */
RCU_INT rcu_del_dev_out_hnd(RCU_IO_DEVICE *io_dev,RCU_OUTPUT_HANDLER
    *out_hnd){
    if (out_hnd == RCU_NULL){
        return(RCU_E_NG);
    }
    if (!rcu_is_out_hnd_found(io_dev,out_hnd)){
        return(RCU_E_NG);
    }
    rcu_remove_list(&out_hnd->link);
    return(RCU_E_OK);
}

/**
 *  @brief Destroys the output device
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_io_device(RCU_IO_DEVICE *io_dev){
RCU_GENERIC_LIST *cursor;
RCU_OUTPUT_HANDLER *out_hnd;

    if (io_dev == RCU_NULL){
        return(RCU_E_NG);
    }
    if (!io_dev->init_done){
        return(RCU_E_NG);
    }
    RCU_FOR_EACH_ENTRY(&io_dev->out_hnd_list,cursor){
        out_hnd = (RCU_OUTPUT_HANDLER*)cursor;
        rcu_destroy_out_hnd(out_hnd);
    }
    io_dev->init_done = RCU_FALSE;
    return(RCU_E_OK);
}

/**
 *  @brief Initializes the output device
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_init_io_device(RCU_IO_DEVICE *io_dev){
    if (io_dev == RCU_NULL){
        return(RCU_E_NG);
    }
    if (io_dev->init_done){
        return(RCU_E_NG);
    }
    memset(io_dev,0x00,sizeof(RCU_IO_DEVICE));
    rcu_init_list(&io_dev->out_hnd_list);
    io_dev->init_done = RCU_TRUE;
    return(RCU_E_OK);
}
