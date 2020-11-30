/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strstr.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_eembed_strstr(void)
{
	unsigned failures = 0;
	char *haystack = "321 Liftoff Ave.\nSnohomish, WA 98291\nUSA";
	char *needle = "Snohomish";
	char *none = "XYZ";
	char *rv = NULL;
	char *expect = NULL;

	rv = eembed_strstr(haystack, haystack);
	expect = haystack;
	failures += check_ptr(rv, expect);

	rv = eembed_strstr(haystack, needle);
	expect = haystack + 17;
	failures += check_ptr(rv, expect);

	rv = eembed_strstr(haystack, none);
	expect = NULL;
	failures += check_ptr(rv, expect);

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_strstr, __FILE__)
