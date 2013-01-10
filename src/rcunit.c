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

/** @brief The test machine */
RCU_TEST_MACHINE the_machine;

/** @brief The RCUNIT I/O device */
RCU_IO_DEVICE g_io_dev;

/** @brief Temporary buffer size */
#define RCU_TEMP_BUFF_SIZE 1024

/** @brief Temporary buffer */
RCU_CHAR g_temp_buff[RCU_TEMP_BUFF_SIZE];

/* forward declarations */
RCU_INT rcu_is_mach_alive(RCU_TEST_MACHINE *mach);
RCU_INT rcu_srch_mod_by_ref_global(RCU_TEST_MODULE *srch_mod,
    RCU_TEST_REGISTRY **which_reg);
RCU_INT rcu_srch_reg_by_ref(RCU_TEST_MACHINE *mach, RCU_TEST_REGISTRY
    *reg);
RCU_INT rcu_destroy_test_dbase(RCU_TEST_MACHINE *mach);
RCU_INT rcu_setup_def_out_hnd(RCU_TEST_MACHINE *mach);

/**
 *   @brief Dumps the given test registry
 *   @param[in] reg Test registry
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_dump_test_reg(RCU_TEST_REGISTRY *reg){
RCU_GENERIC_LIST *cursor1,*cursor2;
RCU_TEST_FUNCTION *func;
RCU_TEST_MODULE *mod;
    if (reg == RCU_NULL){
        reg = RCU_GET_DEF_REG();
    }
    RCU_LOG_INFO_P2("|   +- registry [%s,%s]",reg->name,(reg->enable)?"enabled":"disabled");
    RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor1){
        mod = (RCU_TEST_MODULE*)cursor1;
        RCU_LOG_INFO_P2("|   |   +- module [%s,%s]",mod->name, (mod->enable)?
            "enabled":"disabled");
        RCU_FOR_EACH_ENTRY(&mod->func_list,cursor2){
            func = (RCU_TEST_FUNCTION*)cursor2;
            RCU_LOG_INFO_P2("|   |   |   +-function [%s,%s]",func->name,
                (func->enable)?"enabled":"disabled");
        }
    }
    return(RCU_E_OK);
}

/**
 *   @brief Dumps the entity database of the given test machine
 *   @param[in] mach Test machine
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_dump_test_dbase_impl(RCU_TEST_MACHINE *mach){
RCU_GENERIC_LIST *cursor1;
RCU_TEST_REGISTRY *reg;
    if (mach == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVMACH);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    RCU_LOG_INFO("[ TEST DATABASE DUMP START ]");
    RCU_LOG_INFO("+- Test Entity Database");
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor1){
        reg = (RCU_TEST_REGISTRY*)cursor1;
        rcu_dump_test_reg(reg);
    }
    RCU_LOG_INFO("[ TEST DATABASE DUMP END ]");
    return(RCU_E_OK);
}

/**
 *   @brief Dumps the test entity database
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_dump_test_dbase(){
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_dump_test_dbase");
#endif
    return(rcu_dump_test_dbase_impl(RCU_GET_CURR_MACH()));
}

/**
 *   @brief Creates a failure record
 *   @param[in] info Failure information
 *   @param[in] filepath Filename where the failure was found
 *   @param[in] line_no Line number where the failure was found
 *   @param[in] fatal Flag if this failure is fatal
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_FAILURE_RECORD *rcu_cre_fail_rec(const RCU_CHAR *info, const
    RCU_CHAR *filepath,const RCU_INT line_no, RCU_INT fatal){
RCU_FAILURE_RECORD *fail_rec;
RCU_INT info_len;
const RCU_CHAR *error_str;
    if (filepath == RCU_NULL){
        return(RCU_NULL);
    }
    if ((fail_rec = rcu_malloc(sizeof(RCU_FAILURE_RECORD))) == RCU_NULL){
        return(RCU_NULL);
    }
    memset(g_temp_buff,0x00,RCU_TEMP_BUFF_SIZE);
    error_str = (fatal)?"fatal":"fail";
    sprintf(g_temp_buff,"%s(%d) : %s: %s",filepath,line_no,error_str,info);
    info_len = strlen(g_temp_buff)+1;
    if ((fail_rec->info = rcu_malloc(info_len)) == RCU_NULL){
        rcu_free(fail_rec);
        return(RCU_NULL);
    }
    rcu_init_list(&fail_rec->link);
    strcpy(fail_rec->info,g_temp_buff);
    return(fail_rec);
}


/**
 *   @brief Adds a failure record to the given failure record list
 *   @param[in] fail_rec_list Failure record list
 *   @param[in] info Failure information
 *   @param[in] filepath Filename where the failure was found
 *   @param[in] line_no Line number where the failure was found
 *   @param[in] fatal Flag if this failure is fatal
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_add_fail_rec_impl(RCU_GENERIC_LIST *fail_rec_list,
    const RCU_CHAR *info, const RCU_CHAR *filepath,const
    RCU_INT line_no, RCU_INT fatal){
RCU_FAILURE_RECORD *fail_rec;
RCU_TEST_MACHINE *mach;
    //RCU_LOG_INFO("Creating failure record");
    if (fail_rec_list == RCU_NULL){
        return(RCU_E_NG);
    }
    if (filepath == RCU_NULL){
        return(RCU_E_NG);
    }
    if ((fail_rec = rcu_cre_fail_rec(info,filepath,line_no,fatal)) ==
        RCU_NULL){
        return(RCU_E_NG);
    }
    rcu_insert_list(fail_rec_list,&fail_rec->link);
    mach = RCU_GET_CURR_MACH();
    if (mach->run_ctx == RCU_RUN_CTX_UNKNOWN){
        if (mach->ae.assert_hook != RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
            RCU_LOG_INFO("Invoking assertion hook");
#endif
            mach->ae.assert_hook(&fail_rec);
        }
    }
    RCU_LOG_WARN(fail_rec->info);
    return(RCU_E_OK);
}

/**
 *   @brief Deletes all the failure records from the given list
 *   @param[in] fail_rec_list Failure record list
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_del_all_fail_rec_impl(RCU_GENERIC_LIST *fail_rec_list){
RCU_FAILURE_RECORD *fail_rec;
RCU_GENERIC_LIST *cursor;
    if (fail_rec_list == RCU_NULL){
        return(RCU_E_NG);
    }
    RCU_FOR_EACH_ENTRY(fail_rec_list,cursor){
        fail_rec = (RCU_FAILURE_RECORD*)cursor;
        rcu_free(fail_rec->info);
        RCU_SAVE_CURSOR(cursor)
        rcu_remove_list(cursor);
        rcu_free(fail_rec);
        RCU_RESTORE_CURSOR(cursor)
    }
    return(RCU_E_OK);
}

/**
 *   @brief Adds a failure record to the given test module
 *   @param[in] mod Test module
 *   @param[in] info Failure information
 *   @param[in] filepath Filename where the failure was found
 *   @param[in] line_no Line number where the failure was found
 *   @param[in] fatal Flag if this failure is fatal
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_add_fail_rec_to_mod(RCU_TEST_MODULE *mod, const RCU_CHAR
    *info, const RCU_CHAR *filepath,const RCU_INT line_no, RCU_INT fatal){
    return(rcu_add_fail_rec_impl(&mod->fail_rec_list,info,filepath,line_no,
    fatal));
}

/**
 *   @brief Adds a failure record to the given test function
 *   @param[in] func Test function
 *   @param[in] info Failure information
 *   @param[in] filepath Filename where the failure was found
 *   @param[in] line_no Line number where the failure was found
 *   @param[in] fatal Flag if this failure is fatal
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_add_fail_rec_to_func(RCU_TEST_FUNCTION *func, const RCU_CHAR
    *info, const RCU_CHAR *filepath,const RCU_INT line_no, RCU_INT fatal){
    return(rcu_add_fail_rec_impl(&func->fail_rec_list,info,filepath, line_no,
    fatal));
}

/**
 *   @brief Deletes all the failure records from the given test function
 *   @param[in] func Test function
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_del_all_fail_rec_from_func(RCU_TEST_FUNCTION *func){
    return(rcu_del_all_fail_rec_impl(&func->fail_rec_list));
}

/**
 *   @brief Deletes all the failure records from the given test module
 *   @param[in] mod to the test module
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_del_all_fail_rec_from_mod(RCU_TEST_MODULE *mod){
    return(rcu_del_all_fail_rec_impl(&mod->fail_rec_list));
}

/**
 *  @brief Deletes all the fail records from the test machine
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_del_all_fail_rec(RCU_TEST_MACHINE *mach){
RCU_GENERIC_LIST *cursor1=RCU_NULL,*cursor2=RCU_NULL,*cursor3=RCU_NULL,*cursor4=RCU_NULL;
RCU_TEST_FUNCTION *func=RCU_NULL;
RCU_TEST_MODULE *mod=RCU_NULL;
RCU_TEST_REGISTRY *reg=RCU_NULL;

    RCU_LOG_INFO("Destroying all failure records from test runs");
    if (mach == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMACH));
        return(RCU_E_NG);
    }
    memset(&mach->run_stats,0x00,sizeof(RCU_RUN_STATISTICS));
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor1){
        reg = (RCU_TEST_REGISTRY*)cursor1;
        RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor2){
            mod = (RCU_TEST_MODULE*)cursor2;
            RCU_FOR_EACH_ENTRY(&mod->func_list,cursor3){
                func = (RCU_TEST_FUNCTION*)cursor3;
                rcu_del_all_fail_rec_from_func(func);
            }
            rcu_del_all_fail_rec_from_mod(mod);
        }
    }
    /** Destroy non-test run assertions */
    RCU_LOG_INFO("Destroying all failure records from non test runs");
    rcu_del_all_fail_rec_impl(&mach->ae.assert_list);
    return(RCU_E_OK);
}

