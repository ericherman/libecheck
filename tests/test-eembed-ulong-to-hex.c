/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-ulong-to-hex.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"
#include <limits.h>

void check_ulong_to_hex_nothing_explodes(void)
{
	char buf[30];
	unsigned long ulong_max = ULONG_MAX;
	unsigned char *bytes;

	eembed_ulong_to_hex(buf, 3, ulong_max);
	eembed_ulong_to_hex(buf, 0, ulong_max);
	eembed_ulong_to_hex(NULL, 30, ulong_max);
	eembed_ulong_to_hex(buf, 30, ulong_max);

	bytes = (unsigned char *)&ulong_max;
	eembed_bytes_to_hex(buf, 30, bytes, sizeof(unsigned long));
}

unsigned check_ulong_to_hex(unsigned long ul, const char *ulhex)
{
	unsigned failures = 0;
	char buf[30];
	char *rv;
	unsigned long rul = 0;
	char *endptr = NULL;

	rv = eembed_ulong_to_hex(buf, 30, ul);
	failures += check_ptr_m(rv, buf, ulhex);
	failures += check_str_contains(buf, "0x");
	failures += check_str_contains(buf, ulhex);

	rul = eembed_hex_to_ulong(ulhex, &endptr);
	failures += check_unsigned_long(rul, ul);
	failures += check_ptr_not_null(endptr);

	return failures;
}

unsigned int test_eembed_ulong_to_hex(void)
{
	unsigned failures = 0;

	failures += check_ulong_to_hex(0, "0x00");
	failures += check_ulong_to_hex(2147483000, "7FFFFD78");
	failures += check_ulong_to_hex(4294967295, "FFFFFFFF");
	failures += check_unsigned_long(eembed_hex_to_ulong("ff", NULL), 255);

	check_ulong_to_hex_nothing_explodes();

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_ulong_to_hex)
