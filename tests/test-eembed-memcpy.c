/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-mempy.c */
/* Copyright (C) 2017-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

static void fill_array(unsigned char *a, size_t len, char c, char term)
{
	size_t i, last;

	last = len - 1;
	for (i = 0; i < last; ++i) {
		a[i] = c;
	}
	a[last] = term;
}

static void test_byte_array(unsigned char *actual, size_t actual_len,
			    unsigned char *expect, size_t expect_len)
{
	size_t i;
	eembed_crash_if_false(actual_len == expect_len);
	for (i = 0; i < actual_len; ++i) {
		eembed_crash_if_false(actual[i] == expect[i]);
	}
}

unsigned test_eembed_memcpy_func(void *(*memcpy_func)(void *dest,
						      const void *src, size_t n)
    )
{
	unsigned char expect[20];
	unsigned char actual[20];
	char *rv;

	fill_array(actual, 20, '\0', '\0');
	fill_array(expect, 20, 'Y', '\0');
	rv = (char *)memcpy_func(actual, expect, 20);
	test_byte_array(actual, 20, expect, 20);
	eembed_crash_if_false((void *)rv == (void *)actual);

	fill_array(actual, 20, '\0', '\0');
	fill_array(expect, 20, 'X', '\0');
	fill_array(expect + 10, 20 - 10, 'Y', '\0');
	memcpy_func(actual, expect, 10);
	fill_array(expect, 20, '\0', '\0');
	fill_array(expect, 10, 'X', 'X');
	test_byte_array(actual, 20, expect, 20);
	eembed_crash_if_false((void *)rv == (void *)actual);

	return 0;
}

unsigned test_eembed_memcpy(void)
{
	test_eembed_memcpy_func(eembed_memcpy);
#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	test_eembed_memcpy_func(eembed_diy_memcpy);
#endif
	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_memcpy)
