/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-ulong-to-hex.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include <eembed.h>
#include <limits.h>

void test_ulong_to_hex_nothing_explodes(void)
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

	eembed_bytes_to_hex(buf, (sizeof(unsigned long) - 1),
			    bytes, sizeof(unsigned long));
}

void test_ulong_to_hex(unsigned long ul, const char *ulhex)
{
	char buf[30];
	char *rv;
	unsigned long rul = 0;
	char *endptr = NULL;

	rv = eembed_ulong_to_hex(buf, 30, ul);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strstr(buf, "0x") != NULL);
	eembed_crash_if_false(eembed_strstr(buf, ulhex) != NULL);

	rul = eembed_hex_to_ulong(ulhex, &endptr);
	eembed_crash_if_false(rul == ul);
	eembed_crash_if_false(endptr);
}

unsigned int test_eembed_ulong_to_hex(void)
{
	test_ulong_to_hex(0, "0x00");
	test_ulong_to_hex(2147483000, "7FFFFD78");
	test_ulong_to_hex(4294967295, "FFFFFFFF");
	eembed_crash_if_false(eembed_hex_to_ulong("ff", NULL) == 255);

	test_ulong_to_hex_nothing_explodes();

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_ulong_to_hex)
