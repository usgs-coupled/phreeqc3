#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([phreeqc], [3.0.0-7109], [dlpark@usgs.gov])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/class_main.cpp])

AC_DEFINE(NDEBUG)

# Checks for programs.
AC_PROG_CXX
###AC_PROG_AWK
###AC_PROG_CC
###AC_PROG_CPP
###AC_PROG_INSTALL
###AC_PROG_LN_S
###AC_PROG_MAKE_SET

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([float.h limits.h memory.h stddef.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD

# libc functions
AC_CHECK_FUNCS([memmove], , AC_ERROR([memmove not found in libc]))
AC_CHECK_FUNCS([memset],  , AC_ERROR([memset not found in libc]))
AC_CHECK_FUNCS([strchr],  , AC_ERROR([strchr not found in libc]))
AC_CHECK_FUNCS([strcspn], , AC_ERROR([strcspn not found in libc]))
AC_CHECK_FUNCS([strtol],  , AC_ERROR([strtol not found in libc]))

# libm functions
AC_CHECK_FUNCS([floor],   , AC_CHECK_LIB(m, floor, , AC_ERROR([cannot find floor])) )
AC_CHECK_FUNCS([pow],     , AC_CHECK_LIB(m, pow,   , AC_ERROR([cannot find pow])) )
AC_CHECK_FUNCS([sqrt],    , AC_CHECK_LIB(m, sqrt,  , AC_ERROR([cannot find sqrt])) )

# isfinite
AC_LANG_PUSH(C++)
AC_CACHE_CHECK([for isfinite], ac_cv_isfinite,
 [AC_TRY_LINK([#include <math.h>],
 [double x; int y; y = isfinite(x);],
 ac_cv_isfinite=yes,
 ac_cv_isfinite=no
)])
if test x"$ac_cv_isfinite" = x"yes"; then
  AC_DEFINE(HAVE_ISFINITE, [], [Has isfinite])
fi
AC_LANG_POP(C++)

# finite
AC_LANG_PUSH(C++)
AC_CACHE_CHECK([for finite], ac_cv_finite,
 [AC_TRY_LINK([#include <math.h>],
 [double x; int y; y = finite(x);],
 ac_cv_finite=yes,
 ac_cv_finite=no
)])
if test x"$ac_cv_finite" = x"yes"; then
  AC_DEFINE(HAVE_FINITE, [], [Has finite])
fi
AC_LANG_POP(C++)

# isnan
AC_CHECK_FUNCS([isnan],    , AC_CHECK_LIB(m, isnan))

# check for gmp
AC_ARG_WITH([gmp],
  [AS_HELP_STRING([--with-gmp],
    [support GNU Multiple Precision library @<:@default=check@:>@])],
  [],
  [with_gmp=check])

# check for gmp header
AS_IF([test "x$with_gmp" != xno],
  [
    AC_CHECK_HEADER([gmp.h],
    [AC_DEFINE([INVERSE_CL1MP], [1], [Define if you have gmp])],
    [if test "x$with_gmp" != xcheck; then
       AC_MSG_FAILURE([--with-gmp was given, but gmp.h not found])
     fi
    ])
  ]
)

# check for gmp lib
LIBGMP=
AS_IF([test "x$with_gmp" != xno],
  [
    AC_CHECK_LIB([gmp], [__gmpz_init],
    [AC_SUBST([LIBGMP], ["-lgmp"])],
    [if test "x$with_gmp" != xcheck; then
       AC_MSG_FAILURE([--with-gmp was given, but test for gmp failed])
     fi],
    -lgmp)
  ]
)

AM_CONDITIONAL([BUILD_GMP], [test "X$with_gmp" = "Xyes"])

AC_CONFIG_FILES([
  Makefile
  database/Makefile
  doc/Makefile
  examples/Makefile
  src/Makefile
])
AC_OUTPUT