/**
 *   @brief Searches a named test function from the given test module
 *   @param[in] mod to the test function
 *   @param[in] name Test function name
 *   @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_TEST_FUNCTION *rcu_srch_test_func_by_name(RCU_TEST_MODULE
    *mod, const RCU_CHAR *name){
RCU_GENERIC_LIST *cursor;
RCU_TEST_FUNCTION *func;
    RCU_FOR_EACH_ENTRY(&mod->func_list,cursor){
        func = (RCU_TEST_FUNCTION*)cursor;
        if (!strcmp(func->name,name)){
            return(func);
        }
    }
    return(RCU_NULL);
}

/**
 *  @brief Searches a named test function from the test machine
 *  @param[in] name Test function name
 *  @param[out] which_mod Test module where the test function was found
 *  @param[out] which_reg Test registry where the test function was found
 *  @return Pointer to the test function or RCU_NULL
 */

RCU_TEST_FUNCTION *rcu_srch_test_func_by_name_global(
    const RCU_CHAR *name, RCU_TEST_MODULE **which_mod,
    RCU_TEST_REGISTRY **which_reg){
RCU_GENERIC_LIST *cursor1,*cursor2;
RCU_TEST_FUNCTION *func;
RCU_TEST_MODULE *mod;
RCU_TEST_REGISTRY *reg;
RCU_TEST_MACHINE *mach;
    *which_reg = RCU_NULL;
    *which_mod = RCU_NULL;
    mach = RCU_GET_CURR_MACH();
    if (name == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVFUNCNAME));
        return(RCU_NULL);
    }
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor1){
        reg = (RCU_TEST_REGISTRY*)cursor1;
        RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor2){
            mod = (RCU_TEST_MODULE*)cursor2;
            func = rcu_srch_test_func_by_name(mod,name);
            if (func != RCU_NULL){
                *which_reg = reg;
                *which_mod = mod;
                return(func);
            }
        }
    }
    return(RCU_NULL);
}

/**
 *  @brief Searches a test function entry point from the given test module
 *  @param[in] mod Test module
 *  @param[in] entry Test function entry point
 *  @return Pointer to the test function or RCU_NULL
 */

RCU_TEST_FUNCTION *rcu_srch_test_func_entry(RCU_TEST_MODULE
    *mod, RCU_GENERIC_FUNCTION entry){
RCU_GENERIC_LIST *cursor;
RCU_TEST_FUNCTION *func;
    RCU_FOR_EACH_ENTRY(&mod->func_list,cursor){
        func = (RCU_TEST_FUNCTION*)cursor;
        if (func->entry == entry){
            return(func);
        }
    }
    return(RCU_NULL);
}

/**
 *  @brief Searches the given test function entry point from the test machine
 *  @param[in] entry Test function entry point
 *  @param[out] which_mod Test module where the test function was found
 *  @param[out] which_reg Test registry where the test function was found
 *  @return Pointer to the test function or RCU_NULL
 */

RCU_TEST_FUNCTION *rcu_srch_test_func_entry_global(
    RCU_GENERIC_FUNCTION entry, RCU_TEST_MODULE **which_mod,
    RCU_TEST_REGISTRY **which_reg){
RCU_GENERIC_LIST *cursor1,*cursor2;
RCU_TEST_FUNCTION *func;
RCU_TEST_MODULE *mod;
RCU_TEST_REGISTRY *reg;
RCU_TEST_MACHINE *mach;
    *which_reg = RCU_NULL;
    *which_mod = RCU_NULL;
    mach = RCU_GET_CURR_MACH();
    if (entry == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVFUNCENTRY));
        return(RCU_NULL);
    }
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor1){
        reg = (RCU_TEST_REGISTRY*)cursor1;
        RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor2){
            mod = (RCU_TEST_MODULE*)cursor2;
            func = rcu_srch_test_func_entry(mod,entry);
            if (func != RCU_NULL){
#ifdef RCU_DEBUG
    RCU_LOG_INFO_P2("Found similar  entry with %s (entry = %p)",
        func->name,func->entry);
#endif
                *which_reg = reg;
                *which_mod = mod;
                return(func);
            }
        }
    }
    return(RCU_NULL);
}

/**
 *  @brief Allocates test function
 *  @param[in] nr_func Number of test functions
 *  @return Pointer to the test function or RCU_NULL
 */

RCU_TEST_FUNCTION *rcu_alloc_test_func(RCU_U4 nr_func){
RCU_TEST_FUNCTION *func;
    if (nr_func == 0){
        return(RCU_NULL);
    }
    func = (RCU_TEST_FUNCTION *)rcu_malloc(sizeof(RCU_TEST_FUNCTION)*nr_func);
    if (func != RCU_NULL){
#if RCU_ENABLE_TRACE_ALLOC
        RCU_LOG_INFO_P1("Test function allocated : %p",func);
#endif
        memset(func,0x00,sizeof(RCU_TEST_FUNCTION));
    }
    return(func);
}

