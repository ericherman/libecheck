/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

#include <stdio.h>

int test_check_str_null_ptr(void)
{
	struct echeck_log *orig = NULL;
	int failures = 0;
	const char *strs[2];

	strs[0] = "not_null_second";
	strs[1] = "not_null_first";

	failures += check_str(NULL, NULL);

	orig = echeck_test_log_capture();
	if (0 == check_str(NULL, strs[0])) {
		failures++;
	}
	if (0 == check_str(strs[1], NULL)) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += err_contains(strs, 2);

	if (failures) {
		fprintf(stderr, "%d failures in test_check_str\n", failures);
	}
	return failures;
}

int main(int argc, char *argv[])
{
	int failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_str_null_ptr();

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
