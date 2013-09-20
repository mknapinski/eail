MAINTAINERCLEANFILES = Makefile.in

module_LTLIBRARIES = module.la

module_la_SOURCES  = \
	eail.c \
	eail.h \
	eail_app.c \
	eail_app.h \
	eail_mod.c

moduledir = $(libdir)/elementary/modules/eail/$(MODULE_ARCH)

module_la_CFLAGS = @EAIL_CFLAGS@
module_la_LIBADD = @EAIL_LIBS@
module_la_LDFLAGS = -no-undefined @LT_ENABLE_AUTO_IMPORT@ -module -avoid-version
module_la_LIBTOOLFLAGS = --tag=disable-static
