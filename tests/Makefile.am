## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = rcunit_test 

rcunit_test_SOURCES = rcunit_test.c testmoko.c testmoko_exception.c

noinst_HEADERS = \
    rcunit_test.h \
    rcunit_assertion_tests.c \
    rcunit_exception_tests.c \
    rcunit_framework_tests.c \
    rcunit_function_tests.c \
    rcunit_module_tests.c \
    rcunit_mtrace_tests.c \
    rcunit_registry_tests.c \
    testmoko.h \
    testmoko_exception.h
    
LDADD = ../src/librcunit.a
AM_CPPFLAGS =  -I $(top_srcdir)/src  -I $(srcdir)

check : rcunit_test 
	./rcunit_test

clean-local:
	rm -f *.log
	rm  -f rcunit_test_run_report.*


