/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_unsigned_int(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[2];
	unsigned failures = 0;
	char buf[40];

	buf[0] = '\0';
	strs[0] = "1";
	strs[1] = buf;

	echeck_test_ul_to_str(buf, 40, (unsigned long)-1);

	failures += check_unsigned_int(5, 5);

	orig = echeck_test_log_capture();
	if (0 == check_unsigned_int(1, -1)) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += err_contains(strs, 2);

	if (failures) {
		echeck_test_debug_print_failures(failures,
						 "test_check_unsigned_int");
	}
	return failures;
}

#if ECHECK_HOSTED
int main(int argc, char *argv[])
{
	unsigned failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_unsigned_int();

	if (failures) {
		echeck_test_debug_print_failures(failures, __FILE__);
	}
	return check_status(failures);
}
#endif
