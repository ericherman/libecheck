lib_LTLIBRARIES=libecheck.la
libecheck_la_SOURCES=echeck.c

include_HEADERS=echeck.h

TESTS=$(check_PROGRAMS)
check_PROGRAMS=test-echeck
test-echeck_SOURCES=test-echeck.c
test-echeck_LDADD=libecheck.la

AM_CFLAGS=-std=c89 -O0 -ggdb -Wall -Wextra -Wpedantic -Werror

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

tidy:
	$(LINDENT) \
		-T FILE \
		-T size_t \
		*.h *.c
