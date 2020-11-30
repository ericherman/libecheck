/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strlen.c */
/* Copyright (C) 2016, 2020 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_eembed_strlen(void)
{
	unsigned failures = 0;

	failures += check_size_t(eembed_strlen(""), 0);
	failures += check_size_t(eembed_strlen("1"), 1);
	failures += check_size_t(eembed_strlen("12"), 2);
	failures += check_size_t(eembed_strlen("123"), 3);
	failures += check_size_t(eembed_strlen("123456789"), 9);

	failures += check_size_t(eembed_strnlen("", 3), 0);
	failures += check_size_t(eembed_strnlen("1", 3), 1);
	failures += check_size_t(eembed_strnlen("12", 3), 2);
	failures += check_size_t(eembed_strnlen("123456789", 10), 9);
	failures += check_size_t(eembed_strnlen("123456789", 4), 4);

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_strlen, __FILE__)
