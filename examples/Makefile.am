## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = rcunit_example_app_simple \
                  rcunit_example_app_module \
                  rcunit_example_app_function_table \
                  rcunit_example_app_exception \
                  rcunit_example_app_signal \
                  rcunit_example_app_run_hook \
                  rcunit_example_app_demo

rcunit_example_app_simple_SOURCES = rcunit_example_app_simple.c
rcunit_example_app_module_SOURCES = rcunit_example_app_module.c
rcunit_example_app_function_table_SOURCES = rcunit_example_app_function_table.c
rcunit_example_app_exception_SOURCES = rcunit_example_app_exception.c
rcunit_example_app_signal_SOURCES = rcunit_example_app_signal.c
rcunit_example_app_run_hook_SOURCES = rcunit_example_app_run_hook.c
rcunit_example_app_demo_SOURCES = rcunit_example_app_demo.c

LDADD = ../src/librcunit.a

AM_CPPFLAGS =  -I $(top_srcdir)/src  -I $(srcdir)

clean-local:
	rm -f *.log
	rm  -f rcunit_test_run_report.*
