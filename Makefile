# SPDX-License-Identifier: LGPL-3.0-or-later
# Makefile: "E(asy)Check" boiler-plate to make simple testing easier
# Copyright (C) 2016-2022 Eric Herman <eric@freesa.org>

# $@ : target label
# $< : the first prerequisite after the colon
# $^ : all of the prerequisite files
# $* : wildcard matched part
# Target-specific Variable syntax:
#  https://www.gnu.org/software/make/manual/html_node/Target_002dspecific.html
#
# patsubst, subst, foreach : $(patsubst pattern,replacement,text)
#  https://www.gnu.org/software/make/manual/html_node/Text-Functions.html
# call : $(call func,param1,param2,...)
#  https://www.gnu.org/software/make/manual/html_node/Call-Function.html
# define :
#  https://www.gnu.org/software/make/manual/html_node/Multi_002dLine.html
# dir, notdir :
#  https://www.gnu.org/software/make/manual/html_node/File-Name-Functions.html

CC ?= gcc
BROWSER	?= firefox
VALGRIND ?= $(shell which valgrind)

# pushd, popd are bash-ism
SHELL := /bin/bash
UNAME := $(shell uname)

# VERSION=2.1.0
VERSION := $(shell cat VERSION | xargs)
VER_MAJOR := `echo $(VERSION) | cut -f1 -d'.'`
VER_MINOR := `echo $(VERSION) | cut -f2 -d'.'`
VER_PATCH := `echo $(VERSION) | cut -f3 -d'.'`

ifeq ($(UNAME), Darwin)
SHAREDEXT = dylib
define make-shared-flags
	-dynamiclib
endef
else
SHAREDEXT = so
define make-shared-flags
	-shared -Wl,-soname,$(notdir $(1))
endef
endif

# This Makefile can create several types of builds, including the standard
# "build" directory, but also "faux-freestanding", "debug" and "coverage"
# (and combination) builds. Each build type gets its own directory, so they
# do not interfere with each other. The directories are:
build_dirs=\
 build \
 faux-fs \
 debug \
 debug-faux-fs \
 debug-coverage \
 debug-coverage-faux-fs

.PHONY:
all: all-obj check-all
	find $(build_dirs) \
		-type f -name '*.o' \
		-o -type f -name '*.$(SHAREDEXT)*'
	@echo "SUCCESS $@"

# .PHONY:
all-obj: $(foreach DIR,$(build_dirs),\
	$(DIR)/eembed.o $(DIR)/libeembed.$(SHAREDEXT) $(DIR)/libeembed.a \
	$(DIR)/echeck.o $(DIR)/libecheck.$(SHAREDEXT) $(DIR)/libecheck.a \
	)
	@echo "SUCCESS $@"


# As c11 is probably "everywhere enough", most should use it,
# however the code should still compile and run with -std=c89
CSTD_CFLAGS=-std=c89
# CSTD_CFLAGS=-std=c11

NOISY_CFLAGS=-Wall -Wextra -pedantic -Wcast-qual -Wc++-compat

COMMON_CFLAGS=$(CSTD_CFLAGS) $(NOISY_CFLAGS) -pipe

NORMAL_CFLAGS=-g -O2 -DNDEBUG -fomit-frame-pointer -fstack-protector-all

DEBUG_CFLAGS=-g -O2 -Werror

# -Xlinker \
# -zmuldefs \
#
COVERAGE_CFLAGS=-O0 -g -Werror \
	-fno-inline-small-functions \
	-fkeep-inline-functions \
	-fkeep-static-functions \
	-fprofile-arcs \
	-ftest-coverage \
	--coverage
COVERAGE_LDFLAGS=--coverage

FAUX_FREESTANDING_ENV_CFLAGS=-DFAUX_FREESTANDING=1 -DEEMBED_HOSTED=0

# Each build type implies target specific BUILD CFLAGS and LDFLAGS variables:
build/%: BUILD_DIR = build
build/%: BUILD_CFLAGS = $(COMMON_CFLAGS) \
	$(NORMAL_CFLAGS) \
	$(HOSTED_ENV_CFLAGS) \
	$(CFLAGS)
build/%: BUILD_LDFLAGS = $(LDFLAGS)
check-%: BUILD_DIR = build

