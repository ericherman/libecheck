/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strcat.c */
/* Copyright (C) 2016, 2020 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

static void fill_str(char *str, size_t len, char c)
{
	size_t i, last;

	last = len - 1;
	for (i = 0; i < last; ++i) {
		str[i] = c;
	}
	str[last] = '\0';
}

unsigned test_eembed_strcpy(void)
{
	const char *expect = NULL;
	char actual[80];
	char *rv = NULL;
	unsigned failures = 0;

	fill_str(actual, 80, 'X');
	expect = "foo bar";
	rv = eembed_strcpy(actual, expect);
	failures += check_str(actual, expect);
	failures += check_ptr(rv, actual);

	fill_str(actual, 80, '\0');
	expect = "foo";
	eembed_strncpy(actual, "foo bar", 3);
	failures += check_str(actual, expect);
	failures += check_ptr(rv, actual);

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_strcpy, __FILE__)
