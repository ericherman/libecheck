/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

#if EEMBED_HOSTED || FAUX_FREESTANDING

#include <stdio.h>
/* _POSIX_C_SOURCE >= 200809L || _GNU_SOURCE */
FILE *fmemopen(void *buf, size_t size, const char *mode);

int main(int argc, char *argv[])
{
	char logbuf[255];
	FILE *memlogfile = NULL;
	FILE *orig = NULL;
	unsigned failures = 0;

	fprintf(stderr, "ECHECK_FUNC: '%s'\n", (char *)ECHECK_FUNC);

	(void)argc;
	(void)argv;

	memlogfile = fmemopen(logbuf, 255, "w+");

	orig = eembed_err_log->context;
	eembed_err_log->context = memlogfile;

	if (0 == check_ptr(memlogfile, orig)) {
		++failures;
	}
	if (0 == check_size_t(0x4BADC0DE, 0x00)) {
		++failures;
	}
	if (0 == check_unsigned_long(4211, 12345)) {
		++failures;
	}
	if (0 == check_double_scaled_epsilon(5.7, 13.17)) {
		++failures;
	}

	eembed_err_log->context = orig;

	fflush(memlogfile);

	echeck_test_buf_contains(logbuf, "memlogfile");
	echeck_test_buf_contains(logbuf, "0x4BADC0DE");
	echeck_test_buf_contains(logbuf, "4211");
	echeck_test_buf_contains(logbuf, "5.7");

	if (failures) {
		echeck_test_debug_print_failures(failures, __FILE__);
	}
	return check_status(failures);
}
#endif
