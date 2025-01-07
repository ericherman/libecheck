/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016-2025 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_check_double(void)
{
	struct eembed_log *orig = eembed_err_log;
	struct eembed_str_buf log_ctx;
	struct eembed_log buf_log;
	const size_t mem_buf_len = 1024;
	char mem_buf[1024];

	const char *strs[4];
	unsigned failures = 0;

	strs[0] = "8.8";
	strs[1] = "-8.8";
	strs[2] = "9.001";
	strs[3] = "9.002";

	failures += check_double(1.0, 1.0, 0.00001);
	failures += check_double(2.00002, 2.0, 0.001);
	failures += check_double(3.0003, 3.0004, 0.01);
	failures += check_double(-4.0001, -4.0002, 0.01);
	failures += check_double(-5.0002, -5.0001, 0.01);
	failures += check_double(0.00006, 0.0, 0.001);
	failures += check_double(0.0, 0.00007, 0.001);

	eembed_memset(mem_buf, 0x00, mem_buf_len);
	eembed_err_log =
	    eembed_char_buf_log_init(&buf_log, &log_ctx, mem_buf, mem_buf_len);
	if (0 == check_double(8.8, -8.8, 0.00001)) {
		failures++;
	}
	if (0 == check_double(9.001, 9.002, 0.00001)) {
		failures++;
	}
	eembed_err_log = orig;
	failures += check_str_contains_all(mem_buf, strs, 2);

	failures += check_double(-0.0001, 0.0001, 0.1);
	failures += check_double(0.0002, -0.0002, 0.1);

	failures += check_double_scaled_epsilon(0.0002, 0.0002);

	return failures;
}

ECHECK_TEST_MAIN(test_check_double)
