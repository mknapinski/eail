MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = bin

lib_LTLIBRARIES = libeailexplorer.la

AM_CPPFLAGS = -DDATA_PATH=\"$(datadir)/\eail-explorer\"

libeailexplorer_la_SOURCES = \
	eail-explorer.c \
	eail-explorer-ui.c \
	eail-browser.c

libeailexplorer_la_LIBADD = @EAILEXPLORER_LIBS@
libeailexplorer_la_CFLAGS = @EAILEXPLORER_CFLAGS@
