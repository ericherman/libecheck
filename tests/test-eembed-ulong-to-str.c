/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-ulong-to-str.c */
/* Copyright (C) 2017-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"
#include <limits.h>

void test_ulong_to_str_nothing_explodes(void)
{
	char buf[30];

	eembed_ulong_to_str(buf, 3, ULONG_MAX);
	eembed_ulong_to_str(buf, 0, ULONG_MAX);
	eembed_ulong_to_str(buf, 1, ULONG_MAX);
	eembed_ulong_to_str(NULL, 30, ULONG_MAX);
	eembed_ulong_to_str(buf, 30, ULONG_MAX);

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	eembed_diy_ulong_to_str(buf, 3, ULONG_MAX);
	eembed_diy_ulong_to_str(buf, 0, ULONG_MAX);
	eembed_diy_ulong_to_str(buf, 1, ULONG_MAX);
	eembed_diy_ulong_to_str(NULL, 30, ULONG_MAX);
	eembed_diy_ulong_to_str(buf, 30, ULONG_MAX);
#endif
}

void test_ulong_to_str(char *buf, size_t size, uint64_t ul, const char *ulstr)
{
	char *rv;
	uint64_t rul = 0;

	eembed_assert(eembed_memset(buf, 0x00, size));

	rv = eembed_ulong_to_str(buf, size, ul);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strcmp(buf, ulstr) == 0);

	rul = eembed_str_to_ulong(rv, NULL);
	eembed_crash_if_false(rul == ul);

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	rv = eembed_diy_ulong_to_str(buf, size, ul);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strcmp(buf, ulstr) == 0);

	rul = eembed_str_to_ulong(rv, NULL);
	eembed_crash_if_false(rul == ul);
#endif
}

#define Test_ulong_to_str(ul) test_ulong_to_str(buf, sizeof(buf), ul, #ul)

unsigned int test_eembed_ulong_to_str(void)
{
	char buf[30];

	Test_ulong_to_str(0);
	Test_ulong_to_str(2147483000);
	Test_ulong_to_str(4294967295);

	test_ulong_to_str(buf, 3, 10, "10");

	test_ulong_to_str(buf, sizeof(buf), UINT64_MAX, "18446744073709551615");
	test_ulong_to_str(buf, 1 + eembed_strlen("18446744073709551615"),
			  UINT64_MAX, "18446744073709551615");

	test_ulong_to_str_nothing_explodes();

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_ulong_to_str)
