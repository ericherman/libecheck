/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-mempy.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

static void fill_array(unsigned char *a, size_t len, char c, char term)
{
	size_t i, last;

	last = len - 1;
	for (i = 0; i < last; ++i) {
		a[i] = c;
	}
	a[last] = term;
}

unsigned test_eembed_memcpy(void)
{
	unsigned char expect[20];
	unsigned char actual[20];
	char *rv;
	int failures;

	failures = 0;
	fill_array(actual, 20, '\0', '\0');
	fill_array(expect, 20, 'Y', '\0');
	rv = eembed_memcpy(actual, expect, 20);
	failures += check_byte_array(actual, 20, expect, 20);
	failures += check_ptr(rv, actual);

	fill_array(actual, 20, '\0', '\0');
	fill_array(expect, 20, 'X', '\0');
	fill_array(expect + 10, 20 - 10, 'Y', '\0');
	eembed_memcpy(actual, expect, 10);
	fill_array(expect, 20, '\0', '\0');
	fill_array(expect, 10, 'X', 'X');
	failures += check_byte_array(actual, 20, expect, 20);
	failures += check_ptr(rv, actual);

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_memcpy, __FILE__)
