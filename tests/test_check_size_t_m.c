/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

#include <stdio.h>

int test_check_size_t_m(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[3];
	int failures = 0;
	size_t five = 5;
	size_t six = 6;

	strs[0] = "5";
	strs[1] = "6";
	strs[2] = "contextual info";

	failures += check_size_t_m(five, five, "contextual info");

	orig = echeck_test_log_capture();
	if (0 == check_size_t_m(five, six, "contextual info")) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += err_contains(strs, 3);

	if (failures) {
		fprintf(stderr, "%d failures in test_check_size_t_m\n",
			failures);
	}
	return failures;
}

int main(int argc, char *argv[])
{
	int failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_size_t_m();

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
