/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_eembed_alloca_freea(void)
{
	char *message = NULL;
	unsigned failures = 0;
	const char *expect = "hello, world";

	message = (char *)eembed_alloca(16);
	failures += check_ptr_not_null(message);

	eembed_strcpy(message, expect);
	failures += check_str(message, expect);

	eembed_freea(message);

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_alloca_freea)
