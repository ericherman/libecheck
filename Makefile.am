# SPDX-License-Identifier: LGPL-3.0-or-later
# Makefile: "E(asy)Check" boiler-plate to make simple testing easier
# Copyright (C) 2016, 2017, 2018, 2019, 2020 Eric Herman <eric@freesa.org>

BROWSER=firefox

CSTD_CFLAGS=-std=c89
NOISY_CFLAGS=-Wall -Wextra -pedantic -Werror

eembed_headers=src/eembed.h src/eembed-arduino.h
include_HEADERS=$(eembed_headers) src/echeck.h
lib_LTLIBRARIES=libecheck.la libeembed.la
libecheck_la_SOURCES=src/echeck.h src/echeck.c
libeembed_la_SOURCES=$(eembed_headers) src/eembed.c

if DEBUG
BUILD_TYPE_CFLAGS=-g -O0 \
	-fno-inline-small-functions \
	-fkeep-inline-functions \
	-fkeep-static-functions \
	--coverage
BUILD_TYPE_LDFLAGS=--coverage
else
BUILD_TYPE_CFLAGS=-g -O2 -DNDEBUG -fomit-frame-pointer
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
 test-eembed-log \
 test-eembed-long-to-str \
 test-eembed-ulong-to-str \
 test-eembed-ulong-to-hex \
 test-eembed-float-to-str \
 test-eembed-memcmp \
 test-eembed-memcpy \
 test-eembed-memmove \
 test-eembed-memset \
 test-eembed-strcat \
 test-eembed-strcpy \
 test-eembed-strlen \
 test-eembed-strstr \
 test_check_status \
 test_check_byte_array \
 test_check_byte_array_m \
 test_check_byte_array_m2 \
 test_check_char \
 test_check_char_m \
 test_check_double \
 test_check_int \
 test_check_int_m \
 test_check_long \
 test_check_long_m \
 test_check_ptr \
 test_check_ptr_m \
 test_check_ptr_not_null \
 test_check_size_t \
 test_check_size_t_m \
 test_check_str \
 test_check_str_null_ptr \
 test_check_str_m \
 test_check_unsigned_int \
 test_check_unsigned_int_m \
 test_check_unsigned_long \
 test_check_unsigned_long_m \
 test_hosted

TESTS_COMMON_SOURCES=src/eembed.h src/echeck.h

T_LDADD=libecheck.la -lecheck libeembed.la -leembed

test_eembed_log_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-log.c
test_eembed_log_LDADD=$(T_LDADD)

test_eembed_long_to_str_SOURCES=$(TESTS_COMMON_SOURCES) \
 tests/test-eembed-long-to-str.c
test_eembed_long_to_str_LDADD=$(T_LDADD)

test_eembed_ulong_to_str_SOURCES=$(TESTS_COMMON_SOURCES) \
 tests/test-eembed-ulong-to-str.c
test_eembed_ulong_to_str_LDADD=$(T_LDADD)

test_eembed_ulong_to_hex_SOURCES=$(TESTS_COMMON_SOURCES) \
 tests/test-eembed-ulong-to-hex.c
test_eembed_ulong_to_hex_LDADD=$(T_LDADD)

test_eembed_float_to_str_SOURCES=$(TESTS_COMMON_SOURCES) \
 tests/test-eembed-float-to-str.c
test_eembed_float_to_str_LDADD=$(T_LDADD)

test_eembed_memcmp_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-memcmp.c
test_eembed_memcmp_LDADD=$(T_LDADD)

test_eembed_memcpy_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-memcpy.c
test_eembed_memcpy_LDADD=$(T_LDADD)

test_eembed_memmove_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-memmove.c
test_eembed_memmove_LDADD=$(T_LDADD)

test_eembed_memset_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-memset.c
test_eembed_memset_LDADD=$(T_LDADD)

test_eembed_strcat_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-strcat.c
test_eembed_strcat_LDADD=$(T_LDADD)

test_eembed_strcpy_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-strcpy.c
test_eembed_strcpy_LDADD=$(T_LDADD)

test_eembed_strlen_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-strlen.c
test_eembed_strlen_LDADD=$(T_LDADD)

test_eembed_strstr_SOURCES=$(TESTS_COMMON_SOURCES) tests/test-eembed-strstr.c
test_eembed_strstr_LDADD=$(T_LDADD)

#---------

test_check_status_SOURCES=tests/test_check_status.c \
 $(TESTS_COMMON_SOURCES)
test_check_status_LDADD=$(T_LDADD)

test_check_byte_array_SOURCES=tests/test_check_byte_array.c \
 $(TESTS_COMMON_SOURCES)
test_check_byte_array_LDADD=$(T_LDADD)

test_check_byte_array_m_SOURCES=tests/test_check_byte_array_m.c \
 $(TESTS_COMMON_SOURCES)
test_check_byte_array_m_LDADD=$(T_LDADD)