faux-fs/%: BUILD_DIR = faux-fs
faux-fs/%: BUILD_CFLAGS = $(COMMON_CFLAGS) \
	$(NORMAL_CFLAGS) \
	$(FAUX_FREESTANDING_ENV_CFLAGS) \
	$(CFLAGS)
faux-fs/%: BUILD_LDFLAGS = $(LDFLAGS)
faux-fs-check-%: BUILD_DIR = faux-fs

debug/%: BUILD_DIR = debug
debug/%: BUILD_CFLAGS = $(COMMON_CFLAGS) \
	$(DEBUG_CFLAGS) \
	$(HOSTED_ENV_CFLAGS) \
	$(CFLAGS)
debug/%: BUILD_LDFLAGS = $(LDFLAGS)
debug-check-%: BUILD_DIR = debug

debug-faux-fs/%: BUILD_DIR = debug-faux-fs
debug-faux-fs/%: BUILD_CFLAGS = $(COMMON_CFLAGS) \
	$(DEBUG_CFLAGS) \
	$(FAUX_FREESTANDING_ENV_CFLAGS) \
	$(CFLAGS)
debug-faux-fs/%: BUILD_LDFLAGS = $(LDFLAGS)
debug-faux-fs-check-%: BUILD_DIR = debug-faux-fs

debug-coverage/%: BUILD_DIR = debug-coverage
debug-coverage/%: BUILD_CFLAGS = $(COMMON_CFLAGS) \
	$(COVERAGE_CFLAGS) \
	$(HOSTED_ENV_CFLAGS) \
	$(CFLAGS)
debug-coverage/%: BUILD_LDFLAGS = $(COVERAGE_LDFLAGS) $(LDFLAGS)
debug-coverage/%: BUILD_LDADD = -lgcov
debug-coverage-check-%: BUILD_DIR = debug-coverage

debug-coverage-faux-fs/%: BUILD_DIR = debug-coverage-faux-fs
debug-coverage-faux-fs/%: BUILD_CFLAGS = $(COMMON_CFLAGS) \
	$(COVERAGE_CFLAGS) \
	$(FAUX_FREESTANDING_ENV_CFLAGS) \
	$(CFLAGS)
debug-coverage-faux-fs/%s: BUILD_LDFLAGS = $(COVERAGE_LDFLAGS) $(LDFLAGS)
debug-coverage-faux-fs/%: BUILD_LDADD = -lgcov
debug-coverage-faux-fs-check-%: BUILD_DIR = debug-coverage-faux-fs


test_progs=\
 test-eembed-assert \
 test-eembed-log \
 test-eembed-print \
 test-eembed-long-to-str \
 test-eembed-ulong-to-str \
 test-eembed-ulong-to-hex \
 test-eembed-float-to-str \
 test-eembed-memcmp \
 test-eembed-memcpy \
 test-eembed-memmove \
 test-eembed-memset \
 test-eembed-random-bytes \
 test-eembed-random-bytes-2 \
 test-eembed-strcat \
 test-eembed-strcpy \
 test-eembed-strlen \
 test-eembed-strstr \
 test-eembed-chunk-alloc \
 test-eembed-chunk-realloc \
 test-eembed-malloc-free \
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
 test_out_of_memory \
 test_echeck_err_log

# Make will normally delete intermediate files which it views as no longer
# needed; the ".o" files are examples of this. We set .PRECIOUS to prevent
# output files from getting automatically cleaned up.
# https://www.gnu.org/software/make/manual/html_node/Chained-Rules.html
# https://www.gnu.org/software/make/manual/html_node/Special-Targets.html
PRECIOUS=%.o %.$(SHAREDEXT) %.$(SHAREDEXT).% %.a %.html \
		$(foreach DIR,$(build_dirs),\
			$(foreach TEST,$(test_progs), \
				$(DIR)/tests/$(TEST)))
.PRECIOUS:$(PRECIOUS)

# usage build-o(path/foo.o,src/foo.c)
define build-o
	@mkdir -pv $(dir $1)
	cd $(BUILD_DIR) && \
		$(CC) -c -fPIC -I../src $(BUILD_CFLAGS) \
			../$2 \
			-o $(subst $(BUILD_DIR)/,,$1) $(BUILD_LDADD)
endef

