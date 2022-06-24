/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-ulong-to-str.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include <eembed.h>
#include <limits.h>

void test_ulong_to_str_nothing_explodes(void)
{
	char buf[30];

	eembed_ulong_to_str(buf, 3, ULONG_MAX);
	eembed_ulong_to_str(buf, 0, ULONG_MAX);
	eembed_ulong_to_str(buf, 1, ULONG_MAX);
	eembed_ulong_to_str(NULL, 30, ULONG_MAX);
	eembed_ulong_to_str(buf, 30, ULONG_MAX);
}

void test_ulong_to_str(unsigned long ul, const char *ulstr)
{
	char buf[30];
	char *rv;
	unsigned long rul = 0;

	rv = eembed_ulong_to_str(buf, 30, ul);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strcmp(buf, ulstr) == 0);

	rul = eembed_str_to_ulong(rv, NULL);
	eembed_crash_if_false(rul == ul);
}

#define Test_ulong_to_str(ul) test_ulong_to_str(ul, #ul)

unsigned int test_eembed_ulong_to_str(void)
{
	Test_ulong_to_str(0);
	Test_ulong_to_str(2147483000);
	Test_ulong_to_str(4294967295);

	test_ulong_to_str_nothing_explodes();

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_ulong_to_str)
