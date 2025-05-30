/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-memset.c */
/* Copyright (C) 2017-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

static void fill_str(char *str, size_t len, char c, char term)
{
	size_t i, last;

	last = len - 1;
	for (i = 0; i < last; ++i) {
		str[i] = c;
	}
	str[last] = term;
}

unsigned test_eembed_memset_func(void *(*memset_func)(void *s, int c, size_t n))
{
	char expect[20];
	char actual[20];
	char *rv;
	size_t bytes;

	fill_str(actual, 20, 'Y', '\0');
	fill_str(expect, 10, 'X', 'X');
	fill_str(expect + 10, 10, 'Y', '\0');
	bytes = 10;
	rv = (char *)memset_func(actual, 'X', bytes);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);
	eembed_crash_if_false(rv == actual);

	fill_str(actual, 20, 'Z', '\0');
	fill_str(expect, 20, 'Z', '\0');
	bytes = 0;
	rv = (char *)memset_func(actual, 'Q', bytes);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);
	eembed_crash_if_false(rv == actual);

	return 0;
}

unsigned test_eembed_memset(void)
{
	test_eembed_memset_func(eembed_memset);
#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	test_eembed_memset_func(eembed_diy_memset);
#endif
	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_memset)
