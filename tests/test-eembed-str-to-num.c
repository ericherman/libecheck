/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-ulong-to-str.c */
/* Copyright (C) 2017-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"
#include <limits.h>

void test_str_to_long(const char *str, long expected)
{
	char *end = NULL;
	int64_t actual = eembed_str_to_long(str, &end);
	eembed_crash_if_false(end);
	eembed_crash_if_false(actual == expected);
}

void test_str_to_ulong(const char *str, unsigned long expected)
{
	char *end = NULL;
	uint64_t actual = eembed_str_to_ulong(str, &end);

	eembed_crash_if_false(end);
	eembed_crash_if_false(actual == expected);
}

unsigned int test_eembed_str_to_num(void)
{
	test_str_to_long("0", 0);
	test_str_to_ulong("0", 0);

	test_str_to_long("230", 230);
	test_str_to_ulong("230", 230);

	test_str_to_long("1234L", 1234);
	test_str_to_ulong("1234UL", 1234);

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_str_to_num)