/**
 *  @brief Deallocates a test function
 *  @param[in] func Test function
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_free_test_func(RCU_TEST_FUNCTION *func){
    if (func == RCU_NULL ){
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG_OF(RCU_E_INVFUNC));
        return(RCU_E_NG);
    }
    rcu_free(func);
#if RCU_ENABLE_TRACE_ALLOC
    RCU_LOG_INFO_P1("Test function deallocated : addr = %p",func);
#endif
    return(RCU_E_OK);
}

/**
 *  @brief Registers a named test function entry point to a test module
 *  @param[in] mod Test module
 *  @param[in] entry Test function entry point
 *  @param[in] init Test function's init function entry point
 *  @param[in] destroy Test function's destroy function entry point
 *  @param[in] name Test function name
 *  @param[in] enable Test run flag
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_add_test_func(RCU_TEST_MODULE *mod,
    RCU_GENERIC_FUNCTION entry, RCU_GENERIC_FUNCTION init,
    RCU_GENERIC_FUNCTION destroy, const RCU_CHAR *name,RCU_INT enable){
RCU_TEST_FUNCTION *func;
RCU_TEST_REGISTRY *which_reg;
RCU_TEST_MODULE *which_mod;
RCU_TEST_MACHINE *mach;

#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_add_test_func");
#endif
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (entry == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVFUNCENTRY);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (name == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVFUNCNAME);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    mod = (mod == RCU_NULL)?RCU_GET_DEF_MOD():mod;
    if (!rcu_srch_mod_by_ref_global(mod,&which_reg)){
        RCU_SET_ERCD(RCU_E_UNKNOWNMOD);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    func = rcu_srch_test_func_entry_global(entry,&which_mod,&which_reg);
    if (func != RCU_NULL){
        RCU_SET_ERCD(RCU_E_FUNCEXISTS);
        RCU_LOG_WARN_P3("%s. Similar function entry point with function %s in module %s",RCU_GET_ERR_MSG(),
            func->name,which_mod->name);
        return(RCU_E_NG);
    }
    func = rcu_srch_test_func_by_name_global(name,&which_mod,&which_reg);
    if (func != RCU_NULL){
        RCU_SET_ERCD(RCU_E_FUNCEXISTS);
        RCU_LOG_WARN_P3("%s Similary name with function %s in  module  %s",RCU_GET_ERR_MSG(),
            func->name,which_mod->name);
        return(RCU_E_NG);
    }
    if ((func = rcu_alloc_test_func(1)) == RCU_NULL){
        RCU_SET_ERCD(RCU_E_NOMEM);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    rcu_init_list(&func->link);
    func->name = name;
    func->entry = entry;
    func->init = init;
    func->destroy = destroy;
    func->enable = enable;
    rcu_init_list(&func->fail_rec_list);
    rcu_insert_list(&mod->func_list,&func->link);
    RCU_SET_RUN_STAT(func,RCU_RUN_STAT_NOTTESTED);
    RCU_INCR(mod->nr_test);
    RCU_LOG_INFO_P1("Test function added : %s",func->name);
    return(RCU_E_OK);
}

/**
 *  @brief Registers a test function table to a test module
 *  @param[in] mod Test module
 *  @param[in] func_tbl Test function table
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_add_test_func_tbl(RCU_TEST_MODULE *mod,
    RCU_TEST_FUNCTION_ENTRY *func_tbl){
RCU_TEST_FUNCTION_ENTRY *cursor;
RCU_INT index;

#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_add_test_func_tbl");
#endif
    RCU_LOG_INFO("Adding test function table");
    if (func_tbl == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVFUNCTABLE);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    mod = (mod == RCU_NULL)?RCU_GET_DEF_MOD():mod;
    RCU_FOR_EACH_FUNC_ENTRY(func_tbl,cursor,index){
        if (cursor->entry!=RCU_NULL){
#ifdef RCU_DEBUG
            RCU_LOG_INFO_P2("Adding function %s (entry = %p) ",cursor->name,
                cursor->entry);
#endif
            rcu_add_test_func(mod,cursor->entry,cursor->init,cursor->destroy,
                cursor->name,cursor->enable);
        }else{
            RCU_LOG_WARN_P1("Invalid test function entry. (index = %d)",index);
        }
    }
    RCU_LOG_INFO("Test function table added");
    return(RCU_E_OK);
}


/**
 *  @brief Allocates a test module
 *  @param[in] nr_mod Number of test modules
 *  @return Pointer to test module or RCU_NULL
 */

RCU_TEST_MODULE *rcu_alloc_test_mod(RCU_U4 nr_mod){
RCU_TEST_MODULE *mod;
    if (nr_mod == 0){
        return(RCU_NULL);
    }
    mod = (RCU_TEST_MODULE *)rcu_malloc(sizeof(RCU_TEST_MODULE)*nr_mod);
    if (mod != RCU_NULL){
        memset(mod,0x00,sizeof(RCU_TEST_MODULE));
#if RCU_ENABLE_TRACE_ALLOC
        RCU_LOG_INFO_P1("Test module allocated: addr = %p",mod);
#endif
    }
    return(mod);
}


/**
 *  @brief Deallocates a test module
 *  @param[in] mod Test module
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_free_test_mod(RCU_TEST_MODULE *mod){
    if (mod == RCU_NULL ){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMOD));
        return(RCU_E_NG);
    }
    rcu_free(mod);
#if RCU_ENABLE_TRACE_ALLOC
    RCU_LOG_INFO_P1("Test module deallocated: addr = %p",mod);
#endif
    return(RCU_E_OK);
}

/**
 *  @brief Searches a named test module from the given registry
 *  @param[in] reg Test registry
 *  @param[in] mod_name Test module name
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_TEST_MODULE *rcu_srch_mod_by_name(RCU_TEST_REGISTRY *reg,
    const RCU_CHAR *mod_name){
RCU_GENERIC_LIST *cursor;
RCU_TEST_MODULE *srch_mod;
    if (mod_name == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMODNAME));
        return(RCU_NULL);
    }
    RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor){
        srch_mod = (RCU_TEST_MODULE*)cursor;
        if (!strcmp(srch_mod->name,mod_name)){
            return(srch_mod);
        }
    }
    return(RCU_NULL);
}

/**
 *  @brief Searches a named test module from the test machine
 *  @param[in] mod_name Test module name
 *  @param[out] which_reg Test registry where the test module was found
 *  @return Pointer to the test module or RCU_NULL
 */

RCU_TEST_MODULE *rcu_srch_mod_by_name_global(const RCU_CHAR
    *mod_name, RCU_TEST_REGISTRY **which_reg){
RCU_GENERIC_LIST *cursor1,*cursor2;
RCU_TEST_MODULE *mod;
RCU_TEST_REGISTRY *reg;
RCU_TEST_MACHINE *mach;
    mach = RCU_GET_CURR_MACH();
    *which_reg = RCU_NULL;
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor1){
        reg = (RCU_TEST_REGISTRY*)cursor1;
        RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor2){
            mod = (RCU_TEST_MODULE*)cursor2;
            if (!strcmp(mod->name,mod_name)){
                *which_reg =reg;
                return(mod);
            }
        }
    }
    return(RCU_NULL);
}

/**
 *  @brief Searches a test module from the given test registry
 *  @param[in] reg Test registry
 *  @param[in] srch_mod Test module
 *  @return Boolean value (RCU_TRUE or RCU_FALSE)
 */

RCU_INT rcu_srch_mod_by_ref(RCU_TEST_REGISTRY *reg, RCU_TEST_MODULE
    *srch_mod){
RCU_GENERIC_LIST *cursor;
RCU_TEST_MODULE *mod;
    if (reg == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVREG));
        return(RCU_FALSE);
    }
    if (srch_mod == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMOD));
        return(RCU_FALSE);
    }
    RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor){
        mod = (RCU_TEST_MODULE*)cursor;
        if (mod == srch_mod){
            return(RCU_TRUE);
        }
    }
    return(RCU_FALSE);
}

/**
 *  @brief Searches a test module from the test machine
 *  @param[in] srch_mod Test module
 *  @param[out] which_reg Test registry where the test module was found
 *  @return Boolean value (RCU_TRUE or RCU_FALSE)
 */

RCU_INT rcu_srch_mod_by_ref_global(RCU_TEST_MODULE *srch_mod,
    RCU_TEST_REGISTRY **which_reg){
RCU_GENERIC_LIST *cursor1,*cursor2;
RCU_TEST_MODULE *mod;
RCU_TEST_REGISTRY *reg;
RCU_TEST_MACHINE *mach;
    if (srch_mod == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMOD));
        return(RCU_FALSE);
    }
    mach = RCU_GET_CURR_MACH();
    *which_reg = RCU_NULL;
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor1){
        reg = (RCU_TEST_REGISTRY*)cursor1;
        RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor2){
            mod = (RCU_TEST_MODULE*)cursor2;
            if (mod == srch_mod){
                return(RCU_TRUE);
            }
        }
    }
    return(RCU_FALSE);
}

/**
 *  @brief Initializes a test module
 *  @param[in] mod Test module
 *  @param[in] init Test module's init function entry point
 *  @param[in] destroy Test module's destroy function entry point
 *  @param[in] name Test module name
 *  @param[in] enable Test run flag
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_init_test_mod(RCU_TEST_MODULE *mod, RCU_GENERIC_FUNCTION
    init, RCU_GENERIC_FUNCTION destroy, const RCU_CHAR *name, RCU_INT enable){
    if (mod == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMOD));
        return(RCU_E_NG);
    }
    if (name == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMODNAME));
        return(RCU_E_NG);
    }
    memset(mod,0x00,sizeof(RCU_TEST_MODULE));
    rcu_init_list(&mod->link);
    rcu_init_list(&mod->func_list);
    mod->init=init;
    mod->destroy=destroy;
    mod->name = name;
    mod->enable = enable;
    return(RCU_E_OK);
}


/**
 *  @brief Creates a test module
 *  @param[in] name Test module name
 *  @param[in] init Test module's init function entry point
 *  @param[in] destroy Test module's destroy function entry point
 *  @param[in] enable Test run flag
 *  @return Pointer to the test module or RCU_NULL
 */

