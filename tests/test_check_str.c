/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_check_str(void)
{
	struct eembed_log *orig = eembed_err_log;
	struct eembed_str_buf log_ctx;
	struct eembed_log buf_log;
	const size_t mem_buf_len = 1024;
	char mem_buf[1024];
	const size_t mem_buf2_len = 500;
	char mem_buf2[500];

	const char *strs[2];
	unsigned failures = 0;

	strs[0] = "same";
	strs[1] = "different";

	failures += check_str(NULL, NULL);
	failures += check_str_contains(NULL, NULL);

	failures += check_str("same", "same");
	failures += check_str(strs[0], strs[0]);
	failures += check_str_contains(strs[0], strs[0]);

	eembed_memset(mem_buf, 0x00, mem_buf_len);
	eembed_err_log =
	    eembed_char_buf_log_init(&buf_log, &log_ctx, mem_buf, mem_buf_len);
	if (0 == check_str("same", "different")) {
		failures++;
	}
	eembed_err_log = orig;
	failures += check_str_contains_all(mem_buf, strs, 2);

	/* let's confirm that check_str_contains_all does what we expect */
	eembed_memset(mem_buf2, 0x00, mem_buf2_len);
	eembed_err_log =
	    eembed_char_buf_log_init(&buf_log, &log_ctx, mem_buf2,
				     mem_buf2_len);

	strs[0] = "BOGUS not in mem_buf";
	if (!check_str_contains_all(mem_buf, strs, 2)) {
		++failures;
	}

	eembed_memset(mem_buf2, 0x00, mem_buf2_len);
	strs[0] = NULL;
	if (!check_str_contains_all(mem_buf, strs, 2)) {
		++failures;
	}

	eembed_memset(mem_buf2, 0x00, mem_buf2_len);
	strs[0] = "same";
	if (!check_str_contains_all(NULL, strs, 2)) {
		++failures;
	}

	eembed_err_log = orig;
	return failures;
}

ECHECK_TEST_MAIN(test_check_str)
