/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strlen.c */
/* Copyright (C) 2016, 2020 Eric Herman <eric@freesa.org> */

#include <eembed.h>

unsigned test_eembed_strlen(void)
{
	eembed_crash_if_false(eembed_strlen("") == 0);
	eembed_crash_if_false(eembed_strlen("1") == 1);
	eembed_crash_if_false(eembed_strlen("12") == 2);
	eembed_crash_if_false(eembed_strlen("123") == 3);
	eembed_crash_if_false(eembed_strlen("123456789") == 9);

	eembed_crash_if_false(eembed_strnlen("", 3) == 0);
	eembed_crash_if_false(eembed_strnlen("1", 3) == 1);
	eembed_crash_if_false(eembed_strnlen("12", 3) == 2);
	eembed_crash_if_false(eembed_strnlen("123456789", 10) == 9);
	eembed_crash_if_false(eembed_strnlen("123456789", 4) == 4);

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_strlen)