RCU_INTERFACE RCU_TEST_MODULE *rcu_cre_test_mod(const RCU_CHAR *name,
    RCU_GENERIC_FUNCTION init, RCU_GENERIC_FUNCTION destroy, RCU_INT enable){
RCU_TEST_MODULE *mod;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_cre_test_mod");
#endif
    if ((mod = rcu_alloc_test_mod(1))==RCU_NULL){
        RCU_SET_ERCD(RCU_E_NOMEM);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_NULL);
    }
    if (name != RCU_NULL && !strcmp(name,RCU_DEFAULT_MODULE_NAME)){
        RCU_SET_ERCD(RCU_E_INVMODNAME);
        RCU_LOG_WARN_P2("%s (%s is reserved)",RCU_GET_ERR_MSG(),RCU_DEFAULT_MODULE_NAME);
        rcu_free_test_mod(mod);
        return(RCU_NULL);
    }
    if ((rcu_init_test_mod(mod,init,destroy,name,enable))==RCU_E_NG){
        rcu_free_test_mod(mod);
        return(RCU_NULL);
    }
    RCU_LOG_INFO_P1("Test module created : %s",mod->name);
    return(mod);
}

/**
 *  @brief Registers a test module to the given test registry
 *  @param[in] reg Test registry
 *  @param[in] mod Test module
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_add_test_mod(RCU_TEST_REGISTRY *reg,RCU_TEST_MODULE *mod){
RCU_TEST_REGISTRY *which_reg;
RCU_TEST_MODULE *srch_mod;
RCU_TEST_MACHINE *mach;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_add_test_mod");
#endif
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (mod == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVMOD);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    srch_mod = rcu_srch_mod_by_name_global(mod->name,&which_reg);
    if (srch_mod != RCU_NULL){
        RCU_SET_ERCD(RCU_E_MODEXISTS);
        RCU_LOG_WARN_P3("%s (module = %s, registry = %s)",RCU_GET_ERR_MSG(),
            srch_mod->name,which_reg->name);
        return(RCU_E_NG);
    }
    reg = (reg == RCU_NULL)?RCU_GET_DEF_REG():reg;
    if (!rcu_srch_reg_by_ref(RCU_GET_CURR_MACH(),reg)){
        RCU_SET_ERCD(RCU_E_UNKNOWNREG);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (rcu_srch_mod_by_ref_global(mod,&which_reg)){
        RCU_SET_ERCD(RCU_E_MODEXISTS);
        RCU_LOG_WARN_P3("%s (module = %s, registry = %s)",RCU_GET_ERR_MSG(),
            srch_mod->name,which_reg->name);
        return(RCU_E_NG);
    }
    rcu_init_list(&mod->link);
    rcu_insert_list(&reg->mod_list,&mod->link);
    RCU_INCR(reg->nr_mod);
    rcu_init_list(&mod->fail_rec_list);
    RCU_LOG_INFO_P1("Test module added : %s",mod->name);
    return(RCU_E_OK);
}

/**
 *  @brief Destroys a test module
 *  @param[in] mod Test module
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_destroy_test_mod(RCU_TEST_MODULE *mod){
RCU_GENERIC_LIST *cursor;
RCU_TEST_FUNCTION *func;
RCU_TEST_MACHINE *mach;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_destroy_test_mod");
#endif
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (mod == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVMOD);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    RCU_LOG_INFO_P1("Destroying test module : %s",mod->name);
    RCU_FOR_EACH_ENTRY(&mod->func_list,cursor){
        func = (RCU_TEST_FUNCTION*)cursor;
        RCU_SAVE_CURSOR(cursor)
        rcu_remove_list(cursor);
        rcu_free_test_func(func);
        RCU_RESTORE_CURSOR(cursor)
    }
    /* Unlink this test module */
    rcu_remove_list(&mod->link);

    /* Do not deallocate default test module! */
    if (strcmp(mod->name,RCU_DEFAULT_MODULE_NAME)){
        RCU_LOG_INFO_P1("Test module destroyed : %s",mod->name);
        rcu_free_test_mod(mod);
    }
   return(RCU_E_OK);
}

/**
 *  @brief Registers a test module table to a test registry
 *  @param[in] reg Test registry
 *  @param[in] mod_tbl Test module table
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_add_test_mod_tbl(RCU_TEST_REGISTRY *reg,
    RCU_TEST_MODULE_ENTRY *mod_tbl){
RCU_TEST_MODULE_ENTRY *cursor;
RCU_TEST_MODULE *mod;
RCU_INT mod_index;
RCU_INT ercd;
RCU_TEST_MACHINE *mach;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_add_test_mod_tbl");
#endif
    RCU_LOG_INFO("Adding test module table");
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (mod_tbl == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVMODTABLE);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    RCU_FOR_EACH_MOD_ENTRY(mod_tbl,cursor,mod_index){
        if (cursor->func_tbl == RCU_NULL){
            RCU_LOG_WARN_P1("Invalid test function table (index = %d)",
                mod_index);
            continue;
        }
        mod = rcu_cre_test_mod(cursor->name,cursor->init,cursor->destroy,
            cursor->enable);
        if (mod == RCU_NULL){
            RCU_LOG_WARN_P1("Unable to create test module. (index = %d)",
                mod_index);
            continue;
        }
        ercd = rcu_add_test_mod(reg,mod);
        if (ercd == RCU_E_NG){
            RCU_LOG_WARN_P1("Unable to add test module. (index = %d)",
                mod_index);
            rcu_destroy_test_mod(mod);
            continue;
        }
        ercd = rcu_add_test_func_tbl(mod,cursor->func_tbl);
        if (ercd == RCU_E_NG){
            RCU_LOG_WARN_P1("Unable to add test function table. (index = %d)",
                mod_index);
        }
    }
    RCU_LOG_INFO("Test module table added");
    return(RCU_E_OK);
}

/**
 *  @brief Allocates test registry
 *  @param[in] nr_reg Number of test registry
 *  @return Pointer to test registry or RCU_NULL
 */

RCU_TEST_REGISTRY *rcu_alloc_test_reg(RCU_U4 nr_reg){
RCU_TEST_REGISTRY *reg;
    if (nr_reg == 0){
        return(RCU_NULL);
    }
    reg = (RCU_TEST_REGISTRY *)rcu_malloc(sizeof(RCU_TEST_REGISTRY)*nr_reg);
    if (reg!=RCU_NULL){
        memset(reg,0x00,sizeof(RCU_TEST_REGISTRY));
#if RCU_ENABLE_TRACE_ALLOC
        RCU_LOG_INFO_P1("Test registry allocated : addr = %p",reg);
#endif
    }
    return(reg);
}

/**
 *  @brief Deallocates a test registry
 *  @param[in] reg Test registry
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_free_test_reg(RCU_TEST_REGISTRY *reg){
    if (reg == RCU_NULL ){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVREG));
        return(RCU_E_NG);
    }
    rcu_free(reg);
#if RCU_ENABLE_TRACE_ALLOC
    RCU_LOG_INFO_P1("Test registry deallocated: addr = %p",reg);
#endif
    return(RCU_E_OK);
}

/**
 *  @brief Searches a named test registry from the given test machine
 *  @param[in] mach Test machine
 *  @param[in] reg_name Test registry
 *  @return Pointer to test registry or RCU_NULL
 */

RCU_TEST_REGISTRY *rcu_srch_reg_by_name(RCU_TEST_MACHINE
    *mach, const RCU_CHAR *reg_name){
RCU_GENERIC_LIST *cursor;
RCU_TEST_REGISTRY *srch_reg;
    if (mach == RCU_NULL ){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMACH));
        return(RCU_NULL);
    }
    if (reg_name == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVREGNAME));
        return(RCU_NULL);
    }
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor){
        srch_reg = (RCU_TEST_REGISTRY*)cursor;
        if (!strcmp(reg_name,srch_reg->name)){
            return(srch_reg);
        }
    }
    return(RCU_NULL);
}

