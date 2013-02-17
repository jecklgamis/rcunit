## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = rcunit_sample_app_simple \
                  rcunit_sample_app_module \
                  rcunit_sample_app_function_table \
                  rcunit_sample_app_exception \
                  rcunit_sample_app_signal \
                  rcunit_sample_app_run_hook \
                  rcunit_sample_app_demo

rcunit_sample_app_simple_SOURCES = rcunit_sample_app_simple.c
rcunit_sample_app_module_SOURCES = rcunit_sample_app_module.c
rcunit_sample_app_function_table_SOURCES = rcunit_sample_app_function_table.c
rcunit_sample_app_exception_SOURCES = rcunit_sample_app_exception.c
rcunit_sample_app_signal_SOURCES = rcunit_sample_app_signal.c
rcunit_sample_app_run_hook_SOURCES = rcunit_sample_app_run_hook.c
rcunit_sample_app_demo_SOURCES = rcunit_sample_app_demo.c

LDADD = ../src/librcunit.a

AM_CPPFLAGS =  -I $(top_srcdir)/src  -I $(srcdir)
