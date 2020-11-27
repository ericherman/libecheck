/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_str(void)
{
	struct eembed_log *orig = NULL;
	const char *strs[2];
	unsigned failures = 0;

	strs[0] = "same";
	strs[1] = "different";

	failures += check_str("same", "same");

	orig = echeck_test_log_capture();
	if (0 == check_str("same", "different")) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += echeck_test_err_log_contains(strs, 2);

	if (failures) {
		echeck_test_debug_print_failures(failures, "test_check_str");
	}
	return failures;
}

ECHECK_TEST_MAIN(test_check_str, __FILE__)
