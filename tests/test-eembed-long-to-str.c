/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-long-to-str.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"
#include <limits.h>

void check_long_to_str_nothing_explodes(void)
{
	char buf[30];

	eembed_long_to_str(buf, 3, LONG_MAX);
	eembed_long_to_str(buf, 0, LONG_MAX);
	eembed_long_to_str(NULL, 30, LONG_MAX);
	eembed_long_to_str(buf, 30, LONG_MAX);
}

unsigned check_long_to_str(long l, const char *lstr)
{
	unsigned failures = 0;
	char buf[30];
	char *rv;

	rv = eembed_long_to_str(buf, 30, l);
	failures += check_ptr_m(rv, buf, lstr);
	failures += check_str_m(buf, lstr, lstr);
	return failures;
}

#define Check_long_to_str(ul) check_long_to_str(ul, #ul)

unsigned int test_eembed_long_to_str(void)
{
	unsigned failures = 0;

	failures += Check_long_to_str(0);
	failures += Check_long_to_str(300);
	failures += Check_long_to_str(-400);
	failures += Check_long_to_str(2147483647);
	failures += Check_long_to_str(-2147483647);

	check_long_to_str_nothing_explodes();

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_long_to_str)
