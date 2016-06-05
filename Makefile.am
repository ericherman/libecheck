lib_LTLIBRARIES=libecheck.la
libecheck_la_SOURCES=src/echeck.c src/echeck.h

include_HEADERS=src/echeck.h

TESTS=$(check_PROGRAMS)
check_PROGRAMS=\
 test_check_status \
 test_check_byte_array \
 test_check_byte_array_m \
 test_check_char \
 test_check_char_m \
 test_check_int \
 test_check_int_m \
 test_check_long \
 test_check_long_m \
 test_check_ptr \
 test_check_ptr_m \
 test_check_size_t \
 test_check_size_t_m \
 test_check_str \
 test_check_str_null_ptr \
 test_check_str_m \
 test_check_unsigned_int \
 test_check_unsigned_int_m \
 test_check_unsigned_long \
 test_check_unsigned_long_m

TESTS_COMMON_SOURCES=\
 tests/test-echeck-private-utils.h \
 tests/test-echeck-private-utils.c \
 src/echeck.h

T_LDADD=libecheck.la -lecheck

test_check_status_SOURCES=tests/test_check_status.c \
 $(TESTS_COMMON_SOURCES)
test_check_status_LDADD=$(T_LDADD)

test_check_byte_array_SOURCES=tests/test_check_byte_array.c \
 $(TESTS_COMMON_SOURCES)
test_check_byte_array_LDADD=$(T_LDADD)

test_check_byte_array_m_SOURCES=tests/test_check_byte_array_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_byte_array_m_LDADD=$(T_LDADD)

test_check_char_SOURCES=tests/test_check_char.c \
 $(TESTS_COMMON_SOURCES)
test_check_char_LDADD=$(T_LDADD)

test_check_char_m_SOURCES=tests/test_check_char_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_char_m_LDADD=$(T_LDADD)

test_check_int_SOURCES=tests/test_check_int.c \
 $(TESTS_COMMON_SOURCES)
test_check_int_LDADD=$(T_LDADD)

test_check_int_m_SOURCES=tests/test_check_int_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_int_m_LDADD=$(T_LDADD)

test_check_long_SOURCES=tests/test_check_long.c \
 $(TESTS_COMMON_SOURCES)
test_check_long_LDADD=$(T_LDADD)

test_check_long_m_SOURCES=tests/test_check_long_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_long_m_LDADD=$(T_LDADD)

test_check_ptr_SOURCES=tests/test_check_ptr.c \
 $(TESTS_COMMON_SOURCES)
test_check_ptr_LDADD=$(T_LDADD)

test_check_ptr_m_SOURCES=tests/test_check_ptr_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_ptr_m_LDADD=$(T_LDADD)

test_check_size_t_SOURCES=tests/test_check_size_t.c \
 $(TESTS_COMMON_SOURCES)
test_check_size_t_LDADD=$(T_LDADD)

test_check_size_t_m_SOURCES=tests/test_check_size_t_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_size_t_m_LDADD=$(T_LDADD)

test_check_str_SOURCES=tests/test_check_str.c \
 $(TESTS_COMMON_SOURCES)
test_check_str_LDADD=$(T_LDADD)

test_check_str_null_ptr_SOURCES=tests/test_check_str_null_ptr.c \
 $(TESTS_COMMON_SOURCES)
test_check_str_null_ptr_LDADD=$(T_LDADD)

test_check_str_m_SOURCES=tests/test_check_str_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_str_m_LDADD=$(T_LDADD)

test_check_unsigned_int_SOURCES=tests/test_check_unsigned_int.c \
 $(TESTS_COMMON_SOURCES)
test_check_unsigned_int_LDADD=$(T_LDADD)

test_check_unsigned_int_m_SOURCES=tests/test_check_unsigned_int_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_unsigned_int_m_LDADD=$(T_LDADD)

test_check_unsigned_long_SOURCES=tests/test_check_unsigned_long.c \
 $(TESTS_COMMON_SOURCES)
test_check_unsigned_long_LDADD=$(T_LDADD)

test_check_unsigned_long_m_SOURCES=tests/test_check_unsigned_long_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_unsigned_long_m_LDADD=$(T_LDADD)


ACLOCAL_AMFLAGS=-I m4 --install

AM_CFLAGS=-std=c89 -O0 -ggdb -Wall -Wextra -Wpedantic -Werror

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

tidy:
	$(LINDENT) \
		-T FILE \
		-T size_t \
		`find src tests -name '*.h' -o -name '*.c'`

valgrind: $(check_PROGRAMS)
	libtool --mode=execute valgrind ./test_check_status
	libtool --mode=execute valgrind ./test_check_byte_array
	libtool --mode=execute valgrind ./test_check_byte_array_m
	libtool --mode=execute valgrind ./test_check_char
	libtool --mode=execute valgrind ./test_check_char_m
	libtool --mode=execute valgrind ./test_check_int
	libtool --mode=execute valgrind ./test_check_int_m
	libtool --mode=execute valgrind ./test_check_long
	libtool --mode=execute valgrind ./test_check_long_m
	libtool --mode=execute valgrind ./test_check_ptr
	libtool --mode=execute valgrind ./test_check_ptr_m
	libtool --mode=execute valgrind ./test_check_size_t
	libtool --mode=execute valgrind ./test_check_size_t_m
	libtool --mode=execute valgrind ./test_check_str
	libtool --mode=execute valgrind ./test_check_str_null_ptr
	libtool --mode=execute valgrind ./test_check_str_m
	libtool --mode=execute valgrind ./test_check_unsigned_int
	libtool --mode=execute valgrind ./test_check_unsigned_int_m
	libtool --mode=execute valgrind ./test_check_unsigned_long
	libtool --mode=execute valgrind ./test_check_unsigned_long_m

spotless:
	rm -rf `cat .gitignore | sed -e 's/#.*//'`
	pushd src && rm -rf `cat ../.gitignore | sed -e 's/#.*//'`; popd
	pushd tests && rm -rf `cat ../.gitignore | sed -e 's/#.*//'`; popd
