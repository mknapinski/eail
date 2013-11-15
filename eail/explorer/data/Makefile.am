MAINTAINERCLEANFILES = Makefile.in

EDJE_CC = @EDJE_CC@

layoutdir = $(datadir)/eail-explorer/
layout_DATA = layout.edj


EXTRA_DIST = \
	layout.edc \
	layout_main.edc \
	layout_action.edc \
	layout_component.edc \
	layout_image.edc \
	layout_object.edc \
	layout_selection.edc \
	layout_table.edc \
	layout_value.edc

layout.edj: Makefile $(EXTRA_DIST)
	$(EDJE_CC) \
	$(top_srcdir)/explorer/data/layout.edc \
	$(top_builddir)/explorer/data/layout.edj

clean-local:
	rm -f *.edj
