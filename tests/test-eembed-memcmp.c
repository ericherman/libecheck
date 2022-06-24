/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-memcmp.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include <eembed.h>

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

unsigned int test_eembed_memcmp(void)
{
	char s2[20] =
	    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char s1[20] =
	    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int rv1 = 0;
	int rv2 = 0;

	fill_array_terminated(s1, 20, 'X');
	fill_array_terminated(s2, 20, 'X');
	rv1 = eembed_memcmp(s1, s2, 20);
	eembed_crash_if_false(rv1 == 0);
	rv2 = eembed_strcmp(s1, s2);
	eembed_crash_if_false(rv1 == rv2);
	rv2 = eembed_strncmp(s1, s2, 20);
	eembed_crash_if_false(rv1 == rv2);

	fill_array_terminated(s2 + 10, 20 - 10, 'O');
	rv1 = eembed_memcmp(s1, s2, 20);
	eembed_crash_if_false(rv1 == 0 ? 0 : 1 == 1);
	rv2 = eembed_strcmp(s1, s2);
	eembed_crash_if_false((rv1 == 0 ? 1 : 0) == (rv2 == 0 ? 1 : 0));
	rv2 = eembed_strncmp(s1, s2, 20);
	eembed_crash_if_false((rv1 == 0 ? 1 : 0) == (rv2 == 0 ? 1 : 0));

	rv1 = eembed_memcmp(s1, s2, 20 - 10);
	eembed_crash_if_false(rv1 == 0);
	rv2 = eembed_strncmp(s1, s2, 20 - 10);
	eembed_crash_if_false(rv1 == rv2);

#if (!EEMBED_HOSTED)
	/* glibc explodes on NULL */
	rv1 = eembed_memcmp(NULL, s2, 20);
	eembed_crash_if_false((rv1 == 0 ? 0 : 1) == 1);
	rv2 = eembed_memcmp(s1, NULL, 20);
	eembed_crash_if_false((rv2 == 0 ? 0 : 1) == 1);
#endif

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_memcmp)