/**
 *  @brief Searches the given test registry from the given test machine
 *  @param[in] mach Test machine
 *  @param[in] reg Test registry
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_srch_reg_by_ref(RCU_TEST_MACHINE *mach, RCU_TEST_REGISTRY
    *reg){
RCU_TEST_REGISTRY *srch_reg;
    if (reg == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVREG));
        return(RCU_FALSE);
    }
    srch_reg = rcu_srch_reg_by_name(mach,reg->name);
    return((srch_reg == RCU_NULL)?RCU_FALSE:RCU_TRUE);
}

/**
 *  @brief Initializes a test registry
 *  @param[in] reg Test registry
 *  @param[in] name Test registry name
 *  @param[in] enable Test run flag
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_init_test_reg(RCU_TEST_REGISTRY *reg,const RCU_CHAR
    *name, RCU_INT enable){
    if (reg == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVREG));
        return(RCU_E_NG);
    }
    if (name == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVREGNAME));
        return(RCU_E_NG);
    }
    memset(reg,0x00,sizeof(RCU_TEST_REGISTRY));
    rcu_init_list(&reg->link);
    rcu_init_list(&reg->mod_list);
    reg->name = name;
    reg->enable = enable;
    return(RCU_E_OK);
}

/**
 *  @brief Creates a test registry
 *  @param[in] name Test registry name
 *  @param[in] enable Test run flag
 *  @return Test registry or RCU_NULL
 */

RCU_INTERFACE RCU_TEST_REGISTRY *rcu_cre_test_reg(const RCU_CHAR *name,
    RCU_INT enable){
RCU_TEST_REGISTRY *reg;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_cre_test_reg");
#endif
    if ((reg = rcu_alloc_test_reg(1)) == RCU_NULL){
        RCU_SET_ERCD(RCU_E_NOMEM);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_NULL);
    }
    if (name != RCU_NULL && !strcmp(name,RCU_DEFAULT_REGISTRY_NAME)){
        RCU_SET_ERCD(RCU_E_INVREGNAME);
        RCU_LOG_WARN_P2("%s (%s is reserved)",RCU_GET_ERR_MSG(),
            RCU_DEFAULT_REGISTRY_NAME);
        rcu_free_test_reg(reg);
        return(RCU_NULL);
    }
    if ((rcu_init_test_reg(reg,name,enable)) == RCU_E_NG){
        rcu_free_test_reg(reg);
        return(RCU_NULL);
    }
    RCU_LOG_INFO_P1("Test registry created : %s",reg->name);
    return(reg);
}

/**
 *  @brief Destroys a test registry
 *  @param[in] reg Test registry
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_destroy_test_reg(RCU_TEST_REGISTRY *reg){
RCU_GENERIC_LIST *cursor;
RCU_TEST_MODULE *mod;
RCU_TEST_MACHINE *mach;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_destroy_test_reg");
#endif
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (reg == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVREG);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    RCU_LOG_INFO_P1("Destroying test registry: %s",reg->name);
    RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor){
        mod = (RCU_TEST_MODULE*)cursor;
        RCU_SAVE_CURSOR(cursor)
        rcu_remove_list(cursor);
        rcu_destroy_test_mod(mod);
        RCU_RESTORE_CURSOR(cursor)
    }
    /* Unlink this test registry */
    rcu_remove_list(&reg->link);

    /* Do not destroy pre-allocated default test registry! */
    if (strcmp(reg->name,RCU_DEFAULT_REGISTRY_NAME)){
        rcu_free_test_reg(reg);
    }
    return(RCU_E_OK);
}

/**
 *  @brief Registers a test registry to the test machine
 *  @param[in] reg Test registry
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_add_test_reg(RCU_TEST_REGISTRY *reg){
RCU_TEST_MACHINE *mach;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_add_test_reg");
#endif
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (reg == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVREG);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (rcu_srch_reg_by_ref(mach,reg)){
        RCU_SET_ERCD(RCU_E_REGEXISTS);
        RCU_LOG_WARN_P2("%s (%s)",RCU_GET_ERR_MSG(),reg->name);
        return(RCU_E_NG);
    }
    rcu_init_list(&reg->link);
    rcu_insert_list(&mach->reg_list,&reg->link);
    RCU_INCR(mach->nr_reg);
    RCU_LOG_INFO_P1("Test registry added : %s",reg->name);
    return(RCU_E_OK);
}

/**
 *  @brief Restarts the assertion engine of the given test machine
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_restart_assert_engine(RCU_TEST_MACHINE *mach){
    /** This destroys all the non-test run assertion records */
    memset(&mach->ae,0x00,sizeof(RCU_ASSERTION_ENGINE));
    RCU_LOG_INFO("Assertion engine restarted");
    return(RCU_E_OK);
}

/**
 *  @brief Stops the assertion engine of the given test machine
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_stop_assert_engine(RCU_TEST_MACHINE *mach){
    RCU_LOG_INFO("Assertion engine stopped");
    return(RCU_E_OK);
}

/**
 *  @brief Sets a non - test run assertion hook
 *  @param[in] assert_hook Assertion hook
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_set_assert_hook(RCU_GENERIC_FUNCTION assert_hook){
RCU_TEST_MACHINE *mach;
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    mach->ae.assert_hook = assert_hook;
    RCU_LOG_INFO("Assertion hook set");
    return(RCU_E_OK);
}

/**
 *  @brief Implementation of the assertion mechanism
 *  @param[in] cond Asserted condition
 *  @param[in] fatal Flag if the assertion is fatal
 *  @param[in] info1 Assertion information
 *  @param[in] info2 Assertion information
 *  @param[in] fname File name where the assertion was found
 *  @param[in] line Line number where the assertion was found
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_assert_impl(RCU_INT cond, RCU_INT fatal,
    const RCU_CHAR *info1, const RCU_CHAR *info2, const RCU_CHAR *fname,
    RCU_INT line){
RCU_TEST_MACHINE *mach = RCU_NULL;
RCU_TEST_FUNCTION *func=RCU_NULL;
RCU_TEST_MODULE *mod=RCU_NULL;
RCU_INT run_ctx;

    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    func = RCU_GET_CURR_FUNC(mach);
    mod = RCU_GET_CURR_MOD(mach);
    run_ctx = RCU_GET_RUN_CTX(mach);

    switch(run_ctx){
        case RCU_RUN_CTX_MOD_INIT:
        case RCU_RUN_CTX_MOD_DESTROY:
            if (!cond){
                if (run_ctx == RCU_RUN_CTX_MOD_INIT){
                    RCU_SET_RUN_STAT(mod,RCU_RUN_STAT_FAIL_INIT);
                }else {
                    RCU_SET_RUN_STAT(mod,RCU_RUN_STAT_FAIL_DESTROY);
                }
                /** Note: an assertion failure here is implicitly fatal */
                rcu_add_fail_rec_to_mod(mod,info1,fname,line,RCU_TRUE);
                RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ABORTMODRUN));
            }else{
                //nop
            }
            break;
        case RCU_RUN_CTX_FUNC_INIT:
        case RCU_RUN_CTX_FUNC_DESTROY:
            if (!cond){
                if (run_ctx == RCU_RUN_CTX_FUNC_INIT){
                    RCU_SET_RUN_STAT(func,RCU_RUN_STAT_FAIL_INIT);
                }else{
                    RCU_SET_RUN_STAT(func,RCU_RUN_STAT_FAIL_DESTROY);
                }
                /** Note: an assertion failure here is implicitly fatal */
                rcu_add_fail_rec_to_func(func,info1,fname,line,RCU_TRUE);
                RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ABORTTESTRUN));
            }else{
                //nop
            }
            break;
        case RCU_RUN_CTX_FUNC:
            if (!cond){
                if (fatal){
                    rcu_add_fail_rec_to_func(func,info1,fname,line,RCU_TRUE);
                    RCU_INCR(func->nr_fatal_assert);
                    RCU_THROW(RCU_GET_EXCP(RCU_EXCP_ABORTTESTRUN));
                }else {
                    RCU_INCR(func->nr_fail_assert);
                    rcu_add_fail_rec_to_func(func,info1,fname,line,RCU_FALSE);
                }
            }else{
                RCU_INCR(func->nr_succ_assert);
            }
            break;
        case RCU_RUN_CTX_UNKNOWN :
            /** This is a non-test run assertion */
            if (!cond){
                rcu_add_fail_rec_impl(&mach->ae.assert_list,info1,fname,line,RCU_FALSE);
            }
            break;
        default:
            //Ignore assertion failure from unknown code locations
            break;
    }
    return(RCU_E_OK);
}

