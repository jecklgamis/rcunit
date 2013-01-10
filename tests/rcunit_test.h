#ifndef RCUNIT_TEST_H
#define RCUNIT_TEST_H

typedef RCU_INT (*FUNC_ENTRY_FORMAT_1)(RCU_INT,RCU_CHAR **);

#define RCU_DEF_EMPTY_TEST_FUNC(fname) \
    RCU_DEF_TEST_FUNC(fname,param){ \
    }

typedef struct local_test_entry{
const RCU_CHAR *name;
FUNC_ENTRY_FORMAT_1 entry;
RCU_INT enable;
}LOCAL_TEST_ENTRY;

#define DEF_LOCAL_TEST_FUNC(name) \
    RCU_INT name(RCU_INT argc,RCU_CHAR **argv)

#define DEF_LOCAL_TEST_TABLE_START(name) \
    LOCAL_TEST_ENTRY name[]={

#define INC_LOCAL_TEST_FUNC(func,enable) \
    {#func,func,enable},

#define DEF_LOCAL_TEST_TABLE_END \
    {RCU_NULL,RCU_NULL}                  \
    };

#define LOCAL_ASSERT(cond) \
  do { local_assert_impl((cond), RCU_FALSE,#cond,"",__FILE__, __LINE__); }while(0);

#define LOCAL_ASSERT_FATAL(cond) \
  do { local_assert_impl((cond), RCU_TRUE,#cond,"",__FILE__, __LINE__); }while(0);

#define LOCAL_ASSERT_NOT_NULL(cond) \
  do { local_assert_impl(((cond)!=RCU_NULL), RCU_FALSE,#cond,"",__FILE__, __LINE__); }while(0);

#define LOCAL_ASSERT_NOT_NULL_FATAL(cond) \
  do { local_assert_impl(((cond)!=RCU_NULL), RCU_TRUE,#cond,"",__FILE__, __LINE__); }while(0);

#define LOCAL_ASSERT_NULL(cond) \
  do { local_assert_impl(((cond)==RCU_NULL), RCU_FALSE,#cond,"",__FILE__, __LINE__); }while(0);

#define LOCAL_ASSERT_NULL_FATAL(cond) \
  do { local_assert_impl(((cond)==RCU_NULL), RCU_TRUE,#cond,"",__FILE__, __LINE__); }while(0);


#endif /* RCUNIT_TEST_H */



