#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.53)
AC_INIT([rcunit],[1.0],[jecklgamis@gmail.com])
AC_CANONICAL_HOST
AC_PROG_MAKE_SET
AC_CONFIG_SRCDIR([src/rcunit.c])
AM_INIT_AUTOMAKE([rcunit],[1.0])

# Configuration header
AC_CONFIG_HEADER(src/rcunit_config_env.h)

AC_ARG_ENABLE(debug,[  --enable-debug    Turn on debugging],
	[case "${enableval}" in
       yes) debug=true ;;
       no)  debug=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
    esac],[debug=false])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_PROG_RANLIB

AC_SEARCH_LIBS([lmk_get_logger],[logmoko],,[AC_MSG_ERROR([Unable to find logmoko library])])

# Checks for header files.
AC_HEADER_STDC

AC_CHECK_HEADERS([stdio.h stdlib.h string.h setjmp.h time.h logmoko.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_C_STRINGIZE
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(void*)

# Checks for library functions.
AC_FUNC_MALLOC

# Checks for functions
AC_CHECK_FUNCS([malloc free memset memcpy strcpy strcmp strlen setjmp longjmp sprintf fprintf ctime time])

# Configuration files
AC_CONFIG_FILES([Makefile src/Makefile samples/Makefile tests/Makefile])

AC_OUTPUT