/**
 *  @brief Returns true if there are non run context assertions
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_have_asserts(){
RCU_TEST_MACHINE *mach;
    mach = RCU_GET_CURR_MACH();
    return(!rcu_is_list_empty(&mach->ae.assert_list));
}

/**
 *  @brief Dumps non - test run assertions found
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_dump_asserts(){
RCU_GENERIC_LIST *cursor;
RCU_TEST_MACHINE *mach;
RCU_FAILURE_RECORD *fail_rec;

    mach = RCU_GET_CURR_MACH();
    if (!rcu_have_asserts()){
        return(RCU_E_OK);
    }
    RCU_LOG_WARN("[ NON TEST RUN ASSERTIONS DUMP START]");
    RCU_FOR_EACH_ENTRY(&mach->ae.assert_list,cursor){
        fail_rec = (RCU_FAILURE_RECORD*)cursor;
        RCU_LOG_WARN_P1("%s",fail_rec->info);
    }
    RCU_LOG_WARN("[ NON TEST RUN ASSERTIONS DUMP END]");
    return(RCU_E_OK);
}

/**
 *  @brief Initializes the given test machine
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_init_test_mach(RCU_TEST_MACHINE *mach){
    if (mach == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMACH));
        return(RCU_E_NG);
    }
    RCU_LOG_INFO("Initializing test machine");
    memset(mach,0x00,sizeof(RCU_TEST_MACHINE));
    rcu_init_list(&mach->link);
    rcu_init_list(&mach->reg_list);
    rcu_init_list(&mach->ae.assert_list);
    mach->name =RCU_DEFAULT_MACHINE_NAME;
    mach->init_done = RCU_TRUE;
    RCU_SET_RUN_CTX(mach,RCU_RUN_CTX_UNKNOWN);
    /** Initialize memory tracing facility */
#if RCU_ENABLE_MTRACE
    rcu_init_mtrace();
#endif
    /* Initialize exception mechanism */
    rcu_init_excp();
    /** Initialize default test registry and test module */
    rcu_init_test_reg(&mach->def_reg,RCU_DEFAULT_REGISTRY_NAME,RCU_TRUE);
    rcu_add_test_reg(&mach->def_reg);
    rcu_init_test_mod(&mach->def_mod,RCU_NULL,RCU_NULL,RCU_DEFAULT_MODULE_NAME,RCU_TRUE);
    rcu_add_test_mod(&mach->def_reg,&mach->def_mod);

    return(RCU_E_OK);
}

/**
 *  @brief Destroys the given test machine
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_test_mach(RCU_TEST_MACHINE *mach){

    RCU_LOG_INFO("Destroying test machine");
    /** Destroy exception mechanism */
    rcu_destroy_excp();
    /** Destroy all failure records */
    rcu_del_all_fail_rec(mach);
    /** Destroy test entity database */
    rcu_destroy_test_dbase(mach);
#if RCU_ENABLE_MTRACE
    RCU_CHECK_MEMORY_LEAK_INTERNAL();
    RCU_CHECK_MEMORY_LEAK();
#endif
    /** Destroy memory tracing facility */
#if RCU_ENABLE_MTRACE
    rcu_destroy_mtrace();
#endif
    mach->init_done = RCU_FALSE;
    return(RCU_E_OK);
}

/**
 *  @brief Destroys the test entity database of the given machine
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_destroy_test_dbase(RCU_TEST_MACHINE *mach){
RCU_GENERIC_LIST *cursor;
RCU_TEST_REGISTRY *reg;
    RCU_LOG_INFO("Destroying test database");
    if (mach == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMACH));
        return(RCU_E_NG);
    }
    if (!rcu_is_mach_alive(mach)){
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG_OF(RCU_E_MACHNOINIT));
        return(RCU_E_NG);
    }
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor){
        reg = (RCU_TEST_REGISTRY*)cursor;
        RCU_SAVE_CURSOR(cursor)
        rcu_remove_list(cursor);
        rcu_destroy_test_reg(reg);
        RCU_RESTORE_CURSOR(cursor)
    }
    return(RCU_E_OK);
}

/**
 *  @brief Returns true if the given test machine is initialized
 *  @param[in] mach Test machine
 *  @return Boolean value (RCU_TRUE or RCU_FALSE)
 */

RCU_INT rcu_is_mach_alive(RCU_TEST_MACHINE *mach){
    if (mach == RCU_NULL){
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG_OF(RCU_E_INVMACH));
        return(RCU_FALSE);
    }
    return(mach->init_done);
}

/**
 *  @brief Restarts the given test machine
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_restart_mach(RCU_TEST_MACHINE *mach){
    RCU_LOG_INFO("Restarting machine");
    return(RCU_E_OK);
}

/**
 *  @brief Stops the given test machine
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_stop_mach(RCU_TEST_MACHINE *mach){
    RCU_LOG_INFO("Stopping machine");
    rcu_stop_assert_engine(mach);
    return(RCU_E_OK);
}

/**
 *  @brief Runs the given test module
 *  @param[in] mach Test machine
 *  @param[in] mod Test module
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_run_test_mod_impl(RCU_TEST_MACHINE *mach,
    RCU_TEST_MODULE *mod){
RCU_GENERIC_LIST *cursor3;
RCU_TEST_FUNCTION *func;
RCU_TEST_REGISTRY *reg;
RCU_INT run_event;

    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    mod = (mod == RCU_NULL)?RCU_GET_DEF_MOD():mod;
    if (!mod->enable){
        RCU_SET_ERCD(RCU_E_ENTITYDISABLED);
        RCU_LOG_WARN_P2("%s (module = %s)",RCU_GET_ERR_MSG(),mod->name);
        return(RCU_E_NG);
    }
    /** Execute test run hook if executed at module level */
    if (RCU_GET_RUN_LEVEL(mach) == RCU_RUN_LEVEL_MOD){
        if (mach->run_hook != RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
            RCU_LOG_INFO("Invoking test run hook");
#endif
            run_event = RCU_TEST_RUN_STARTED;
            mach->run_hook(&run_event);
        }
    }
    reg = RCU_GET_CURR_REG(mach);
    RCU_CLEAR_ABORT(mod);
    RCU_LOG_INFO_P1("Running test module : %s",mod->name);
    if (mod->init!=RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
        RCU_LOG_INFO("Invoking test module init function");
#endif
        RCU_TRY{
            RCU_SET_RUN_CTX(mach,RCU_RUN_CTX_MOD_INIT);
            mod->init(RCU_NULL);
        }RCU_CATCH(e){
            RCU_LOG_WARN("Test module init function failed");
            RCU_SET_ABORT(mod);
            RCU_SET_RUN_STAT(mod,RCU_RUN_STAT_FAIL_INIT);
            /** Invoke the test run hook here */
            if (RCU_GET_RUN_LEVEL(mach) == RCU_RUN_LEVEL_MOD){
                if (mach->run_hook != RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
                    RCU_LOG_INFO("Running test run hook");
#endif
                    run_event = RCU_TEST_RUN_FINISHED;
                    mach->run_hook(&run_event);
                }
            }
            RCU_SET_ERCD(RCU_E_EXEC);
            RCU_LOG_WARN_P2("%s (module = %s)",RCU_GET_ERR_MSG(),mod->name);
            return(RCU_E_NG);
        }RCU_END_CATCH
    }

    RCU_FOR_EACH_ENTRY(&mod->func_list,cursor3){
        func = (RCU_TEST_FUNCTION*)cursor3;
        if (!func->enable){
            RCU_LOG_WARN_P1("Disabled test function : %s",func->name);
            continue;
        }
        RCU_SET_CURR_FUNC(mach,func);
        RCU_SET_RUN_STAT(func,RCU_RUN_STAT_NOTTESTED);
        RCU_CLEAR_ABORT(func);
        RCU_RESET(func->nr_fail_assert);
        RCU_RESET(func->nr_succ_assert);
        RCU_LOG_INFO_P1("Running test function : %s",func->name);
        if (func->init!=RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
            RCU_LOG_INFO("Invoking init function");
#endif
            RCU_SET_RUN_CTX(mach,RCU_RUN_CTX_FUNC_INIT);
            RCU_TRY {
                func->init(RCU_NULL);
            }RCU_CATCH(e){
                RCU_LOG_WARN("Test function init function failed. Aborting function");
                /** Mark the test function as "aborted" and "failed" */
                RCU_SET_ABORT(func);
                RCU_SET_RUN_STAT(func,RCU_RUN_STAT_FAIL);
                continue;
            }RCU_END_CATCH
        }
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
        //RCU_LOG_INFO("Running test now...");
#endif
        RCU_SET_RUN_CTX(mach,RCU_RUN_CTX_FUNC)
        RCU_TRY {
            func->entry(RCU_NULL);
        }RCU_CATCH(e){
           RCU_SET_RUN_STAT(func,RCU_RUN_STAT_FAIL);
           RCU_SET_ABORT(func);
        }RCU_END_CATCH

        if (func->destroy != RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
            RCU_LOG_INFO("Invoking destroy function");
#endif
            RCU_SET_RUN_CTX(mach,RCU_RUN_CTX_FUNC_DESTROY);
            RCU_TRY {
                func->destroy(RCU_NULL);
            }RCU_CATCH(e){
                RCU_LOG_WARN("Test function destroy function failed");
                /** Mark the test function as "failed" */
                RCU_SET_RUN_STAT(func,RCU_RUN_STAT_FAIL);
            }RCU_END_CATCH
        }
        /** Set the test function status if no failures in the init or
         *  destroy functions
         */
        if (!RCU_RUN_STAT_INIT_FAILED(func) &&
            !RCU_RUN_STAT_DESTROY_FAILED(func)){
            if (func->nr_fail_assert == 0 && func->nr_fatal_assert == 0){
                RCU_SET_RUN_STAT(func,RCU_RUN_STAT_SUCC);
            }else{
                RCU_SET_RUN_STAT(func,RCU_RUN_STAT_FAIL);
            }
        }else{
        }
    }
    if (mod->destroy!=RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
        RCU_LOG_INFO("Invoking test module destroy function");
#endif
        RCU_SET_RUN_CTX(mach,RCU_RUN_CTX_MOD_DESTROY);
        RCU_TRY{
            mod->destroy(RCU_NULL);
        }RCU_CATCH(e){
            RCU_LOG_WARN("Test module destroy function failed");
            RCU_SET_RUN_STAT(mod,RCU_RUN_STAT_FAIL_DESTROY);
        }RCU_END_CATCH
    }
    if (RCU_GET_RUN_LEVEL(mach) == RCU_RUN_LEVEL_MOD){
        if (mach->run_hook != RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
            RCU_LOG_INFO("Invoking test run hook");
#endif
            run_event = RCU_TEST_RUN_FINISHED;
            mach->run_hook(&run_event);
        }
    }

    return(RCU_E_OK);
}

