/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strstr.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include <eembed.h>

unsigned test_eembed_strstr(void)
{
	const char *haystack = "321 Liftoff Ave.\nSnohomish, WA 98291\nUSA";
	const char *needle = "Snohomish";
	const char *none = "XYZ";
	char *rv = NULL;
	const char *expect = NULL;

	rv = eembed_strstr(haystack, haystack);
	expect = haystack;
	eembed_crash_if_false(rv == expect);

	rv = eembed_strstr(haystack, needle);
	expect = haystack + 17;
	eembed_crash_if_false(rv == expect);

	rv = eembed_strstr(haystack, none);
	expect = NULL;
	eembed_crash_if_false(rv == expect);

	rv = eembed_strstr(haystack, "");
	expect = haystack;
	eembed_crash_if_false(rv == expect);

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_strstr)
