/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

unsigned test_check_byte_array_m_1(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[3];
	unsigned failures = 0;

	unsigned char bytes_a[2] = { 0x00, 0xFF };
	unsigned char bytes_b[2] = { 0x00, 0xFF };
	unsigned char bytes_c[2] = { 0x13, 0x10 };

	strs[0] = "";
	strs[1] = "";
	strs[2] = "contextual info";

	failures +=
	    check_byte_array_m(bytes_a, 2, bytes_b, 2, "contextual info");

	orig = echeck_test_log_capture();
	if (0 == check_byte_array_m(bytes_b, 2, bytes_c, 2, "contextual info")) {
		failures++;
	}
	echeck_test_log_release(orig);
	failures += err_contains(strs, 3);

	if (failures) {
		echeck_test_debug_print_failures(failures,
						 "test_check_byte_array_m_1");
	}
	return failures;
}

unsigned test_check_byte_array_m_2(void)
{
	struct echeck_log *orig = NULL;
	const char *strs[2];
	unsigned failures = 0;

	unsigned char bytes_a[3] = { 0x13, 0x10, 0x00 };
	unsigned char bytes_b[4] = { 0x13, 0x10, 0x00, 0x7F };

	strs[0] = "length mis-match";
	strs[1] = "contextual";

	orig = echeck_test_log_capture();
	if (0 == check_byte_array_m(bytes_a, 3, bytes_b, 4, "contextual")) {
		failures++;
	}
	echeck_test_log_release(orig);

	failures += err_contains(strs, 2);
	if (failures) {
		echeck_test_debug_print_failures(failures,
						 "test_check_byte_array_m_2");
	}
	return failures;
}

unsigned test_check_byte_array_m(void)
{
	unsigned failures = 0;
	failures += test_check_byte_array_m_1();
	failures += test_check_byte_array_m_2();
	return failures;
}

#if ECHECK_HOSTED
int main(int argc, char *argv[])
{
	unsigned failures = 0;

	(void)argc;
	(void)argv;

	failures += test_check_byte_array_m();

	if (failures) {
		echeck_test_debug_print_failures(failures, __FILE__);
	}
	return check_status(failures);
}
#endif
