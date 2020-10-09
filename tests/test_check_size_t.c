/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_size_t(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[2];
	unsigned failures = 0;
	size_t one = 1;
	size_t big = -1;
	size_t five = 5;

	strs[0] = "1";
	strs[1] = "";

	switch (sizeof(size_t)) {
	case 8:
		strs[1] = "18446744073709551615";
		break;
	case 4:
		strs[1] = "4294967295";
		break;
	case 2:
		strs[1] = "65535";
		break;
	default:
		echeck_test_debug_prints("sizeof(size_t) == ");
		echeck_test_debug_printz(sizeof(size_t));
		echeck_test_debug_prints(" not supported");
		echeck_test_debug_printeol();
		return 1;
	}

	failures += check_size_t(five, five);

	orig = echeck_test_log_capture();
	if (0 == check_size_t(one, big)) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += echeck_test_err_log_contains(strs, 2);

	if (failures) {
		echeck_test_debug_print_failures(failures, "test_check_size_t");
	}
	return failures;
}

#if ECHECK_HOSTED
int main(int argc, char *argv[])
{
	unsigned failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_size_t();

	if (failures) {
		echeck_test_debug_print_failures(failures, __FILE__);
	}
	return check_status(failures);
}
#endif
