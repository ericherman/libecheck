/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_str_m(void)
{
	struct eembed_log *orig = NULL;
	const char *strs[3];
	unsigned failures = 0;

	strs[0] = "weirdly";
	strs[1] = "different";
	strs[2] = "contextual info";

	failures += check_str_m("same", "same", "contextual info");

	orig = echeck_test_log_capture();
	if (0 == check_str_m("weirdly", "different", "contextual info")) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += echeck_test_err_log_contains(strs, 3);

	if (failures) {
		echeck_test_debug_print_failures(failures, "test_check_str_m");
	}
	return failures;
}

ECHECK_TEST_MAIN(test_check_str_m, __FILE__)