test_check_byte_array_m2_SOURCES=tests/test_check_byte_array_m2.c \
 $(TESTS_COMMON_SOURCES)
test_check_byte_array_m2_LDADD=$(T_LDADD)

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

test_check_ptr_not_null_SOURCES=tests/test_check_ptr.c \
 $(TESTS_COMMON_SOURCES)
test_check_ptr_not_null_LDADD=$(T_LDADD)

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
test_hosted_CFLAGS=-std=c11 \
 $(BUILD_ENV_CFLAGS) \
 $(BUILD_TYPE_CFLAGS) \
 $(NOISY_CFLAGS) \
 -I./src \
 -fPIC \
 -pipe
test_hosted_LDADD=$(T_LDADD)

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

vg-test-eembed-log: test-eembed-log
	./libtool --mode=execute valgrind -q ./test-eembed-log

vg-test-eembed-long-to-str: test-eembed-long-to-str
	./libtool --mode=execute valgrind -q ./test-eembed-long-to-str

vg-test-eembed-ulong-to-str: test-eembed-ulong-to-str
	./libtool --mode=execute valgrind -q ./test-eembed-ulong-to-str

vg-test-eembed-ulong-to-hex: test-eembed-ulong-to-hex
	./libtool --mode=execute valgrind -q ./test-eembed-ulong-to-hex

vg-test-eembed-float-to-str: test-eembed-float-to-str
	./libtool --mode=execute valgrind -q ./test-eembed-float-to-str

vg-test-eembed-memcmp: test-eembed-memcmp
	./libtool --mode=execute valgrind -q ./test-eembed-memcmp

vg-test-eembed-memcpy: test-eembed-memcpy
	./libtool --mode=execute valgrind -q ./test-eembed-memcpy

vg-test-eembed-memmove: test-eembed-memmove
	./libtool --mode=execute valgrind -q ./test-eembed-memmove

vg-test-eembed-memset: test-eembed-memset
	./libtool --mode=execute valgrind -q ./test-eembed-memset

vg-test-eembed-strcat: test-eembed-strcat
	./libtool --mode=execute valgrind -q ./test-eembed-strcat

vg-test-eembed-strcpy: test-eembed-strcpy
	./libtool --mode=execute valgrind -q ./test-eembed-strcpy

vg-test-eembed-strlen: test-eembed-strlen
	./libtool --mode=execute valgrind -q ./test-eembed-strlen

vg-test-eembed-strstr: test-eembed-strstr
	./libtool --mode=execute valgrind -q ./test-eembed-strstr

vg-test_check_status: test_check_status
	./libtool --mode=execute valgrind -q ./test_check_status

vg-test_check_byte_array: test_check_byte_array
	./libtool --mode=execute valgrind -q ./test_check_byte_array

vg-test_check_byte_array_m: test_check_byte_array_m
	./libtool --mode=execute valgrind -q ./test_check_byte_array_m

vg-test_check_byte_array_m2: test_check_byte_array_m2
	./libtool --mode=execute valgrind -q ./test_check_byte_array_m2

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

vg-test_check_ptr_not_null: test_check_ptr_not_null
	./libtool --mode=execute valgrind -q ./test_check_ptr_not_null

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
	vg-test-eembed-log \
	vg-test-eembed-long-to-str \
	vg-test-eembed-ulong-to-str \
	vg-test-eembed-ulong-to-hex \
	vg-test-eembed-float-to-str \
	vg-test-eembed-memcmp \
	vg-test-eembed-memcpy \
	vg-test-eembed-memmove \
	vg-test-eembed-memset \
	vg-test-eembed-strcat \
	vg-test-eembed-strcpy \
	vg-test-eembed-strlen \
	vg-test-eembed-strstr \
	vg-test_check_byte_array \
	vg-test_check_byte_array_m \
	vg-test_check_byte_array_m2 \
	vg-test_check_char \
	vg-test_check_char_m \
	vg-test_check_double \
	vg-test_check_int \
	vg-test_check_int_m \
	vg-test_check_long \
	vg-test_check_long_m \
	vg-test_check_ptr \
	vg-test_check_ptr_m \
	vg-test_check_ptr_not_null \
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

line-cov: check
	lcov	--checksum \
		--capture \
		--base-directory . \
		--directory . \
		--output-file coverage.info

html-report: line-cov
	mkdir -pv ./coverage_html
	genhtml coverage.info --output-directory coverage_html

coverage: html-report
	$(BROWSER) ./coverage_html/src/index.html

spotless:
	rm -rf `cat .gitignore | sed -e 's/#.*//'`
	pushd src && rm -rf `cat ../.gitignore | sed -e 's/#.*//'`; popd
	pushd tests && rm -rf `cat ../.gitignore | sed -e 's/#.*//'`; popd

mrproper:
	git clean -dffx
	git submodule foreach --recursive git clean -dffx
