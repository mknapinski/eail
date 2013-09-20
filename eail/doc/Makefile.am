MAINTAINERCLEANFILES = Makefile.in

if BUILD_DOC
clean:
	rm -rf html/
	rm -rf latex/

doc: all
	doxygen Doxyfile

clean-local: clean

else
doc:
	@echo "Documentation not built. Run ./configure --help"

endif

.PHONY: all doc clean