/**
 *  @brief Runs a test registry
 *  @param[in] mach Test machine
 *  @param[in] reg Test module
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_run_test_reg_impl(RCU_TEST_MACHINE *mach,
    RCU_TEST_REGISTRY *reg){
RCU_GENERIC_LIST *cursor2;
RCU_TEST_MODULE *mod;
RCU_INT run_event;

    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    reg = (reg == RCU_NULL)?&mach->def_reg:reg;
    if (!reg->enable){
        RCU_SET_ERCD(RCU_E_ENTITYDISABLED);
        RCU_LOG_WARN_P2("%s (registry = %s)",RCU_GET_ERR_MSG(),reg->name);
        return(RCU_E_NG);
    }
    if (RCU_GET_RUN_LEVEL(mach) == RCU_RUN_LEVEL_REG){
        if (mach->run_hook != RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
            RCU_LOG_INFO("Invoking test run hook");
#endif
            run_event = RCU_TEST_RUN_STARTED;
            mach->run_hook(&run_event);
        }
    }
    RCU_LOG_INFO_P1("Running test registry : %s",reg->name);
    RCU_FOR_EACH_ENTRY(&reg->mod_list,cursor2){
        mod = (RCU_TEST_MODULE*)cursor2;
        RCU_SET_CURR_MOD(mach,mod);
        rcu_run_test_mod_impl(mach,mod);
    }
    if (RCU_GET_RUN_LEVEL(mach) == RCU_RUN_LEVEL_REG){
        if (mach->run_hook != RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
            RCU_LOG_INFO("Invoking test run hook");
#endif
            run_event = RCU_TEST_RUN_FINISHED;
            mach->run_hook(&run_event);
        }
    }
    return(RCU_E_OK);
}

/**
 *  @brief Runs the given test machine
 *  @param[in] mach Test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_run_test_mach_impl(RCU_TEST_MACHINE *mach){
RCU_GENERIC_LIST *cursor1;
RCU_TEST_REGISTRY *reg;
RCU_INT run_event;

    if (mach == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVMACH);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (!mach->init_done){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    if (mach->run_hook != RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
            RCU_LOG_INFO("Invoking test run hook");
#endif
        run_event = RCU_TEST_RUN_STARTED;
        mach->run_hook(&run_event);
    }
    RCU_LOG_INFO_P1("Running test machine : %s",mach->name);
    RCU_FOR_EACH_ENTRY(&mach->reg_list,cursor1){
        reg = (RCU_TEST_REGISTRY*)cursor1;
        RCU_SET_CURR_REG(mach,reg);
        rcu_run_test_reg_impl(mach,reg);
    }
    if (mach->run_hook != RCU_NULL){
#if RCU_ENABLE_TRACE_FUNCTION_ENTRY_POINTS
            RCU_LOG_INFO("Invoking test run hook");
#endif
        run_event = RCU_TEST_RUN_FINISHED;
        mach->run_hook(&run_event);
    }
    return(RCU_E_OK);
}

/**
 *  @brief Runs a test module
 *  @param[in] mod Test module
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_run_test_mod(RCU_TEST_MODULE *mod){
RCU_TEST_MACHINE *mach;
RCU_CHAR ts_buff[RCU_TSTAMP_BUFF_SIZE];
    if (mod == RCU_NULL){
        mod = RCU_GET_DEF_MOD();
    }
    mach = RCU_GET_CURR_MACH();
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO_P1("Test run started %s",ts_buff);
    rcu_restart_mach(mach);
    RCU_SET_RUN_LEVEL(mach,RCU_RUN_LEVEL_MOD);
    RCU_SET_CURR_MOD(mach,mod);
    rcu_run_test_mod_impl(mach,mod);
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO_P1("Test run finished %s",ts_buff);
    rcu_stop_mach(mach);
    rcu_gen_test_run_report(mach);
    return(RCU_E_OK);
}

/**
 *  @brief Runs a named test module
 *  @param[in] name Test module name
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_run_test_mod_by_name(const RCU_CHAR *name){
RCU_TEST_MODULE *mod;
RCU_TEST_REGISTRY *which_reg;

    if (name == RCU_NULL){
        mod = RCU_GET_DEF_MOD();
    }else{
        mod = rcu_srch_mod_by_name_global(name,&which_reg);
    }
    if (mod == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVMOD);
        RCU_LOG_WARN_P2("%s (%s)",RCU_GET_ERR_MSG(),name);
        return(RCU_E_NG);
    }
    return(rcu_run_test_mod(mod));
}


/**
 *  @brief Runs a test registry
 *  @param[in] reg Test registry
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_run_test_reg(RCU_TEST_REGISTRY *reg){
RCU_TEST_MACHINE *mach;
RCU_CHAR ts_buff[RCU_TSTAMP_BUFF_SIZE];
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_run_test_reg");
#endif
    mach = RCU_GET_CURR_MACH();
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO_P1("Test run started %s",ts_buff);
    rcu_restart_mach(mach);
    RCU_SET_RUN_LEVEL(mach,RCU_RUN_LEVEL_REG);
    reg = (reg == RCU_NULL)?&mach->def_reg:reg;
    if (rcu_srch_reg_by_ref(mach,reg) == RCU_FALSE){
        RCU_SET_ERCD(RCU_E_UNKNOWNREG);
        RCU_LOG_WARN_P2("%s (%s)",RCU_GET_ERR_MSG(),reg->name);
        return(RCU_E_NG);
    }
    RCU_SET_CURR_REG(mach,reg);
    if ((rcu_run_test_reg_impl(mach,reg)) == RCU_E_NG){
        return(RCU_E_NG);
    }
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO_P1("Test run finished %s",ts_buff);
    rcu_stop_mach(mach);
    rcu_gen_test_run_report(mach);
    return(RCU_E_OK);
}

/**
 *  @brief Runs a named test registry
 *  @param[in] name Test registry name
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_run_test_reg_by_name(const RCU_CHAR *name){
RCU_TEST_REGISTRY *reg;
RCU_TEST_MACHINE *mach;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_run_test_reg_by_name");
#endif
    mach = RCU_GET_CURR_MACH();
    if (name == RCU_NULL){
        reg = RCU_GET_DEF_REG();
    }else{
        reg = rcu_srch_reg_by_name(mach,name);
    }
    if (reg == RCU_NULL){
        RCU_SET_ERCD(RCU_E_INVREG);
        RCU_LOG_WARN_P1("%s (null)",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    return(rcu_run_test_reg(reg));
}

/**
 *  @brief Runs the test machine
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_run_test_mach(){
RCU_TEST_MACHINE *mach;
RCU_CHAR ts_buff[RCU_TSTAMP_BUFF_SIZE];
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_run_test_mach");
#endif
    mach = RCU_GET_CURR_MACH();
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO_P1("Test run started %s",ts_buff);
    RCU_SET_RUN_LEVEL(mach,RCU_RUN_LEVEL_MACH);
    rcu_restart_mach(mach);
    rcu_run_test_mach_impl(mach);
    rcu_get_timestamp(ts_buff,RCU_TSTAMP_BUFF_SIZE);
    RCU_LOG_INFO_P1("Test run finished %s",ts_buff);
    rcu_stop_mach(mach);
    rcu_gen_test_run_report(mach);
    return(RCU_E_OK);
}


/**
 *  @brief Initializes the framework
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */
RCU_INT rcu_sys_info(){
    RCU_LOG_INFO("[ RCUNIT SYSTEM INFORMATION ]");
    RCU_LOG_INFO_P3("RCUNIT version is %s (Built on %s %s)",RCU_VERSION_STRING,__DATE__,__TIME__);
#ifdef RCU_DEBUG
    RCU_LOG_INFO("RCUNIT is running in debug mode");
#else
    RCU_LOG_INFO("RCUNIT is not running in debug mode");
#endif

#if RCU_ENABLE_FILE_LOG_HANDLER
    RCU_LOG_INFO("File log handler is enabled");
#else
    RCU_LOG_INFO("File log handler is disabled");
#endif

#if RCU_ENABLE_STDOUT_LOG_HANDLER
    RCU_LOG_INFO("Standard output handler is enabled");
#else
    RCU_LOG_INFO("Standard output handler is disabled");
#endif

#if RCU_ENABLE_TRACE_ALLOC
    RCU_LOG_INFO("Memory allocation tracing is enabled");
#else
    RCU_LOG_INFO("Memory allocation tracing is disabled");
#endif

#if RCU_ENABLE_MTRACE
    RCU_LOG_INFO("Memory leak check facility is enabled");
#else
    RCU_LOG_INFO("Memory leak check facility is disabled");
#endif

#if RCU_ENABLE_MALLOC_ALIGNMENT
    RCU_LOG_INFO_P1("Memory allocation aligned to %lu bytes",RCU_DEFAULT_MALLOC_ALIGNMENT);
#endif
    RCU_LOG_INFO_P1("Native integer size is %lu bytes",sizeof(RCU_INT));
    RCU_LOG_INFO_P1("Void pointer size is %lu bytes",sizeof(RCU_VOID*));
    RCU_LOG_INFO("[ RCUNIT SYSTEM INFORMATION END ]");
    return(RCU_E_OK);
}

