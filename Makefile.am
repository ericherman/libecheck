lib_LTLIBRARIES=libecheck.la
libecheck_la_SOURCES=src/echeck.c src/echeck.h

include_HEADERS=src/echeck.h

TESTS=$(check_PROGRAMS)
check_PROGRAMS=test-echeck
test_echeck_SOURCES=tests/test-echeck.c src/echeck.h
test_echeck_LDADD=libecheck.la

ACLOCAL_AMFLAGS=-I m4 --install

AM_CFLAGS=-std=c89 -O0 -ggdb -Wall -Wextra -Wpedantic -Werror

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

tidy:
	$(LINDENT) \
		-T FILE \
		-T size_t \
		`find src tests -name '*.h' -o -name '*.c'`

valgrind: test-echeck
	libtool --mode=execute valgrind ./test-echeck

spotless:
	rm -rf `cat .gitignore`
	( cd src; rm -rf `cat ../.gitignore`; cd .. )
	( cd tests; rm -rf `cat ../.gitignore`; cd .. )
