/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strlen.c */
/* Copyright (C) 2016-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

unsigned test_eembed_strlen_func(size_t (*strlen_func)(const char *s),
				 size_t(*strnlen_func)(const char *s,
						       size_t max)
    )
{
	eembed_crash_if_false(strlen_func("") == 0);
	eembed_crash_if_false(strlen_func("1") == 1);
	eembed_crash_if_false(strlen_func("12") == 2);
	eembed_crash_if_false(strlen_func("123") == 3);
	eembed_crash_if_false(strlen_func("123456789") == 9);

	eembed_crash_if_false(strnlen_func("", 3) == 0);
	eembed_crash_if_false(strnlen_func("1", 3) == 1);
	eembed_crash_if_false(strnlen_func("12", 3) == 2);
	eembed_crash_if_false(strnlen_func("123456789", 10) == 9);
	eembed_crash_if_false(strnlen_func("123456789", 4) == 4);

	return 0;
}

unsigned test_eembed_strlen(void)
{
	test_eembed_strlen_func(eembed_strlen, eembed_strnlen);
	test_eembed_strlen_func(eembed_diy_strlen, eembed_diy_strnlen);
	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_strlen)
