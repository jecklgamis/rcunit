## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = rcunit_test 

rcunit_test_SOURCES = rcunit_test.c testmoko.c testmoko_exception.c

noinst_HEADERS = \
    rcunit_test.c \
    rcunit_test_assertions.c \
    rcunit_test_exceptions.c \
    rcunit_test_framework.c \
    rcunit_test_function.c \
    rcunit_test_module.c \
    rcunit_test_mtrace.c \
    rcunit_test_registry.c
    
LDADD =  $(top_srcdir)/src/librcunit.a

if DEBUG
else
endif

