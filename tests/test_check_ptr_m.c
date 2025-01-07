/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016-2025 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_check_ptr_m(void)
{
	struct eembed_log *orig = eembed_err_log;
	struct eembed_str_buf log_ctx;
	struct eembed_log buf_log;
	const size_t mem_buf_len = 1024;
	char mem_buf[1024];

	const char *strs[3];
	unsigned failures = 0;

	strs[0] = "contextual info";
	strs[1] = "foo";

	failures += check_ptr_m(strs[0], strs[0], "contextual info");

	eembed_memset(mem_buf, 0x00, mem_buf_len);
	eembed_err_log =
	    eembed_char_buf_log_init(&buf_log, &log_ctx, mem_buf, mem_buf_len);
	if (0 == check_ptr_m(strs[0], strs[1], "contextual info")) {
		failures++;
	}
	eembed_err_log = orig;
	failures += check_str_contains_all(mem_buf, strs, 1);

	return failures;
}

ECHECK_TEST_MAIN(test_check_ptr_m)
