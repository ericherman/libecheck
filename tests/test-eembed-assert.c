/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-assert.c */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include "echeck.h"

#define bad_line(log) \
	do { \
		if (log) { \
			log->append_s(log, "line: "); \
			log->append_ul(log, __LINE__); \
			log->append_eol(log); \
		} \
	} while (0)

#if NDEBUG
unsigned test_eembed_assert(void)
{
	eembed_assert(1 == 1);
	eembed_assert(1 == 2);
	return 0;
}
#else /* NDEBUG */
#if EEMBED_HOSTED
#include <stdlib.h>
extern void (*eembed_exit)(int status);

static unsigned test_eemebed_global_crash_count = 0;
static int test_eembed_global_exit_code = 0;

static void test_eembed_bogus_exit(int status)
{
	test_eembed_global_exit_code = status;
	++test_eemebed_global_crash_count;
}

unsigned test_eembed_assert(void)
{
	unsigned failures = 0;
	struct eembed_log *orig_log = eembed_err_log;
	void (*orig_exit)(int status) = eembed_exit;

	eembed_err_log = NULL;
	eembed_exit = test_eembed_bogus_exit;

	test_eemebed_global_crash_count = 0;
	test_eembed_global_exit_code = -23;
	eembed_assert(1 == 1);
	if (test_eemebed_global_crash_count != 0) {
		bad_line(orig_log);
		++failures;
	}
	if (test_eembed_global_exit_code != -23) {
		bad_line(orig_log);
		++failures;
	}

	test_eemebed_global_crash_count = 0;
	test_eembed_global_exit_code = -23;
	eembed_assert(1 == 2);
	if (test_eemebed_global_crash_count != 1) {
		bad_line(orig_log);
		++failures;
	}
	if (test_eembed_global_exit_code != EXIT_FAILURE) {
		bad_line(orig_log);
		++failures;
	}

	eembed_err_log = orig_log;
	eembed_exit = orig_exit;

	return failures;
}

#else /* EEMBED_HOSTED */

static unsigned test_eemebed_global_crash_count = 0;
static void test_eembed_bogus_assert_crash(void)
{
	++test_eemebed_global_crash_count;
}

unsigned test_eembed_assert(void)
{
	unsigned failures = 0;
	struct eembed_log *orig_log = eembed_err_log;
	void (*orig_assert_crash)(void) = eembed_assert_crash;

	eembed_err_log = NULL;
	eembed_assert_crash = test_eembed_bogus_assert_crash;

	test_eemebed_global_crash_count = 0;
	eembed_assert(1 == 1);
	if (test_eemebed_global_crash_count != 0) {
		bad_line(orig_log);
		++failures;
	}

	test_eemebed_global_crash_count = 0;
	eembed_assert(1 == 2);
	if (test_eemebed_global_crash_count != 1) {
		bad_line(orig_log);
		++failures;
	}

	eembed_err_log = orig_log;
	eembed_assert_crash = orig_assert_crash;

	return failures;
}

#endif /* EEMBED_HOSTED */
#endif /* NDEBUG */

ECHECK_TEST_MAIN(test_eembed_assert)
