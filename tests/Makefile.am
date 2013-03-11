## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = rcunit_test 

rcunit_test_SOURCES = rcunit_test.c testmoko.c testmoko_exception.c

noinst_HEADERS = \
    rcunit_test.h \
    rcunit_test_assertions.c \
    rcunit_test_exceptions.c \
    rcunit_test_framework.c \
    rcunit_test_function.c \
    rcunit_test_module.c \
    rcunit_test_mtrace.c \
    rcunit_test_registry.c \
    testmoko.h \
    testmoko_exception.h
    
LDADD = ../src/librcunit.a
AM_CPPFLAGS =  -I $(top_srcdir)/src  -I $(srcdir)

check : rcunit_test 
	./rcunit_test

clean-local:
	rm -f *.log
	rm  -f rcunit_test_run_report.*


