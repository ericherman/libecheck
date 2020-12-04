/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

#if EEMBED_HOSTED

#include <stdio.h>
/* _POSIX_C_SOURCE >= 200809L || _GNU_SOURCE */
FILE *fmemopen(void *buf, size_t size, const char *mode);

int main(int argc, char *argv[])
{
	char logbuf[255];
	FILE *memlogfile = NULL;
	FILE *orig = NULL;
	unsigned failures = 0;

	(void)argc;
	(void)argv;

	eembed_memset(logbuf, 0x00, 255);

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

	failures += check_str_contains(logbuf, "memlogfile");
	failures += check_str_contains(logbuf, "0x4BADC0DE");
	failures += check_str_contains(logbuf, "4211");
	failures += check_str_contains(logbuf, "5.7");

	if (failures) {
		eembed_err_log->append_ul(eembed_err_log, failures);
		eembed_err_log->append_s(eembed_err_log, " failures in ");
		eembed_err_log->append_s(eembed_err_log, __FILE__);
		eembed_err_log->append_eol(eembed_err_log);
	}
	return check_status(failures);
}
#elif FAUX_FREESTANDING
int main(void)
{
	return 0;
}
#endif
