/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_char(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[2];
	unsigned failures = 0;

	strs[0] = "a";
	strs[1] = "b";

	failures += check_char('a', 'a');

	orig = echeck_test_log_capture();
	if (0 == check_char('a', 'b')) {
		failures++;
	}
	echeck_test_log_release(orig);

	failures += err_contains(strs, 2);
	if (failures) {
		echeck_test_debug_print_failures(failures, "test_check_char");
	}
	return failures;
}

#if ECHECK_HOSTED
int main(int argc, char *argv[])
{
	unsigned failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_char();

	if (failures) {
		echeck_test_debug_print_failures(failures, __FILE__);
	}
	return check_status(failures);
}
#endif
