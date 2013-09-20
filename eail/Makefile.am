AUTOMAKE_OPTIONS = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.h.in configure depcomp install-sh missing
ACLOCAL_AMFLAGS= -I m4 ${ACLOCAL_FLAGS}
INCLUDES = -I$(top_srcdir)

SUBDIRS = . eail tests doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = eail.pc
