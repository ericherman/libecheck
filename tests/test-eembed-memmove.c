/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-memmove.c */
/* Copyright (C) 2017-2024 Eric Herman <eric@freesa.org> */

#include "eembed.h"

static void test_byte_arrays(unsigned char *actual, size_t actual_len,
			     unsigned char *expect, size_t expect_len)
{
	size_t i;
	eembed_crash_if_false(actual_len == expect_len);
	for (i = 0; i < actual_len; ++i) {
		eembed_crash_if_false(actual[i] == expect[i]);
	}
}

unsigned test_eembed_memmove_func(void *(*memmove_func)(void *dest,
							const void *src,
							size_t n)
    )
{
	unsigned char actual[20];
	unsigned char expect[20];
	unsigned char *rv = NULL;
	size_t i = 0;

	for (i = 0; i < 20; ++i) {
		actual[i] = '\0';
		expect[i] = 'a' + i;
	}
	rv = (unsigned char *)memmove_func(actual, expect, 20);
	test_byte_arrays(actual, 20, expect, 20);
	eembed_crash_if_false(rv == actual);

	for (i = 2; i < 12; ++i) {
		expect[i] = expect[i + 4];
	}
	rv = (unsigned char *)memmove_func(actual + 2, actual + 6, 10);
	test_byte_arrays(actual, 20, expect, 20);
	eembed_crash_if_false(rv == (actual + 2));

	for (i = 0; i < 20; ++i) {
		expect[i] = 'a' + i;
		actual[i] = 'a' + i;
	}
	for (i = 15; i > 5; --i) {
		expect[i] = expect[i - 4];
	}
	rv = (unsigned char *)memmove_func(actual + 6, actual + 2, 10);
	test_byte_arrays(actual, 20, expect, 20);
	eembed_crash_if_false(rv == (actual + 6));

	return 0;
}

unsigned test_eembed_memmove(void)
{
	test_eembed_memmove_func(eembed_memmove);
#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	test_eembed_memmove_func(eembed_diy_memmove);
#endif

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_memmove)
