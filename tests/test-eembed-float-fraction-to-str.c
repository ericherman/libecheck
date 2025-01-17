/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-float-to-str.c */
/* Copyright (C) 2017-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"
#include <limits.h>
#include <float.h>

void test_float_fraction_to_str_nothing_explodes(void)
{
	char buf[30];
	double f = 1.0 / 16.0;
	uint8_t d = 3;

	eembed_float_fraction_to_str(NULL, 30, f, d);

	eembed_float_fraction_to_str(buf, 0, f, d);
	eembed_float_fraction_to_str(buf, 1, f, d);
	eembed_float_fraction_to_str(buf, 2, f, d);
	eembed_float_fraction_to_str(buf, 3, f, d);
	eembed_float_fraction_to_str(buf, 4, f, d);
	eembed_float_fraction_to_str(buf, 5, f, d);
	eembed_float_fraction_to_str(buf, 6, f, d);

	eembed_float_fraction_to_str(buf, 30, DBL_MAX, d);
	eembed_float_fraction_to_str(buf, 30, -DBL_MAX, d);
	eembed_float_fraction_to_str(buf, 30, DBL_MIN, d);
	eembed_float_fraction_to_str(buf, 30, -DBL_MIN, d);

	eembed_float_fraction_to_str(buf, 30, LDBL_MAX / 2, d);
	eembed_float_fraction_to_str(buf, 30, LDBL_MAX * 2, d);

	eembed_float_fraction_to_str(buf, 30, 0.0001, d);
	eembed_float_fraction_to_str(buf, 30, 0.0001, 0);
	eembed_float_fraction_to_str(buf, 30, 0.00000000001, d);
	eembed_float_fraction_to_str(buf, 30, 0.00000000001, 0);
	eembed_float_fraction_to_str(buf, 30, ((double)UINT64_MAX), d);
	eembed_float_fraction_to_str(buf, 30, ((double)UINT64_MAX), 0);
	eembed_float_fraction_to_str(buf, 30, ((double)UINT64_MAX) + 0.9, d);
	eembed_float_fraction_to_str(buf, 30, ((double)UINT64_MAX) + 0.9, 0);

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	eembed_diy_float_fraction_to_str(NULL, 30, f, d);

	eembed_diy_float_fraction_to_str(buf, 0, f, d);
	eembed_diy_float_fraction_to_str(buf, 1, f, d);
	eembed_diy_float_fraction_to_str(buf, 2, f, d);
	eembed_diy_float_fraction_to_str(buf, 3, f, d);
	eembed_diy_float_fraction_to_str(buf, 4, f, d);
	eembed_diy_float_fraction_to_str(buf, 5, f, d);
	eembed_diy_float_fraction_to_str(buf, 6, f, d);

	eembed_diy_float_fraction_to_str(buf, 30, DBL_MAX, d);
	eembed_diy_float_fraction_to_str(buf, 30, -DBL_MAX, d);
	eembed_diy_float_fraction_to_str(buf, 30, DBL_MIN, d);
	eembed_diy_float_fraction_to_str(buf, 30, -DBL_MIN, d);

	eembed_diy_float_fraction_to_str(buf, 30, LDBL_MAX / 2, d);
	eembed_diy_float_fraction_to_str(buf, 30, LDBL_MAX * 2, d);

	eembed_diy_float_fraction_to_str(buf, 30, 0.0001, d);
	eembed_diy_float_fraction_to_str(buf, 30, 0.0001, 0);
	eembed_diy_float_fraction_to_str(buf, 30, 0.00000000001, d);
	eembed_diy_float_fraction_to_str(buf, 30, 0.00000000001, 0);
	eembed_diy_float_fraction_to_str(buf, 30, ((double)UINT64_MAX), d);
	eembed_diy_float_fraction_to_str(buf, 30, ((double)UINT64_MAX), 0);
	eembed_diy_float_fraction_to_str(buf, 30, ((double)UINT64_MAX) + 0.9,
					 d);
	eembed_diy_float_fraction_to_str(buf, 30, ((double)UINT64_MAX) + 0.9,
					 0);
#endif
}

void assert_float_fraction_to_str(int line, double f, uint8_t d, const char *in,
				  const char *expect, const char *expect2)
{
	char buf[30] = { '\0' };
	char *rv = NULL;

	rv = eembed_float_fraction_to_str(buf, 30, f, d);
	eembed_crash_if_false(rv == buf);
	rv = eembed_strstr(buf, expect);
	if (!rv && expect2) {
		rv = eembed_strstr(buf, expect2);
	}
	if (!rv) {
		print_err_s(__FILE__);
		print_err_s(" +");
		print_err_i(line);
		print_err_s(" : actual =");
		print_err_s(" eembed_float_fraction_to_str");
		print_err_s("(buf, 30, ");
		print_err_s(in);
		print_err_s(", ");
		print_err_u(d);
		print_err_s(");");
		print_err_eol();
		print_err_s("actual: '");
		print_err_s(buf);
		print_err_c('\'');
		print_err_eol();
		print_err_s("expect: '");
		print_err_s(expect);
		print_err_c('\'');
		print_err_eol();
	}
	eembed_crash_if_false(rv);

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	rv = eembed_diy_float_fraction_to_str(buf, 30, f, d);
	eembed_crash_if_false(rv == buf);
	rv = eembed_strstr(buf, expect);
	if (!rv && expect2) {
		rv = eembed_strstr(buf, expect2);
	}
	if (!rv) {
		print_err_s(__FILE__);
		print_err_s(" +");
		print_err_i(line);
		print_err_s(" : actual =");
		print_err_s(" eembed_diy_float_fraction_to_str");
		print_err_s("(buf, 30, ");
		print_err_s(in);
		print_err_s(", ");
		print_err_u(d);
		print_err_s(");");
		print_err_eol();
		print_err_s("actual: '");
		print_err_s(buf);
		print_err_c('\'');
		print_err_eol();
		print_err_s("expect: '");
		print_err_s(expect);
		print_err_c('\'');
		print_err_eol();
	}
	eembed_crash_if_false(rv);
#endif
}

#define Assert_float_fraction_to_str2(f, d, in, expect, expect2) \
	assert_float_fraction_to_str(__LINE__, f, d, in, expect, expect2)

#define Assert_float_fraction_to_str(f, d, in, expect) \
	assert_float_fraction_to_str(__LINE__, f, d, in, expect, NULL)

void assert_float_fraction_to_str_if_not_enough_space(void)
{
	char buf[2] = { '\0', '\0' };
	char *rv = NULL;

	rv = eembed_float_fraction_to_str(buf, 2, 2.0, 1);
	eembed_crash_if_false(eembed_strcmp("2", buf) == 0);

	rv = eembed_float_fraction_to_str(buf, 2, -2.0, 1);
	eembed_crash_if_false(rv == NULL);

	rv = eembed_float_fraction_to_str(buf, 2, FLT_MAX, 255);
	eembed_crash_if_false(rv == NULL);

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
	rv = eembed_diy_float_fraction_to_str(buf, 2, 2.0, 1);
	eembed_crash_if_false(rv == buf);
	eembed_crash_if_false(eembed_strcmp("2", buf) == 0);

	rv = eembed_diy_float_fraction_to_str(buf, 2, -2.0, 1);
	eembed_crash_if_false(rv == NULL);

	rv = eembed_diy_float_fraction_to_str(buf, 2, FLT_MAX, 255);
	eembed_crash_if_false(rv == NULL);
#endif
}

unsigned int test_eembed_float_fraction_to_str(void)
{
	static const double eembed_zero = 0.0;
	static const double eembed_nan = eembed_zero / eembed_zero;

	Assert_float_fraction_to_str(eembed_zero, 0, "0.0", "0");
	Assert_float_fraction_to_str(eembed_zero, 3, "0.0", "0.000");

	Assert_float_fraction_to_str2(1.0 / 16.0, 0, "1.0 / 16.0", "0", "wee");
	Assert_float_fraction_to_str(1.0 / 16.0, 1, "1.0 / 16.0", "0.1");
	Assert_float_fraction_to_str(1.0 / 16.0, 2, "1.0 / 16.0", "0.06");
	Assert_float_fraction_to_str(1.0 / 16.0, 4, "1.0 / 16.0", "0.0625");
	Assert_float_fraction_to_str(1.0 / 16.0, 5, "1.0 / 16.0", "0.06250");
	Assert_float_fraction_to_str(1.0 / 16.0, 10, "1.0 / 16.0",
				     "0.0625000000");

	Assert_float_fraction_to_str(-600.0 / 32, 2, "-600.0 / 32", "-18.75");
	Assert_float_fraction_to_str(1.2, 1, "1.2", "1.2");
	Assert_float_fraction_to_str(-0.5, 1, "-0.5", "-0.5");
	Assert_float_fraction_to_str(-0.6, 1, "-0.6", "-0.6");
	Assert_float_fraction_to_str(eembed_nan, 10, "0.0/0.0", "nan");

	assert_float_fraction_to_str_if_not_enough_space();

	test_float_fraction_to_str_nothing_explodes();

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_float_fraction_to_str)