# usage build-exe(path/test_name,tests/test_src.c)
define build-exe
	@mkdir -pv $(dir $1)
	cd $(BUILD_DIR) && \
		$(CC) -I../src $(BUILD_CFLAGS) \
			eembed.o echeck.o ../$2 \
			-o $(subst $(BUILD_DIR)/,,$1) $(BUILD_LDADD)
endef

# usage build-so(so_name,major,minor,patch,so_objs)
define build-so
	@mkdir -pv $(BUILD_DIR)
	cd $(BUILD_DIR) && $(CC) $(call make-shared-flags,$1) \
		-o $(subst $(BUILD_DIR)/,,$1).$2.$3.$4 $5 $(BUILD_LDADD)
	{ pushd $(BUILD_DIR); \
		ln -sfv $(notdir $1.$2.$3.$4) $(notdir $1.$2.$3); \
		ln -sfv $(notdir $1.$2.$3.$4) $(notdir $1.$2); \
		ln -sfv $(notdir $1.$2.$3.$4) $(notdir $1); \
	 popd; }
	ls -l $(BUILD_DIR)/*.$(SHAREDEXT)*
endef

# usage build-a(a_name,so_objs)
define build-a
	@echo "build-a($1,$2)"
	@mkdir -pv $(BUILD_DIR)
	ar -r $1 $(shell readlink -f $2)
	ar -t $1
endef

src/eembed.c: src/eembed.h
	ls -l $^ $@

src/echeck.c: src/echeck.h
	ls -l $^ $@

#
# eembed.o
#
$(foreach %,$(build_dirs),%/eembed.o): src/eembed.c
	$(call build-o,$@,$<)

#
# libeembed.so
#
build/libeembed.$(SHAREDEXT): build/eembed.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),eembed.o)

faux-fs/libeembed.$(SHAREDEXT): faux-fs/eembed.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),eembed.o)

debug/libeembed.$(SHAREDEXT): debug/eembed.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),eembed.o)

debug-faux-fs/libeembed.$(SHAREDEXT): debug-faux-fs/eembed.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),eembed.o)

debug-coverage/libeembed.$(SHAREDEXT): debug-coverage/eembed.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),eembed.o)

debug-coverage-faux-fs/libeembed.$(SHAREDEXT): debug-coverage-faux-fs/eembed.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),eembed.o)

#
# libeembed.a
#
build/libeembed.a: build/libeembed.$(SHAREDEXT)
	$(call build-a,$@,$^)

faux-fs/libeembed.a: faux-fs/libeembed.$(SHAREDEXT)
	$(call build-a,$@,$^)

debug/libeembed.a: debug/libeembed.$(SHAREDEXT)
	$(call build-a,$@,$^)

debug-faux-fs/libeembed.a: debug-faux-fs/libeembed.$(SHAREDEXT)
	$(call build-a,$@,$^)

debug-coverage/libeembed.a: debug-coverage/libeembed.$(SHAREDEXT)
	$(call build-a,$@,$^)

debug-coverage-faux-fs/libeembed.a: \
		debug-coverage-faux-fs/libeembed.$(SHAREDEXT)
	$(call build-a,$@,$^)



#
# echeck.o
#
$(foreach %,$(build_dirs),%/echeck.o): src/echeck.c
	$(call build-o,$@,$<)

#
# libecheck.so
#
build/libecheck.$(SHAREDEXT): build/echeck.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),echeck.o)

faux-fs/libecheck.$(SHAREDEXT): faux-fs/echeck.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),echeck.o)

debug/libecheck.$(SHAREDEXT): debug/echeck.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),echeck.o)

debug-faux-fs/libecheck.$(SHAREDEXT): debug-faux-fs/echeck.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),echeck.o)

debug-coverage/libecheck.$(SHAREDEXT): debug-coverage/echeck.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),echeck.o)

debug-coverage-faux-fs/libecheck.$(SHAREDEXT): debug-coverage-faux-fs/echeck.o
	$(call build-so,$@,$(VER_MAJOR),$(VER_MINOR),$(VER_PATCH),echeck.o)

#
# libecheck.a
#
build/libecheck.a: build/libecheck.$(SHAREDEXT)
	$(call build-a,$@,$^)

faux-fs/libecheck.a: faux-fs/libecheck.$(SHAREDEXT)
	$(call build-a,$@,$^)

debug/libecheck.a: debug/libecheck.$(SHAREDEXT)
	$(call build-a,$@,$^)

debug-faux-fs/libecheck.a: debug-faux-fs/libecheck.$(SHAREDEXT)
	$(call build-a,$@,$^)

debug-coverage/libecheck.a: debug-coverage/libecheck.$(SHAREDEXT)
	$(call build-a,$@,$^)

debug-coverage-faux-fs/libecheck.a: \
		debug-coverage-faux-fs/libecheck.$(SHAREDEXT)
	$(call build-a,$@,$^)



#
# 'build' TESTS
#
build/tests/test%: tests/test%.c \
		build/echeck.o build/eembed.o
	$(call build-exe,$@,$<)

.PHONY:
check-%: build/tests/%
	pushd build && ../$<
	@echo "SUCCESS $@"

check_static_asserts=\
	demo-eembed-static-assert \
	demo-eembed-static-assert-dl \
	demo-eembed-static-assert-fail \

.PHONY:
build-check: $(patsubst %, check-%, $(test_progs) $(check_static_asserts))
	@echo "SUCCESS $@"


#
# 'faux-fs' TESTS
#
faux-fs/tests/test%: tests/test%.c \
		faux-fs/echeck.o faux-fs/eembed.o
	$(call build-exe,$@,$<)

.PHONY:
faux-fs-check-%: faux-fs/tests/%
	pushd faux-fs && ../$<
	@echo "SUCCESS $@"

.PHONY:
faux-fs-check: $(patsubst %, faux-fs-check-%, $(test_progs) \
		$(check_static_asserts))
	@echo "SUCCESS $@"


#
# 'debug' TESTS
#
debug/tests/test%: tests/test%.c \
		debug/echeck.o debug/eembed.o
	$(call build-exe,$@,$<)

.PHONY:
debug-check-%: debug/tests/%
	pushd debug && $(VALGRIND) ../$< > $@.out 2>&1
	if [ $$(grep -c 'definitely lost' debug/$@.out) -eq 0 ]; \
		then true; else false; fi && \
		if [ $$(grep -c 'probably lost' debug/$@.out) -eq 0 ]; \
		then true; else false; fi
	@echo "SUCCESS $@"

.PHONY:
debug-check: $(patsubst %, debug-check-%, $(test_progs) $(check_static_asserts))
	@echo "SUCCESS $@"


#
# 'debug-faux-fs' TESTS
#
debug-faux-fs/tests/test%: tests/test%.c \
		debug-faux-fs/echeck.o debug-faux-fs/eembed.o
	$(call build-exe,$@,$<)

.PHONY:
debug-faux-fs-check-%: debug-faux-fs/tests/%
	pushd debug-faux-fs && $(VALGRIND) ../$< > $@.out 2>&1
	if [ $$(grep -c 'definitely lost' debug-faux-fs/$@.out) -eq 0 ]; \
		then true; else false; fi && \
		if [ $$(grep -c 'probably lost' debug-faux-fs/$@.out) -eq 0 ]; \
		then true; else false; fi
	@echo "SUCCESS $@"

.PHONY:
debug-faux-fs-check: $(patsubst %, debug-faux-fs-check-%, $(test_progs) \
		$(check_static_asserts))
	@echo "SUCCESS $@"


#
# 'debug-coverage' TESTS
#
debug-coverage/tests/test%: tests/test%.c \
		debug-coverage/echeck.o debug-coverage/eembed.o
	$(call build-exe,$@,$<)

.PHONY:
debug-coverage-check-%: debug-coverage/tests/%
	pushd debug-coverage && ../$<
	@echo "SUCCESS $@"

.PHONY:
debug-coverage-check: $(patsubst %, debug-coverage-check-%, $(test_progs) \
		$(check_static_asserts))
	@echo "SUCCESS $@"

#
# 'debug-coverage-faux-fs' TESTS
#
debug-coverage-faux-fs/tests/test%: tests/test%.c \
		debug-coverage-faux-fs/echeck.o \
		debug-coverage-faux-fs/eembed.o
	$(call build-exe,$@,$<)

.PHONY:
debug-coverage-faux-fs-check-%: debug-coverage-faux-fs/tests/%
	pushd debug-coverage-faux-fs && ../$<
	@echo "SUCCESS $@"

.PHONY:
debug-coverage-faux-fs-check: $(patsubst %, debug-coverage-faux-fs-check-%, \
		$(test_progs) $(check_static_asserts))
	@echo "SUCCESS $@"

#
# check demo-eembed-static-assert
#
$(foreach DIR,$(build_dirs),$(DIR)/demo-eembed-static-assert): \
		tests/demo-eembed-static-assert.c all-obj
	$(call build-exe,$@,$<)
	@echo "SUCCESS $@"

.PHONY:
check-demo-eembed-static-assert: build/demo-eembed-static-assert
	$<
	@echo "SUCCESS $@"

.PHONY:
faux-fs-check-demo-eembed-static-assert: faux-fs/demo-eembed-static-assert
	$<
	@echo "SUCCESS $@"

.PHONY:
debug-check-demo-eembed-static-assert: debug/demo-eembed-static-assert
	$<
	@echo "SUCCESS $@"

.PHONY:
debug-faux-fs-check-demo-eembed-static-assert: \
		debug-faux-fs/demo-eembed-static-assert
	$<
	@echo "SUCCESS $@"

.PHONY:
debug-coverage-check-demo-eembed-static-assert: \
		debug-coverage/demo-eembed-static-assert
	cd debug-coverage && ./demo-eembed-static-assert
	@echo "SUCCESS $@"

.PHONY:
debug-coverage-faux-fs-check-demo-eembed-static-assert: \
		debug-coverage-faux-fs/demo-eembed-static-assert
	cd debug-coverage-faux-fs && ./demo-eembed-static-assert
	@echo "SUCCESS $@"


$(foreach DIR,$(build_dirs),$(DIR)/demo-eembed-static-assert-dl): \
		tests/demo-eembed-static-assert.c all-obj
	cd $(BUILD_DIR) && \
		$(CC) $(BUILD_CFLAGS) -I ../src \
			-L . $(BUILD_LDFLAGS) \
			./libeembed.$(SHAREDEXT) \
			../$< -o $(subst $(BUILD_DIR)/,,$@) \
			$(BUILD_LDADD) -leembed
	@echo "SUCCESS $@"

.PHONY:
check-demo-eembed-static-assert-dl: build/demo-eembed-static-assert-dl
	LD_LIBRARY_PATH=build $<
	@echo "SUCCESS $@"

.PHONY:
faux-fs-check-demo-eembed-static-assert-dl: faux-fs/demo-eembed-static-assert-dl
	LD_LIBRARY_PATH=faux-fs $<
	@echo "SUCCESS $@"

.PHONY:
debug-check-demo-eembed-static-assert-dl: debug/demo-eembed-static-assert-dl
	LD_LIBRARY_PATH=debug $<
	@echo "SUCCESS $@"

.PHONY:
debug-faux-fs-check-demo-eembed-static-assert-dl: \
		debug-faux-fs/demo-eembed-static-assert-dl
	LD_LIBRARY_PATH=debug-faux-fs $<
	@echo "SUCCESS $@"

.PHONY:
debug-coverage-check-demo-eembed-static-assert-dl: \
		debug-coverage/demo-eembed-static-assert-dl
	cd debug-coverage && \
		LD_LIBRARY_PATH=. ./demo-eembed-static-assert-dl
	@echo "SUCCESS $@"

.PHONY:
debug-coverage-faux-fs-check-demo-eembed-static-assert-dl: \
		debug-coverage-faux-fs/demo-eembed-static-assert-dl
	cd debug-coverage-faux-fs && \
		LD_LIBRARY_PATH=. ./demo-eembed-static-assert-dl
	@echo "SUCCESS $@"

$(foreach DIR,$(build_dirs),$(DIR)/demo-eembed-static-assert-fail.out): \
		tests/demo-eembed-static-assert.c all-obj
	mkdir -pv $(BUILD_DIR)
	cd $(BUILD_DIR) && \
		if $(CC) -DFAIL=1 $(BUILD_CFLAGS) -I ../src ./eembed.o \
		../$< -o a.out > \
		../$@ \
		2>&1; then false; else true; fi
	ls -l $@
	@echo "SUCCESS $@"

.PHONY:
$(foreach DIR,$(build_dirs),$(DIR)-check-demo-eembed-static-assert-fail): \
		$(foreach DIR,$(build_dirs),\
			$(DIR)/demo-eembed-static-assert-fail.out)
	if [ $$(grep -c 'static_assert' \
		$(BUILD_DIR)/demo-eembed-static-assert-fail.out ) \
		-eq 0 ]; \
		then false; else true; fi
	@echo "SUCCESS $@"

.PHONY:
check-demo-eembed-static-assert-fail: build-check-demo-eembed-static-assert-fail
	@echo "SUCCESS $@"

.PHONY:
check: build-check faux-fs-check
	@echo "SUCCESS $@"

check-all: build-check faux-fs-check debug-check debug-faux-fs-check \
		debug-coverage-check debug-coverage-faux-fs-check \
		check-coverage
	@echo "SUCCESS $@"

valgrind: debug-check debug-faux-fs-check
	@echo "SUCCESS $@"

debug-coverage/coverage.info: debug-coverage-check
	lcov	--checksum \
		--capture \
		--base-directory $(dir $@) \
		--directory $(dir $@) \
		--output-file $@
	ls -l $@

debug-coverage/tests/coverage_html/src/index.html: debug-coverage/coverage.info
	mkdir -pv $(dir $@)
	genhtml $< --output-directory \
		./debug-coverage/tests/coverage_html
	ls -l $@

.PHONY: check-coverage-debug
check-coverage-debug: debug-coverage/tests/coverage_html/src/index.html
	if [ $$(grep -c 'headerCovTableEntryHi">100.0 %' $< ) -eq 2 ]; then \
		true; \
	else grep headerCovTableEntryHi $< && \
		false; \
	fi
	@echo "SUCCESS $@"

.PHONY: coverage-debug
coverage-debug: debug-coverage/tests/coverage_html/src/index.html
	$(BROWSER) $<


debug-coverage-faux-fs/coverage-faux-fs.info: debug-coverage-faux-fs-check
	lcov	--checksum \
		--capture \
		--base-directory $(dir $@) \
		--directory $(dir $@) \
		--output-file $@
	ls -l $@

debug-coverage-faux-fs/tests/coverage-faux-fs_html/src/index.html: \
		debug-coverage-faux-fs/coverage-faux-fs.info
	mkdir -pv $(dir $@)
	genhtml $< --output-directory \
		./debug-coverage-faux-fs/tests/coverage-faux-fs_html
	ls -l $@

.PHONY: check-coverage-faux-fs
check-coverage-faux-fs: \
 debug-coverage-faux-fs/tests/coverage-faux-fs_html/src/index.html
	if [ $$(grep -c 'headerCovTableEntryHi">100.0 %' $< ) -eq 2 ]; then \
		true; \
	else grep headerCovTableEntryHi $< && \
		false; \
	fi
	@echo "SUCCESS $@"

.PHONY: coverage-faux-fs
coverage-faux-fs: \
 debug-coverage-faux-fs/tests/coverage-faux-fs_html/src/index.html
	$(BROWSER) $<

.PHONY: check-coverage
check-coverage: check-coverage-debug check-coverage-faux-fs
	@echo "SUCCESS $@"

.PHONY: coverage
coverage: coverage-faux-fs coverage-debug
	@echo "SUCCESS $@"

.PHONY: tidy
tidy: bin/ctidy
	bin/ctidy \
		-T echeck_err_injecting_context \
		-T eembed_allocator \
		-T eembed_alloc_chunk \
		-T eembed_log \
		-T eembed_str_buf \
		`find src tests -name '*.h' -o -name '*.c' -o -name '*.cpp'` \
		eembed_arduino_demo/eembed_arduino_demo.ino \
		echeck_tests_arduino/echeck_tests_arduino.ino \
		echeck_arduino_demo/echeck_arduino_demo.ino

.PHONY: clean
clean:
	rm -rfv $(build_dirs)
	rm -rfv `cat .gitignore | sed -e 's/#.*//'`
	pushd src && rm -rfv `cat ../.gitignore | sed -e 's/#.*//'`; popd
	pushd tests && rm -rfv `cat ../.gitignore | sed -e 's/#.*//'`; popd

.PHONY: mrproper
mrproper:
	git clean -dffx
	git submodule foreach --recursive git clean -dffx
