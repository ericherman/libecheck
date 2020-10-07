/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_double(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[4];
	unsigned failures = 0;

	strs[0] = "8.8";
	strs[1] = "-8.8";
	strs[2] = "9.001";
	strs[3] = "9.002";

	failures += check_double(1.0, 1.0, 0.00001);
	failures += check_double(2.00002, 2.0, 0.001);
	failures += check_double(3.0003, 3.0004, 0.01);
	failures += check_double(-4.0001, -4.0002, 0.01);
	failures += check_double(-5.0002, -5.0001, 0.01);
	failures += check_double(0.00006, 0.0, 0.001);
	failures += check_double(0.0, 0.00007, 0.001);

	orig = echeck_test_log_capture();
	if (0 == check_double(8.8, -8.8, 0.00001)) {
		failures++;
	}
	if (0 == check_double(9.001, 9.002, 0.00001)) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += echeck_test_err_log_contains(strs, 2);

	failures += check_double(-0.0001, 0.0001, 0.1);
	failures += check_double(0.0002, -0.0002, 0.1);

	failures += check_double_scaled_epsilon(0.0002, 0.0002);

	if (failures) {
		echeck_test_debug_print_failures(failures, "test_check_double");
	}
	return failures;
}

#if ECHECK_HOSTED
int main(int argc, char *argv[])
{
	unsigned failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_double();

	if (failures) {
		echeck_test_debug_print_failures(failures, __FILE__);
	}
	return check_status(failures);
}
#endif
