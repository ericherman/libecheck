/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016-2025 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_check_size_t(void)
{
	struct eembed_log *orig = eembed_err_log;
	struct eembed_str_buf log_ctx;
	struct eembed_log buf_log;
	const size_t mem_buf_len = 1024;
	char mem_buf[1024];

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
		orig->append_s(orig, "sizeof(size_t) == ");
		orig->append_ul(orig, sizeof(size_t));
		orig->append_s(orig, " not supported");
		orig->append_eol(orig);
		return 1;
	}

	failures += check_size_t(five, five);

	eembed_memset(mem_buf, 0x00, mem_buf_len);
	eembed_err_log =
	    eembed_char_buf_log_init(&buf_log, &log_ctx, mem_buf, mem_buf_len);
	if (0 == check_size_t(one, big)) {
		failures++;
	}
	eembed_err_log = orig;
	failures += check_str_contains_all(mem_buf, strs, 2);

	return failures;
}

ECHECK_TEST_MAIN(test_check_size_t)
