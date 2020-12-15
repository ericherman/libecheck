/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-float-to-str.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"
#include <limits.h>

void check_float_to_str_nothing_explodes(void)
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
}

unsigned check_float_to_str(double d, const char *fltstr)
{
	unsigned failures = 0;
	char buf[30];
	char *rv;

	rv = eembed_float_to_str(buf, 30, d);
	failures += check_ptr_m(rv, buf, fltstr);
	failures += check_str_contains(buf, fltstr);
	return failures;
}

unsigned int test_eembed_float_to_str(void)
{
	unsigned failures = 0;
	static const double eembed_zero = 0.0;
	static const double eembed_nan = eembed_zero / eembed_zero;

	failures += check_float_to_str(eembed_zero, "0");
	failures += check_float_to_str(1.0 / 16.0, "0.0625");
	failures += check_float_to_str(-600.0 / 32, "-18.75");
	failures += check_float_to_str(1.2, "1.2");
	failures += check_float_to_str(-0.5, "-0.5");
	failures += check_float_to_str(-0.6, "-0.6");
	failures += check_float_to_str(eembed_nan, "nan");

	check_float_to_str_nothing_explodes();

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_float_to_str)
