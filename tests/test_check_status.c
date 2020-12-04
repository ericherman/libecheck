/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

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
	len = eembed_strnlen(buf, buflen);
	eembed_ulong_to_str(buf + len, buflen - len, ul);
	len = eembed_strnlen(buf, buflen);
	buf[len++] = ',';
	buf[len] = '\0';
	if (expected < 0) {
		buf[len++] = '-';
		buf[len] = '\0';
		ul = (-expected);
	} else {
		ul = expected;
	}
	eembed_ulong_to_str(buf + len, buflen - len, ul);
}

unsigned test_check_status(void)
{
	struct eembed_log *orig = eembed_err_log;
	struct eembed_str_buf log_ctx;
	struct eembed_log buf_log;
	const size_t mem_buf_len = 1024;
	char mem_buf[1024];

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
			failures += check_int_m(actual, 0, "EXIT_SUCCESS");
		}
	}

	expected_strs[0] = buf;

	expected = 127;
	for (i = expected; i < 260; ++i) {
		eembed_memset(mem_buf, 0x00, mem_buf_len);
		eembed_err_log =
		    eembed_char_buf_log_init(&buf_log,
					     &log_ctx, mem_buf, mem_buf_len);
		actual = check_status_m(i, "127 < 260");
		eembed_err_log = orig;
		eembed_ulong_to_str(buf, 80, i);
		if (i != expected) {
			failures += check_str_contains_all(mem_buf,
							   expected_strs, 1);
		}
		failures += check_int_m(actual, expected, buf);
	}

	expected = -128;
	for (i = expected; i < -260; --i) {
		eembed_err_log = orig;
		actual = check_status(i);
		eembed_err_log = orig;
		buf[0] = '-';
		buf[1] = '\0';
		eembed_ulong_to_str(buf + 1, 80 - 1, (unsigned long)(-i));
		if (i != expected) {
			failures += check_str_contains_all(mem_buf,
							   expected_strs, 1);
		}
		failures += check_int_m(actual, expected, buf);
	}

	return failures;
}

ECHECK_TEST_MAIN(test_check_status)
