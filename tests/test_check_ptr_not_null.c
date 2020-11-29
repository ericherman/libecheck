/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_ptr_not_null(void)
{
	struct eembed_log *orig = NULL;
	const char *str = NULL;
	const char *strs[1];
	unsigned failures = 0;

	str = "foo";

	failures += check_ptr_not_null(str);

	orig = echeck_test_log_capture();
	str = NULL;
	if (0 == check_ptr_not_null(str)) {
		failures++;
	}
	echeck_test_log_release(orig);
	str = "NULL";
	strs[0] = str;
	failures += echeck_test_err_log_contains(strs, 1);

	if (failures) {
		echeck_test_debug_print_failures(failures, "test_check_ptr");
	}
	return failures;
}

ECHECK_TEST_MAIN(test_check_ptr, __FILE__)
