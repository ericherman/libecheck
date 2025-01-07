/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016-2025 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_check_byte_array(void)
{
	struct eembed_log *orig = eembed_err_log;
	struct eembed_str_buf log_ctx;
	struct eembed_log buf_log;
	const size_t mem_buf_len = 1024;
	char mem_buf[1024];

	const char *strs[3];
	unsigned failures = 0;

	unsigned char bytes_a[2] = { 0x00, 0xFF };
	unsigned char bytes_b[2] = { 0x00, 0xFF };
	unsigned char bytes_c[2] = { 0x13, 0x10 };

	strs[0] = "bytes_b";
	strs[1] = "FAIL";
	strs[2] = "differ";

	failures += check_byte_array(bytes_a, 2, bytes_b, 2);

	eembed_memset(mem_buf, 0x00, mem_buf_len);
	eembed_err_log =
	    eembed_char_buf_log_init(&buf_log, &log_ctx, mem_buf, mem_buf_len);
	if (0 == check_byte_array(bytes_b, 2, bytes_c, 2)) {
		failures++;
	}
	eembed_err_log = orig;

	failures += check_str_contains_all(mem_buf, strs, 3);

	return failures;
}

ECHECK_TEST_MAIN(test_check_byte_array)
