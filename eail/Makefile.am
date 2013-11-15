AUTOMAKE_OPTIONS = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.h.in configure depcomp install-sh missing
ACLOCAL_AMFLAGS= -I m4 ${ACLOCAL_FLAGS}
INCLUDES = -I$(top_srcdir)

if ENABLE_EXPLORER
EXTRA = explorer
else
EXTRA =
endif

SUBDIRS = . eail tests doc $(EXTRA)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = eail.pc
