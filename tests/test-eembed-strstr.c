/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strstr.c */
/* Copyright (C) 2017-2024 Eric Herman <eric@freesa.org> */

#include "eembed.h"

unsigned test_eembed_strstr_func(char *(*strstr_func)(const char *haystack,
						      const char *needle)
    )
{
	const char *haystack = "321 Liftoff Ave.\nSnohomish, WA 98291\nUSA";
	const char *needle = "Snohomish";
	const char *none = "XYZ";
	char *rv = NULL;
	const char *expect = NULL;

	rv = strstr_func(haystack, haystack);
	expect = haystack;
	eembed_crash_if_false(rv == expect);

	rv = strstr_func(haystack, needle);
	expect = haystack + 17;
	eembed_crash_if_false(rv == expect);

	rv = strstr_func(haystack, none);
	expect = NULL;
	eembed_crash_if_false(rv == expect);

	rv = strstr_func(haystack, "");
	expect = haystack;
	eembed_crash_if_false(rv == expect);

	rv = strstr_func(needle, haystack);
	expect = NULL;
	eembed_crash_if_false(rv == expect);

	return 0;
}

unsigned test_eembed_strstr(void)
{
	test_eembed_strstr_func(eembed_strstr);
#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	test_eembed_strstr_func(eembed_diy_strstr);
#endif
	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_strstr)
