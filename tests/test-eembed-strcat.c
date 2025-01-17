/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strcat.c */
/* Copyright (C) 2016-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

unsigned test_eembed_strcat_func(char *(*strcat_func)(char *dst,
						      const char *src),
				 char *(*strncat_func)(char *dst,
						       const char *src,
						       size_t sz)
    )
{
	char actual[80];
	char *rv = NULL;
	const char *expect = NULL;

	actual[0] = '\0';
	expect = "foo bar";
	rv = strcat_func(actual, "foo");
	eembed_crash_if_false(rv == actual);
	rv = strcat_func(actual, " bar");
	eembed_crash_if_false(rv == actual);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);

#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
	rv = strncat_func(actual, " whiz bang pow", 10);
#if __GNUC__
#pragma GCC diagnostic pop
#endif
	eembed_crash_if_false(rv == actual);
	expect = "foo bar whiz bang";
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);

	return 0;
}

unsigned test_eembed_strcat(void)
{
	test_eembed_strcat_func(eembed_strcat, eembed_strncat);
#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	test_eembed_strcat_func(eembed_diy_strcat, eembed_diy_strncat);
#endif
	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_strcat)
