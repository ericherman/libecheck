/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

#include <stdio.h>

int test_check_unsigned_long(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[2];
	int failures = 0;

	strs[0] = "1";
	strs[1] = sizeof(size_t) == 8 ? "18446744073709551615" : "4294967295";

	failures += check_unsigned_long(5L, 5L);

	orig = echeck_test_log_capture();
	if (0 == check_unsigned_long(1L, -1L)) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += err_contains(strs, 2);

	if (failures) {
		fprintf(stderr, "%d failures in test_check_unsigned_long\n",
			failures);
	}
	return failures;
}

int main(int argc, char *argv[])
{
	int failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_unsigned_long();

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
