/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

#include <stdio.h>

int test_check_ptr(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[2];
	int failures = 0;

	strs[0] = "strs[0]";
	strs[1] = "Expected";	/* this is cheating */

	failures += check_ptr(strs[0], strs[0]);

	orig = echeck_test_log_capture();
	if (0 == check_ptr(strs[0], strs[1])) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += err_contains(strs, 2);

	if (failures) {
		fprintf(stderr, "%d failures in test_check_ptr\n", failures);
	}
	return failures;
}

int main(int argc, char *argv[])
{
	int failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_ptr();

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
