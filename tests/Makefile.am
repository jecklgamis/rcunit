## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = rcunit_test 

rcunit_test_SOURCES = rcunit_test.c rcunit_test.h

noinst_HEADERS = \
    rcunit_test_analysis.c \
    rcunit_test_assertions.c \
    rcunit_test.c \
    rcunit_test_exceptions.c \
    rcunit_test_framework.c \
    rcunit_test_function.c \
    rcunit_test_logging.c \
    rcunit_test_module.c \
    rcunit_test_mtrace.c \
    rcunit_test_registry.c \
    rcunit_test_report.c \
    rcunit_test_run.c \
    rcunit_test_random_emelgeek.c \
    rcunit_test_redirect.c 
    
LDADD = ../src/librcunit.a

DEPENDENCIES =

AM_CPPFLAGS =  -I $(top_srcdir)/src  -I $(srcdir)

