dnl Process this file with the command: autoreconf -f -i

m4_define([EFL_A11Y_VERSION], [1.0.0])

AC_INIT([efl-a11y], [EFL_A11Y_VERSION])

AC_PREREQ([2.52])
AC_CONFIG_SRCDIR([configure.ac])
AC_CANONICAL_HOST

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.6 dist-bzip2])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST

AC_LIBTOOL_WIN32_DLL
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

LT_RELEASE_VERSION=EFL_A11Y_VERSION
AC_SUBST([LT_RELEASE_VERSION])

LT_ENABLE_AUTO_IMPORT=""
case "$host_os" in
   mingw*)
      LT_ENABLE_AUTO_IMPORT="-Wl,--enable-auto-import"
esac
AC_SUBST([LT_ENABLE_AUTO_IMPORT])

REQUIREMENT_EAIL="elementary >= 1.7.0 ecore-evas >= 1.7.0 ecore-file >= 1.7.0 ecore >= 1.7.0 evas >= 1.7.0 eina >= 1.7.0 atk >= 2.2.0 gobject-2.0 >= 2.30.0 gmodule-2.0 >= 2.30.0 gio-2.0"
AC_SUBST([REQUIREMENT_EAIL])

PKG_CHECK_MODULES([EAIL], [
    eina >= 1.7.0
    evas >= 1.7.0
    ecore >= 1.7.0
    ecore-file >= 1.7.0
    ecore-evas >= 1.7.0
    elementary >= 1.7.0
    gmodule-2.0 >= 2.30.0
    gobject-2.0 >= 2.30.0
    atk >= 2.2.0
    atk-bridge-2.0 >= 2.2.0
    gio-2.0
])
PKG_CHECK_MODULES([TEST_DEPS], [
                   elementary >= 1.7.0
                   atk >= 2.2.0])

dnl Strict compiler
AC_ARG_ENABLE(strict-cc,
        AS_HELP_STRING([--enable-strict-cc],[enable strict C compiler]),,
        enable_strict_cc=no)
if test "x$enable_strict_cc" = "xyes"; then
        CFLAGS="$CFLAGS -Wall -Werror"
else
        CFLAGS="$CFLAGS -Wall"
fi

dnl doc
AC_ARG_ENABLE([doc],
              AS_HELP_STRING([--enable-doc],
                             [Enable documentation [default=YES]]),,
                             enable_doc=yes)
AM_CONDITIONAL([BUILD_DOC], [test "x$enable_doc" = "xyes"])

dnl Tests
AC_ARG_ENABLE([tests],
              AS_HELP_STRING([--enable-tests],[build tests]),,
              enable_tests=no)
AM_CONDITIONAL([MAKE_TESTS], [test "x$enable_tests" = "xyes"])

dnl Generate coverage report
AC_ARG_ENABLE([coverage],
  AS_HELP_STRING([--enable-coverage],
                 [Generate converage reports]),
  [],
  [enable_coverage=no])
AM_CONDITIONAL([ENABLE_COVERAGE],[test "x$enable_coverage" != "xno"])
AS_IF([test "x$enable_coverage" != "xno"],
  [
    AC_PATH_PROG([LCOV], [lcov])
    AC_PATH_PROG([GENHTML], [genhtml])
    AC_SUBST([COVERAGE_LIBS],["-lgcov"])
    AC_SUBST([COVERAGE_CFLAGS],["-fprofile-arcs -ftest-coverage"])
  ]
)
dnl set elementatary modules dir
AC_ARG_WITH(elementary-modules,
           [AS_HELP_STRING([--with-elementary-modules=<directory>],
           [where elementary module directory is])])
AM_CONDITIONAL(ENABLE_ELM_MODULE, false )
if ! test -z "$with_elementary_modules" ; then
           ELEMENTARY_MODULE_DIR="$with_elementary_modules"
           AC_SUBST(ELEMENTARY_MODULE_DIR)
           AM_CONDITIONAL(ENABLE_ELM_MODULE, true )
fi

AC_OUTPUT([
    eail.pc
    Makefile
    eail/Makefile
    tests/Makefile
    doc/Makefile
])
