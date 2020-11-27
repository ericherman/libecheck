/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_byte_array(void)
{
	struct eembed_log *orig = NULL;
	const char *strs[3];
	unsigned failures = 0;

	unsigned char bytes_a[2] = { 0x00, 0xFF };
	unsigned char bytes_b[2] = { 0x00, 0xFF };
	unsigned char bytes_c[2] = { 0x13, 0x10 };

	strs[0] = "bytes_b";
	strs[1] = "FAIL";
	strs[2] = "differ";

	failures += check_byte_array(bytes_a, 2, bytes_b, 2);

	orig = echeck_test_log_capture();
	if (0 == check_byte_array(bytes_b, 2, bytes_c, 2)) {
		failures++;
	}
	echeck_test_log_release(orig);

	failures += echeck_test_err_log_contains(strs, 3);
	if (failures) {
		echeck_test_debug_print_failures(failures,
						 "test_check_byte_array");
	}
	return failures;
}

ECHECK_TEST_MAIN(test_check_byte_array, __FILE__)
