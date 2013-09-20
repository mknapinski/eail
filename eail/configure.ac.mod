dnl Process this file with the command: autoreconf -f -i

AC_INIT([efl-a11y], [0.1.0])

AC_PREREQ([2.52])
AC_CONFIG_SRCDIR([configure.ac])
AC_CANONICAL_HOST

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.6 dist-bzip2])

AC_PROG_CC
AC_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST

AC_LIBTOOL_WIN32_DLL
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

LT_ENABLE_AUTO_IMPORT=""
case "$host_os" in
   mingw*)
      LT_ENABLE_AUTO_IMPORT="-Wl,--enable-auto-import"
esac
AC_SUBST([LT_ENABLE_AUTO_IMPORT])

PKG_PROG_PKG_CONFIG
elm_version=`$PKG_CONFIG --modversion elementary | cut -d '.' -f '1-3'`
MODULE_ARCH="$host_os-$host_cpu-$elm_version"
AC_SUBST([MODULE_ARCH])

PKG_CHECK_MODULES([EAIL], [
    eina >= 1.1.0
    evas >= 1.1.0
    ecore >= 1.1.0
    ecore-evas >= 1.1.0
    ecore-file >= 1.1.0
    elementary >= 0.8.0
    gobject-2.0 >= 2.30.0
    gmodule-2.0 >= 2.30.0
    atk >= 2.2.0
])

AC_OUTPUT([
    Makefile
    eail/Makefile
])
