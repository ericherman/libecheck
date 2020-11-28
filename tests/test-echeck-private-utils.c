/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

#include <limits.h>

#define echeck_test_mem_log_len 1024
char echeck_test_mem_log[echeck_test_mem_log_len];
struct eembed_str_buf echeck_test_buf_context;
struct eembed_log echeck_test_buf_log;

struct eembed_log *echeck_test_log_capture(void)
{
	struct eembed_log *orig = eembed_err_log;
	echeck_test_mem_log[0] = '\0';
	eembed_err_log =
	    eembed_char_buf_log_init(&echeck_test_buf_log,
				     &echeck_test_buf_context,
				     echeck_test_mem_log,
				     echeck_test_mem_log_len);
	return orig;
}

void echeck_test_log_release(struct eembed_log *orig)
{
	eembed_err_log = orig;
}

int echeck_test_buf_contains(const char *log, const char *expected)
{
	if (eembed_strstr(log, expected)) {
		return 1;
	}
	eembed_system_print("'");
	eembed_system_print(expected);
	eembed_system_print("' not in '");
	eembed_system_print(log);
	eembed_system_print("'");
	eembed_system_println();
	return 0;
}

int echeck_test_err_log_contains(const char *expected[], size_t expected_len)
{
	size_t i;
	unsigned int msg_found;

	msg_found = 0;
	for (i = 0; i < expected_len; i++) {
		msg_found +=
		    echeck_test_buf_contains(echeck_test_mem_log, expected[i]);
	}
	return expected_len - msg_found;
}

void echeck_test_debug_print_failures(unsigned failures, const char *where)
{
	char buf[25];

	buf[0] = '\0';

	eembed_ulong_to_str(buf, 25, failures);
	eembed_system_print(buf);
	eembed_system_print(" failures in ");
	eembed_system_print(where);
	eembed_system_println();
}

#if FAUX_FREESTANDING
#include <stdio.h>
void eembed_faux_freestanding_system_print(const char *str)
{
	printf(str);
}
#endif
