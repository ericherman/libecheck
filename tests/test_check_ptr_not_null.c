/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_check_ptr_not_null(void)
{
	struct eembed_log *orig = eembed_err_log;
	struct eembed_str_buf log_ctx;
	struct eembed_log buf_log;
	const size_t mem_buf_len = 1024;
	char mem_buf[1024];

	const char *str = NULL;
	const char *strs[1];
	unsigned failures = 0;

	str = "foo";

	failures += check_ptr_not_null(str);

	eembed_memset(mem_buf, 0x00, mem_buf_len);
	eembed_err_log =
	    eembed_char_buf_log_init(&buf_log, &log_ctx, mem_buf, mem_buf_len);
	str = NULL;
	if (0 == check_ptr_not_null(str)) {
		failures++;
	}
	eembed_err_log = orig;
	str = "NULL";
	strs[0] = str;
	failures += check_str_contains_all(mem_buf, strs, 1);

	return failures;
}

ECHECK_TEST_MAIN(test_check_ptr)
