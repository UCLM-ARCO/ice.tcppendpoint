#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.60])

AC_INIT(TCP Ice Endpoint, 1.0.0, tobias.deb@gmail.com)
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/TCPPEndpointI.cpp])
AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS(config.h)

VERSION=1.0.0
PACKAGE=IceTCPP

GENERIC_LIBRARY_NAME=$PACKAGE
GENERIC_LIBRARY_VERSION=33:0:0
AC_SUBST(GENERIC_LIBRARY_NAME)
AC_SUBST(GENERIC_LIBRARY_VERSION)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_DISABLE_STATIC
AM_PROG_AS
AM_PROG_LIBTOOL
AM_SANITY_CHECK

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([Ice], [IceUtil])
#AM_LDFLAGS=-module -export-dynamic -rpath /usr/local/lib
# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([select])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_LANG_CPLUSPLUS
AC_OUTPUT
