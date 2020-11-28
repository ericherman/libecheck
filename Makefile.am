# SPDX-License-Identifier: LGPL-3.0-or-later
# Makefile: "E(asy)Check" boiler-plate to make simple testing easier
# Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org>

CSTD_CFLAGS=-std=c89
NOISY_CFLAGS=-Wall -Wextra -pedantic -Werror

eembed_headers=src/eembed.h src/eembed-arduino.h
include_HEADERS=$(eembed_headers) src/echeck.h
lib_LTLIBRARIES=libecheck.la libeembed.la
libecheck_la_SOURCES=src/echeck.h src/echeck.c
libeembed_la_SOURCES=$(eembed_headers) src/eembed.c

if DEBUG
BUILD_TYPE_CFLAGS=-ggdb -O0 \
	-fsanitize=address,leak,undefined \
	-fno-inline-small-functions \
	-fkeep-inline-functions \
	-fkeep-static-functions \
	--coverage
BUILD_TYPE_LDFLAGS=--coverage
else
BUILD_TYPE_CFLAGS=-ggdb -O2 -DNDEBUG -fomit-frame-pointer
BUILD_TYPE_LDFLAGS=
endif

# ./configure --enable-faux-freestanding
if FAUX_FREESTANDING
BUILD_ENV_CFLAGS=-DFAUX_FREESTANDING=1 -DEEMBED_HOSTED=0
else
BUILD_ENV_CFLAGS=
endif

TESTS=$(check_PROGRAMS)
check_PROGRAMS=\
 test_check_status \
 test_check_byte_array \
 test_check_byte_array_m \
 test_check_char \
 test_check_char_m \
 test_check_double \
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
 test_check_unsigned_long_m \
 test_hosted \
 test-eembed-log

TESTS_COMMON_SOURCES=\
 tests/test-echeck-private-utils.h \
 tests/test-echeck-private-utils.c \
 src/eembed.h \
 src/eembed-hosted.h \
 src/echeck.h

T_LDADD=libecheck.la -lecheck libeembed.la -leembed

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

test_check_double_SOURCES=tests/test_check_double.c $(TESTS_COMMON_SOURCES)
test_check_double_LDADD=$(T_LDADD)

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

test_hosted_SOURCES=tests/test_hosted.c $(TESTS_COMMON_SOURCES)
test_hosted_CFLAGS=-std=c11 -pipe -Isrc -Itests \
		   -Wall -Wextra -Wpedantic -Werror \
		   $(BUILD_TYPE_CFLAGS)
test_hosted_LDADD=$(T_LDADD)

test_eembed_log_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-log.c
test_eembed_log_LDADD=$(T_LDADD)

EXTRA_DIST=COPYING.LESSER

ACLOCAL_AMFLAGS=-I m4 --install

AM_CFLAGS=\
 $(CSTD_CFLAGS) \
 $(BUILD_ENV_CFLAGS) \
 $(BUILD_TYPE_CFLAGS) \
 $(NOISY_CFLAGS) \
 -I./src \
 -fPIC \
 -pipe
AM_LDFLAGS=$(BUILD_TYPE_LDFLAGS)

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

tidy:
	$(LINDENT) \
		-T FILE \
		-T size_t \
		-T eembed_log \
		-T eembed_str_buf \
		-T eembed_allocator \
		`find src tests -name '*.h' -o -name '*.c' -o -name '*.cpp'` \
		echeck_tests_arduino/echeck_tests_arduino.ino \
		echeck_arduino_demo/echeck_arduino_demo.ino

vg-test_check_status: test_check_status
	./libtool --mode=execute valgrind -q ./test_check_status

vg-test_check_byte_array: test_check_byte_array
	./libtool --mode=execute valgrind -q ./test_check_byte_array

vg-test_check_byte_array_m: test_check_byte_array_m
	./libtool --mode=execute valgrind -q ./test_check_byte_array_m

vg-test_check_char: test_check_char
	./libtool --mode=execute valgrind -q ./test_check_char

vg-test_check_char_m: test_check_char_m
	./libtool --mode=execute valgrind -q ./test_check_char_m

vg-test_check_double: test_check_double
	./libtool --mode=execute valgrind -q ./test_check_double

vg-test_check_int: test_check_int
	./libtool --mode=execute valgrind -q ./test_check_int

vg-test_check_int_m: test_check_int_m
	./libtool --mode=execute valgrind -q ./test_check_int_m

vg-test_check_long: test_check_long
	./libtool --mode=execute valgrind -q ./test_check_long

vg-test_check_long_m: test_check_long_m
	./libtool --mode=execute valgrind -q ./test_check_long_m

vg-test_check_ptr: test_check_ptr
	./libtool --mode=execute valgrind -q ./test_check_ptr

vg-test_check_ptr_m: test_check_ptr_m
	./libtool --mode=execute valgrind -q ./test_check_ptr_m

vg-test_check_size_t: test_check_size_t
	./libtool --mode=execute valgrind -q ./test_check_size_t

vg-test_check_size_t_m: test_check_size_t_m
	./libtool --mode=execute valgrind -q ./test_check_size_t_m

vg-test_check_str: test_check_str
	./libtool --mode=execute valgrind -q ./test_check_str

vg-test_check_str_null_ptr: test_check_str_null_ptr
	./libtool --mode=execute valgrind -q ./test_check_str_null_ptr

vg-test_check_str_m: test_check_str_m
	./libtool --mode=execute valgrind -q ./test_check_str_m

vg-test_check_unsigned_int: test_check_unsigned_int
	./libtool --mode=execute valgrind -q ./test_check_unsigned_int

vg-test_check_unsigned_int_m: test_check_unsigned_int_m
	./libtool --mode=execute valgrind -q ./test_check_unsigned_int_m

vg-test_check_unsigned_long: test_check_unsigned_long
	./libtool --mode=execute valgrind -q ./test_check_unsigned_long

vg-test_check_unsigned_long_m: test_check_unsigned_long_m
	./libtool --mode=execute valgrind -q ./test_check_unsigned_long_m

vg-test_hosted: test_hosted
	./libtool --mode=execute valgrind -q ./test_hosted

valgrind: \
	vg-test_check_byte_array \
	vg-test_check_byte_array_m \
	vg-test_check_char \
	vg-test_check_char_m \
	vg-test_check_double \
	vg-test_check_int \
	vg-test_check_int_m \
	vg-test_check_long \
	vg-test_check_long_m \
	vg-test_check_ptr \
	vg-test_check_ptr_m \
	vg-test_check_size_t \
	vg-test_check_size_t_m \
	vg-test_check_status \
	vg-test_check_str \
	vg-test_check_str_m \
	vg-test_check_str_null_ptr \
	vg-test_check_unsigned_int \
	vg-test_check_unsigned_int_m \
	vg-test_check_unsigned_long \
	vg-test_check_unsigned_long_m \
	vg-test_hosted
	echo "valgrind complete"


spotless:
	rm -rf `cat .gitignore | sed -e 's/#.*//'`
	pushd src && rm -rf `cat ../.gitignore | sed -e 's/#.*//'`; popd
	pushd tests && rm -rf `cat ../.gitignore | sed -e 's/#.*//'`; popd

mrproper:
	git clean -dffx
	git submodule foreach --recursive git clean -dffx
