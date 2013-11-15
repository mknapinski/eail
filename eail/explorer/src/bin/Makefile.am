bin_SCRIPTS = eail-explorer.sh
CLEANFILES = $(bin_SCRIPTS)

eail-explorer.sh: eail-explorer.sh.in
	$(SED) \
            -e 's|[@]libdir@|@libdir@|g' \
            < "$<" > "$@"
