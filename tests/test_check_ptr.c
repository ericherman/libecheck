/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_ptr(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[2];
	unsigned failures = 0;

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
		echeck_test_debug_print_failures(failures, "test_check_ptr");
	}
	return failures;
}

#if ECHECK_HOSTED
int main(int argc, char *argv[])
{
	unsigned failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_ptr();

	if (failures) {
		echeck_test_debug_print_failures(failures, __FILE__);
	}
	return check_status(failures);
}
#endif
