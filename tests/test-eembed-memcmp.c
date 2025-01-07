/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-memcmp.c */
/* Copyright (C) 2017-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

static void fill_array_terminated(void *dest, size_t len, unsigned char c)
{
	size_t i = 0;
	unsigned char *chars = NULL;

	chars = (unsigned char *)dest;
	for (i = 0; i < (len - 1); ++i) {
		chars[i] = c;
	}
	chars[len - 1] = '\0';
}

unsigned int
test_memcmp_func(int (*memcmp_func)(const void *s1, const void *s2, size_t n),
		 int (*strcmp_func)(const char *s1, const char *s2),
		 int (*strncmp_func)(const char *s1, const char *s2, size_t n)
    )
{
	char s2[20] =
	    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char s1[20] =
	    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int rv1 = 0;
	int rv2 = 0;

	fill_array_terminated(s1, 20, 'X');

	rv1 = memcmp_func(s1, s1, 20);
	eembed_crash_if_false(rv1 == 0);
	rv2 = strcmp_func(s1, s1);
	eembed_crash_if_false(rv1 == rv2);
	rv2 = strncmp_func(s1, s1, 20);
	eembed_crash_if_false(rv1 == rv2);

	fill_array_terminated(s2, 20, 'X');

	rv1 = memcmp_func(s1, s2, 20);
	eembed_crash_if_false(rv1 == 0);
	rv2 = strcmp_func(s1, s2);
	eembed_crash_if_false(rv1 == rv2);
	rv2 = strncmp_func(s1, s2, 20);
	eembed_crash_if_false(rv1 == rv2);

	fill_array_terminated(s2 + 10, 20 - 10, 'O');
	rv1 = memcmp_func(s1, s2, 20);
	eembed_crash_if_false(rv1 == 0 ? 0 : 1 == 1);
	rv2 = strcmp_func(s1, s2);
	eembed_crash_if_false((rv1 == 0 ? 1 : 0) == (rv2 == 0 ? 1 : 0));
	rv2 = strncmp_func(s1, s2, 20);
	eembed_crash_if_false((rv1 == 0 ? 1 : 0) == (rv2 == 0 ? 1 : 0));

	rv1 = memcmp_func(s1, s2, 20 - 10);
	eembed_crash_if_false(rv1 == 0);
	rv2 = strncmp_func(s1, s2, 20 - 10);
	eembed_crash_if_false(rv1 == rv2);

	return 0;
}

unsigned int test_eembed_memcmp(void)
{
	test_memcmp_func(eembed_memcmp, eembed_strcmp, eembed_strncmp);
#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	test_memcmp_func(eembed_diy_memcmp, eembed_diy_strcmp,
			 eembed_diy_strncmp);
#endif
	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_memcmp)