/**
 *  @brief Initializes the framework
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_init(){
RCU_TEST_MACHINE *mach;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_init");
#endif
    RCU_LOG_INFO("Initializing framework");
    mach = RCU_GET_CURR_MACH();
    if (rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHINITDONE);
        return(RCU_E_NG);
    }
    if (rcu_init_io_device(&g_io_dev) == RCU_E_NG){
        return(RCU_E_NG);
    }
    if (rcu_init_log() == RCU_E_NG){
        return(RCU_E_NG);
    }
    rcu_sys_info();
    if (rcu_init_test_mach(mach) == RCU_E_NG){
        RCU_SET_ERCD(RCU_E_MACHINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    return(RCU_E_OK);
}

/**
 *  @brief Destroys the framework
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_destroy(){
RCU_TEST_MACHINE *mach;
#if RCU_ENABLE_TRACE_IFACE_ENTRY
    RCU_LOG_INFO("INTERFACE : rcu_destroy");
#endif
    mach = RCU_GET_CURR_MACH();
    RCU_LOG_INFO("Destroying framework");
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    /** Destroy the machine */
    rcu_destroy_test_mach(mach);

    /* Destroy logging facility */
    rcu_destroy_log();

    /** Destroy I/O device */
    rcu_destroy_io_device(&g_io_dev);
    return(RCU_E_OK);
}

/**
 *  @brief Framework exit hook
 *  @param[in] param Additional info
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_exit_hook(RCU_VOID *param){
    RCU_LOG_FATAL("RCUNIT terminating");
    rcu_destroy();
    return(RCU_E_OK);
}

/**
 *  @brief Registers a test run callback function
 *  @param[in] run_hook - Test run callback function entry point
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INTERFACE RCU_INT rcu_set_run_hook(RCU_GENERIC_FUNCTION run_hook){
RCU_TEST_MACHINE *mach;
    mach = RCU_GET_CURR_MACH();
    if (!rcu_is_mach_alive(mach)){
        RCU_SET_ERCD(RCU_E_MACHNOINIT);
        RCU_LOG_WARN_P1("%s",RCU_GET_ERR_MSG());
        return(RCU_E_NG);
    }
    RCU_LOG_INFO("Test run hook set");
    mach->run_hook = run_hook;
    return(RCU_E_OK);
}

/**
 *  @brief Generates timestamp
 *  @param[out] ts_buff - Timestamp buffer. Must be at least RCU_TSTAMP_BUFF_SIZE bytes
 *  @param[in] ts_buff_len - Timestamp buffer length
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_get_timestamp(RCU_CHAR *ts_buff, const RCU_INT ts_buff_len){
time_t t;
RCU_INT len;
    if (ts_buff == RCU_NULL || ts_buff_len < RCU_TSTAMP_BUFF_SIZE){
        RCU_LOG_WARN("Null timestamp buffer or insufficient buffer length");
        return(RCU_E_NG);
    }
    memset(ts_buff,0x00,ts_buff_len);
    time(&t);
    sprintf(ts_buff,"%s",ctime(&t));
    len = strlen(ts_buff);
    memset(&ts_buff[len-1],0x00,1);
    return(RCU_E_OK);
}

/**
 *  @brief RCUNIT print interface
 &  @param[in] str String
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_print(const RCU_CHAR *str){
RCU_TEST_MACHINE *mach;
    mach = RCU_GET_CURR_MACH();
    return(rcu_out_impl(&g_io_dev,(RCU_CHAR*)str));
}

/**
 *  @brief Add an output handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_add_out_hnd(RCU_OUTPUT_HANDLER *out_hnd){
RCU_TEST_MACHINE *mach;
    mach = RCU_GET_CURR_MACH();
    return(rcu_add_dev_out_hnd(&g_io_dev,out_hnd));
}

/**
 *  @brief Add an output handler
 *  @return Operation error code (RCU_E_OK, RCU_E_NG)
 */

RCU_INT rcu_del_out_hnd(RCU_OUTPUT_HANDLER *out_hnd){
RCU_TEST_MACHINE *mach;
    mach = RCU_GET_CURR_MACH();
    return(rcu_del_dev_out_hnd(&g_io_dev,out_hnd));
}





