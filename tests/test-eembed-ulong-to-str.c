/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-ulong-to-str.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"
#include <limits.h>

void check_ulong_to_str_nothing_explodes(void)
{
	char buf[30];

	eembed_ulong_to_str(buf, 3, ULONG_MAX);
	eembed_ulong_to_str(buf, 0, ULONG_MAX);
	eembed_ulong_to_str(buf, 1, ULONG_MAX);
	eembed_ulong_to_str(NULL, 30, ULONG_MAX);
	eembed_ulong_to_str(buf, 30, ULONG_MAX);
}

unsigned check_ulong_to_str(unsigned long ul, const char *ulstr)
{
	unsigned failures = 0;
	char buf[30];
	char *rv;
	unsigned long rul = 0;

	rv = eembed_ulong_to_str(buf, 30, ul);
	failures += check_ptr_m(rv, buf, ulstr);
	failures += check_str_m(buf, ulstr, ulstr);

	rul = eembed_str_to_ulong(rv, NULL);
	failures += check_unsigned_long(rul, ul);

	return failures;
}

#define Check_ulong_to_str(ul) check_ulong_to_str(ul, #ul)

unsigned int test_eembed_ulong_to_str(void)
{
	unsigned failures = 0;

	failures += Check_ulong_to_str(0);
	failures += Check_ulong_to_str(2147483000);
	failures += Check_ulong_to_str(4294967295);

	check_ulong_to_str_nothing_explodes();

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_ulong_to_str)
