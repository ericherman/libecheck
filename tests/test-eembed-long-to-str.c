/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-long-to-str.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include <eembed.h>
#include <limits.h>
#include <float.h>

void test_long_to_str_nothing_explodes(void)
{
	char buf[30];

	eembed_long_to_str(buf, 3, LONG_MAX);
	eembed_long_to_str(buf, 0, LONG_MAX);
	eembed_long_to_str(NULL, 30, LONG_MAX);
	eembed_long_to_str(buf, 30, LONG_MAX);
}

void test_long_to_str(long l, const char *lstr)
{
	char buf[30];
	char *rv = NULL;
	long rl = 0;

	rv = eembed_long_to_str(buf, 30, l);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strcmp(buf, lstr) == 0);

	rl = eembed_str_to_long(rv, NULL);
	eembed_crash_if_false(rl == l);
}

#define Test_long_to_str(ul) test_long_to_str(ul, #ul)

unsigned int test_eembed_long_to_str(void)
{
	Test_long_to_str(0);
	Test_long_to_str(300);
	Test_long_to_str(-400);
	Test_long_to_str(2147483647);
	Test_long_to_str(-2147483647);

	test_long_to_str_nothing_explodes();

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_long_to_str)
