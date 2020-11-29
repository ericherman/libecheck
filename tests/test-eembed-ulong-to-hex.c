/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-ulong-to-hex.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"
#include <limits.h>

void check_ulong_to_hex_nothing_explodes(void)
{
	char buf[30];

	eembed_ulong_to_hex(buf, 3, ULONG_MAX);
	eembed_ulong_to_hex(buf, 0, ULONG_MAX);
	eembed_ulong_to_hex(NULL, 30, ULONG_MAX);
	eembed_ulong_to_hex(buf, 30, ULONG_MAX);
}

unsigned check_ulong_to_hex(unsigned long ul, const char *ulhex)
{
	unsigned failures = 0;
	char buf[30];
	char *rv;

	rv = eembed_ulong_to_hex(buf, 30, ul);
	failures += check_ptr_m(rv, buf, ulhex);
	failures += check_ptr_not_null_m(eembed_strstr(buf, ulhex), ulhex);
	return failures;
}

unsigned int test_eembed_ulong_to_hex(void)
{
	unsigned failures = 0;

	failures += check_ulong_to_hex(0, "0x00");
	failures += check_ulong_to_hex(2147483000, "0x7ffffd78");
	failures += check_ulong_to_hex(4294967295, "0xffffffff");

	check_ulong_to_hex_nothing_explodes();

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_ulong_to_hex, __FILE__)
