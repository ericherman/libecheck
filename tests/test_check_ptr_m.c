/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_ptr_m(void)
{
	struct eembed_log *orig = NULL;
	const char *strs[3];
	unsigned failures = 0;

	strs[0] = "contextual info";
	strs[1] = "foo";

	failures += check_ptr_m(strs[0], strs[0], "contextual info");

	orig = echeck_test_log_capture();
	if (0 == check_ptr_m(strs[0], strs[1], "contextual info")) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += echeck_test_err_log_contains(strs, 1);

	if (failures) {
		echeck_test_debug_print_failures(failures, "test_check_ptr_m");
	}
	return failures;
}

ECHECK_TEST_MAIN(test_check_ptr_m, __FILE__)
