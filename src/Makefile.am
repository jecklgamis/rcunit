## Process this file with automake to produce Makefile.in

lib_LIBRARIES = librcunit.a

librcunit_a_SOURCES = \
    rcunit.c \
    rcunit_error.c \
    rcunit_function.c \
    rcunit_module.c \
    rcunit_registry.c \
    rcunit_machine.c \
    rcunit_exception.c \
    rcunit_list.c \
    rcunit_random.c \
    rcunit_hashtable.c \
    rcunit_mem.c  \
    rcunit_queue.c \
    rcunit_assert.c \
    rcunit_mtrace.c \
    rcunit_report.c \
    rcunit_report_html.c \
    rcunit_report_plaintext.c \
    rcunit_report_xml.c \
    rcunit_report_json.c \
    rcunit_util.c \
    rcunit_thread.c

include_HEADERS =  \
	rcunit.h \
	rcunit_api.h \
	rcunit_assert.h \
	rcunit_config.h \
	rcunit_config_env.h \
	rcunit_error.h \
	rcunit_exception.h \
	rcunit_hashtable.h \
	rcunit_helpers.h \
	rcunit_list.h \
	rcunit_mem.h \
	rcunit_mtrace.h \
	rcunit_queue.h \
	rcunit_random.h \
	rcunit_report.h \
	rcunit_thread.h \
	rcunit_types.h

if DEBUG
AM_CPPFLAGS  = 
else
AM_CPPFLAGS  =
endif
