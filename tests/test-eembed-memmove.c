/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-memmove.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_eembed_memmove(void)
{
	unsigned char actual[20];
	unsigned char expect[20];
	char *rv = NULL;
	size_t i = 0;
	unsigned failures = 0;

	for (i = 0; i < 20; ++i) {
		actual[i] = '\0';
		expect[i] = 'a' + i;
	}
	rv = (char *)eembed_memmove(actual, expect, 20);
	failures += check_byte_array(actual, 20, expect, 20);
	failures += check_ptr(rv, actual);

	for (i = 2; i < 12; ++i) {
		expect[i] = expect[i + 4];
	}
	rv = (char *)eembed_memmove(actual + 2, actual + 6, 10);
	failures += check_byte_array(actual, 20, expect, 20);
	failures += check_ptr(rv, actual + 2);

	for (i = 0; i < 20; ++i) {
		expect[i] = 'a' + i;
		actual[i] = 'a' + i;
	}
	for (i = 15; i > 5; --i) {
		expect[i] = expect[i - 4];
	}
	rv = (char *)eembed_memmove(actual + 6, actual + 2, 10);
	failures += check_byte_array(actual, 20, expect, 20);
	failures += check_ptr(rv, actual + 6);

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_memmove)
