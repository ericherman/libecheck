/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strcat.c */
/* Copyright (C) 2016, 2020 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_eembed_strcat(void)
{
	char actual[80];
	char *rv = NULL;
	const char *expect = NULL;
	unsigned failures = 0;

	actual[0] = '\0';
	expect = "foo bar";
	rv = eembed_strcat(actual, "foo");
	failures += check_ptr(rv, actual);
	rv = eembed_strcat(actual, " bar");
	failures += check_ptr(rv, actual);
	failures += check_str(actual, expect);

	rv = eembed_strncat(actual, " whiz bang pow", 10);
	failures += check_ptr(rv, actual);
	expect = "foo bar whiz bang";
	failures += check_str(actual, expect);

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_strcat, __FILE__)
