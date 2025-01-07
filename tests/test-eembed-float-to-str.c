/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-float-to-str.c */
/* Copyright (C) 2017-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"
#include <limits.h>
#include <float.h>

void test_float_to_str_nothing_explodes(void)
{
	char buf[30];
	double d = 1.0 / 16.0;

	eembed_float_to_str(NULL, 30, d);

	eembed_float_to_str(buf, 0, d);
	eembed_float_to_str(buf, 1, d);
	eembed_float_to_str(buf, 2, d);
	eembed_float_to_str(buf, 3, d);
	eembed_float_to_str(buf, 4, d);
	eembed_float_to_str(buf, 5, d);
	eembed_float_to_str(buf, 6, d);

	eembed_float_to_str(buf, 30, DBL_MAX);
	eembed_float_to_str(buf, 30, -DBL_MAX);
	eembed_float_to_str(buf, 30, DBL_MIN);
	eembed_float_to_str(buf, 30, -DBL_MIN);

	eembed_float_to_str(buf, 30, LDBL_MAX / 2);
	eembed_float_to_str(buf, 30, LDBL_MAX * 2);

	eembed_float_to_str(buf, 30, 0.000000001);

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	eembed_diy_float_to_str(NULL, 30, d);

	eembed_diy_float_to_str(buf, 0, d);
	eembed_diy_float_to_str(buf, 1, d);
	eembed_diy_float_to_str(buf, 2, d);
	eembed_diy_float_to_str(buf, 3, d);
	eembed_diy_float_to_str(buf, 4, d);
	eembed_diy_float_to_str(buf, 5, d);
	eembed_diy_float_to_str(buf, 6, d);

	eembed_diy_float_to_str(buf, 30, DBL_MAX);
	eembed_diy_float_to_str(buf, 30, -DBL_MAX);
	eembed_diy_float_to_str(buf, 30, DBL_MIN);
	eembed_diy_float_to_str(buf, 30, -DBL_MIN);

	eembed_diy_float_to_str(buf, 30, LDBL_MAX / 2);
	eembed_diy_float_to_str(buf, 30, LDBL_MAX * 2);

	eembed_diy_float_to_str(buf, 30, 0.000000001);
#endif
}

void assert_float_to_str(double d, const char *fltstr)
{
	char buf[30];
	char *rv;

	rv = eembed_float_to_str(buf, 30, d);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strstr(buf, fltstr) != NULL);

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	rv = eembed_diy_float_to_str(buf, 30, d);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strstr(buf, fltstr) != NULL);
#endif
}

void assert_float_to_str_if_not_enough_space(void)
{
	char buf[2];
	char *rv;

	rv = eembed_float_to_str(buf, 2, 2.0);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strcmp("2", buf) == 0);

	rv = eembed_float_to_str(buf, 2, -2.0);
	eembed_crash_if_false(rv == NULL);

	rv = eembed_float_to_str(buf, 2, FLT_MAX);
	eembed_crash_if_false(rv == NULL);

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	rv = eembed_diy_float_to_str(buf, 2, 2.0);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strcmp("2", buf) == 0);

	rv = eembed_diy_float_to_str(buf, 2, -2.0);
	eembed_crash_if_false(rv == NULL);

	rv = eembed_diy_float_to_str(buf, 2, FLT_MAX);
	eembed_crash_if_false(rv == NULL);
#endif
}

unsigned int test_eembed_float_to_str(void)
{
	static const double eembed_zero = 0.0;
	static const double eembed_nan = eembed_zero / eembed_zero;

	assert_float_to_str(eembed_zero, "0");
	assert_float_to_str(1.0 / 16.0, "0.0625");
	assert_float_to_str(-600.0 / 32, "-18.75");
	assert_float_to_str(1.2, "1.2");
	assert_float_to_str(-0.5, "-0.5");
	assert_float_to_str(-0.6, "-0.6");
	assert_float_to_str(eembed_nan, "nan");

	assert_float_to_str_if_not_enough_space();

	test_float_to_str_nothing_explodes();

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_float_to_str)
