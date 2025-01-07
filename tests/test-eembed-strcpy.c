/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strcat.c */
/* Copyright (C) 2016-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

static void fill_str(char *str, size_t len, char c)
{
	size_t i, last;

	last = len - 1;
	for (i = 0; i < last; ++i) {
		str[i] = c;
	}
	str[last] = '\0';
}

unsigned test_eembed_strcpy_func(char *(*strcpy_func)(char *dst,
						      const char *src),
				 char *(*strncpy_func)(char *dst,
						       const char *src,
						       size_t sz)
    )
{
	const char *expect = NULL;
	char actual[80];
	char *rv = NULL;

	fill_str(actual, 80, 'X');
	expect = "foo bar";
	rv = strcpy_func(actual, expect);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);
	eembed_crash_if_false(rv == actual);

	fill_str(actual, 80, 'X');
	expect = "bar foo";
	rv = strncpy_func(actual, expect, 80);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);
	eembed_crash_if_false(rv == actual);

	fill_str(actual, 80, '\0');
	expect = "foo";
#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
	rv = strncpy_func(actual, "foo bar", 3);
#if __GNUC__
#pragma GCC diagnostic pop
#endif
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);
	eembed_crash_if_false(rv == actual);

	/* strncpy will null-pad a buffer, but if it fits exactly,
	 * will _NOT_ null-terminate */
#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
	strncpy_func(actual, "abc", 3);
#if __GNUC__
#pragma GCC diagnostic pop
#endif
	eembed_crash_if_false(actual[2] == 'c');

	/* strcpy_safe will always NULL terminate */
	eembed_strcpy_safe(actual, 3, "abc");
	eembed_crash_if_false(actual[2] == '\0');

	/* and strcpy_safe will ignore NULL strings */
	eembed_strcpy_safe(actual, 80, NULL);

	return 0;
}

unsigned test_eembed_strcpy(void)
{
	test_eembed_strcpy_func(eembed_strcpy, eembed_strncpy);
#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	test_eembed_strcpy_func(eembed_diy_strcpy, eembed_diy_strncpy);
#endif
	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_strcpy)
