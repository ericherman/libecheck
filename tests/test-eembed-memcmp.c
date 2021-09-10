/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-memcmp.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

static void fill_array(void *dest, size_t len, unsigned char c,
		       unsigned char term)
{
	size_t i = 0;
	unsigned char *chars = NULL;

	chars = (unsigned char *)dest;
	for (i = 0; i < (len - 1); ++i) {
		chars[i] = c;
	}
	chars[len - 1] = term;
}

unsigned int test_eembed_memcmp(void)
{
	char s2[20];
	char s1[20];
	int rv1 = 0;
	int rv2 = 0;
	unsigned failures = 0;

	s1[0] = '\0';
	s2[0] = '\0';

	fill_array(s1, 20, 'X', '\0');
	fill_array(s2, 20, 'X', '\0');
	rv1 = eembed_memcmp(s1, s2, 20);
	failures += check_int(rv1, 0);
	rv2 = eembed_strcmp(s1, s2);
	failures += check_int(rv1, rv2);
	rv2 = eembed_strncmp(s1, s2, 20);
	failures += check_int(rv1, rv2);

	fill_array(s2 + 10, 20 - 10, 'O', '\0');
	rv1 = eembed_memcmp(s1, s2, 20);
	failures += check_int(rv1 == 0 ? 0 : 1, 1);
	rv2 = eembed_strcmp(s1, s2);
	failures += check_int(rv1, rv2);
	rv2 = eembed_strncmp(s1, s2, 20);
	failures += check_int(rv1, rv2);

	rv1 = eembed_memcmp(s1, s2, 20 - 10);
	failures += check_int(rv1, 0);
	rv2 = eembed_strncmp(s1, s2, 20 - 10);
	failures += check_int(rv1, rv2);

#if (!EEMBED_HOSTED)
	/* glibc explodes on NULL */
	rv1 = eembed_memcmp(NULL, s2, 20);
	failures += check_int(rv1 == 0 ? 0 : 1, 1);
	rv2 = eembed_memcmp(s1, NULL, 20);
	failures += check_int(rv2 == 0 ? 0 : 1, 1);
#endif

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_memcmp)
