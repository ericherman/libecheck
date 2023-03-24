/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strcat.c */
/* Copyright (C) 2016, 2020 Eric Herman <eric@freesa.org> */

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

unsigned test_eembed_strcpy(void)
{
	const char *expect = NULL;
	char actual[80];
	char *rv = NULL;

	fill_str(actual, 80, 'X');
	expect = "foo bar";
	rv = eembed_strcpy(actual, expect);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);
	eembed_crash_if_false(rv == actual);

	fill_str(actual, 80, '\0');
	expect = "foo";
	rv = eembed_strncpy(actual, "foo bar", 3);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);
	eembed_crash_if_false(rv == actual);

#if (!EEMBED_HOSTED)
	rv = eembed_strcpy(NULL, "foo bar");
	eembed_crash_if_false(rv == NULL);

	rv = eembed_strncpy(NULL, "foo bar", 7);
	eembed_crash_if_false(rv == NULL);
#endif

	/* strncpy will null-pad a buffer, but if it fits exactly,
	 * will not NULL terminate */
	eembed_strncpy(actual, "abc", 3);
	eembed_crash_if_false(actual[2] == 'c');

	/* strcpy_safe will always NULL terminate */
	eembed_strcpy_safe(actual, 3, "abc");
	eembed_crash_if_false(actual[2] == '\0');

	/* and strcpy_safe will ignore NULL strings */
	eembed_strcpy_safe(actual, 80, NULL);

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_strcpy)
