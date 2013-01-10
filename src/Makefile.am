## Process this file with automake to produce Makefile.in

lib_LIBRARIES = librcunit.a

librcunit_a_SOURCES = rcunit.c \
    rcunit_error.c \
    rcunit_exception.c \
    rcunit_list.c \
    rcunit_log.c \
    rcunit_log_hnd_stdout.c \
    rcunit_log_hnd_file.c \
    rcunit_mtrace.c \
    rcunit_report.c \
    rcunit_mem.c \
    rcunit_random.c \
    rcunit_hashtable.c \
    rcunit_io_device.c

include_HEADERS =  \
    rcunit_config_env.h  \
    rcunit_config.h \
    rcunit_types.h  \
    rcunit_helpers.h \
    rcunit_assert.h \
    rcunit.h  \
    rcunit_error.h \
    rcunit_exception.h \
    rcunit_list.h \
    rcunit_log.h \
    rcunit_mtrace.h \
    rcunit_report.h \
    rcunit_mem.h \
    rcunit_random.h \
    rcunit_hashtable.h \
    rcunit_io_device.h

if DEBUG
AM_CPPFLAGS  =  -DRCU_DEBUG=1
else
AM_CPPFLAGS  =
endif
