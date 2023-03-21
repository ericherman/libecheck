/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-strcat.c */
/* Copyright (C) 2016, 2020 Eric Herman <eric@freesa.org> */

#include "eembed.h"

unsigned test_eembed_strcat(void)
{
	char actual[80];
	char *rv = NULL;
	const char *expect = NULL;

	actual[0] = '\0';
	expect = "foo bar";
	rv = eembed_strcat(actual, "foo");
	eembed_crash_if_false(rv == actual);
	rv = eembed_strcat(actual, " bar");
	eembed_crash_if_false(rv == actual);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);

	rv = eembed_strncat(actual, " whiz bang pow", 10);
	eembed_crash_if_false(rv == actual);
	expect = "foo bar whiz bang";
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);


#if (!EEMBED_HOSTED)
	rv = eembed_strcat(actual, NULL);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);

	rv = eembed_strcat(NULL, " whiz bang pow");
	eembed_crash_if_false(rv == NULL);

	rv = eembed_strncat(actual, NULL, 10);
	eembed_crash_if_false(eembed_strcmp(actual, expect) == 0);

	rv = eembed_strncat(NULL, " whiz bang pow", 0);
	eembed_crash_if_false(rv == NULL);
#endif

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_strcat)
