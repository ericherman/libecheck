/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

static void fill_buf(char *buf, size_t buflen, int in, char expected)
{
	size_t len = 0;
	unsigned long ul = 0;

	buf[len] = '\0';

	buf[len++] = 'i';
	buf[len++] = 'n';
	buf[len++] = ':';
	if (in < 0) {
		buf[len++] = '-';
		ul = (-in);
	} else {
		ul = in;
	}
	buf[len] = '\0';
	len = echeck_diy_strnlen(buf, buflen);
	echeck_test_ul_to_str(buf + len, buflen - len, ul);
	len = echeck_diy_strnlen(buf, buflen);
	buf[len++] = ',';
	buf[len] = '\0';
	if (expected < 0) {
		buf[len++] = '-';
		buf[len] = '\0';
		ul = (-expected);
	} else {
		ul = expected;
	}
	echeck_test_ul_to_str(buf + len, buflen - len, ul);
}

unsigned test_check_status(void)
{
	struct echeck_log *orig = NULL;
	unsigned failures = 0;
	char actual = '\0';
	char expected = '\0';
	int i = 0;
	char buf[80];
	const char *expected_strs[1];

	failures = 0;
	buf[0] = '\0';

	for (i = -128; i < 128; ++i) {
		expected = (char)i;
		actual = check_status(i);
		fill_buf(buf, 80, i, expected);
		failures += check_int_m(actual, i, buf);
		if (i == 0) {
			failures +=
			    check_int_m(actual, EXIT_SUCCESS, "EXIT_SUCCESS");
		}
	}

	expected_strs[0] = buf;

	expected = 127;
	for (i = expected; i < 260; ++i) {
		orig = echeck_test_log_capture();
		actual = check_status(i);
		echeck_test_log_release(orig);
		echeck_test_ul_to_str(buf, 80, i);
		if (i != expected) {
			failures += err_contains(expected_strs, 1);
		}
		failures += check_int_m(actual, expected, buf);
	}

	expected = -128;
	for (i = expected; i < -260; --i) {
		echeck_test_log_release(orig);
		actual = check_status(i);
		echeck_test_log_release(orig);
		buf[0] = '-';
		buf[1] = '\0';
		echeck_test_ul_to_str(buf + 1, 80 - 1, (unsigned long)(-i));
		if (i != expected) {
			failures += err_contains(expected_strs, 1);
		}
		failures += check_int_m(actual, expected, buf);
	}

	if (failures) {
		echeck_test_debug_print_failures(failures, "test_check_status");
	}
	return failures;
}

#if ECHECK_HOSTED
int main(int argc, char *argv[])
{
	unsigned failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_status();

	if (failures) {
		echeck_test_debug_print_failures(failures, __FILE__);
	}

	return ((failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
#endif
