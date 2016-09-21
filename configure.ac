# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([CMOH], [0.2], [cmoh@nut.email])
AC_CONFIG_SRCDIR([include/cmoh/accessor_bundle.hpp])
AM_INIT_AUTOMAKE([foreign subdir-objects])

# Checks for programs.
AC_PROG_CXX
AC_CHECK_TOOL([PANDOC], [pandoc])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AX_CXX_COMPILE_STDCXX(14, [noext])
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

AC_CONFIG_FILES([
    Makefile
])
AC_OUTPUT
