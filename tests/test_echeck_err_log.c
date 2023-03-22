/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

void eembed_no_op_append_s(struct eembed_log *log, const char *str);

#if EEMBED_HOSTED

#include <stdio.h>
/* _POSIX_C_SOURCE >= 200809L || _GNU_SOURCE */
FILE *fmemopen(void *buf, size_t size, const char *mode);
#else
void eembed_test_hosted_buf_append_s(struct eembed_log *log, const char *str)
{
	struct eembed_str_buf *sbuf = NULL;
	size_t slen = 0;
	size_t used = 0;
	size_t left = 0;

	(void)log;

	if (eembed_err_log) {
		sbuf = (struct eembed_str_buf *)eembed_err_log->context;
	}
	if (sbuf) {
		used = eembed_strnlen(sbuf->buf, sbuf->size);
		left = sbuf->size - used;
		slen = eembed_strlen(str);
		if (slen < left) {
			eembed_strcat(sbuf->buf, str);
		}
	}
}
#endif

#if EEMBED_HOSTED
void *eembed_test_func(void *data)
{
	return data;
}
#endif

unsigned test_echeck_err_log(void)
{
	const size_t logbuf_size = 512;
	char logbuf[512];
#if EEMBED_HOSTED
	FILE *memlogfile;
	struct eembed_function_context mem_context = { NULL, eembed_test_func };
#else
	struct eembed_str_buf sbuf;
#endif
	void *orig = NULL;
	unsigned failures = 0;

#if !EEMBED_HOSTED
	eembed_out_log->append_s(eembed_out_log, __FILE__);
	eembed_out_log->append_s(eembed_out_log,
				 " WE SHOULD NOT SEE THIS OUTPUT!\n");
#endif

#if (0)
	eembed_system_printc('(');
	eembed_system_print("ECHECK_FUNC == ");
	eembed_system_print(ECHECK_FUNC);
	eembed_system_printc(')');
	eembed_system_println();
#endif

	eembed_memset(logbuf, 0x00, logbuf_size);

	orig = eembed_err_log->context;
#if EEMBED_HOSTED
	memlogfile = fmemopen(logbuf, logbuf_size, "w+");
	mem_context.data = memlogfile;
	eembed_err_log->context = &mem_context;
#else
	eembed_err_log->append_s = eembed_test_hosted_buf_append_s;
	sbuf.buf = logbuf;
	sbuf.size = logbuf_size;
	eembed_err_log->context = &sbuf;
#endif

	if (0 == check_ptr(logbuf, orig)) {
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

#if EEMBED_HOSTED
	fflush(memlogfile);
	fclose(memlogfile);
	memlogfile = NULL;
#else
	eembed_err_log->append_s = eembed_no_op_append_s;
#endif

	failures += check_str_contains(logbuf, "logbuf");
	failures += check_str_contains(logbuf, "0x4BADC0DE");
	failures += check_str_contains(logbuf, "4211");
	failures += check_str_contains(logbuf, "5.7");

	if (failures) {
		eembed_err_log->append_ul(eembed_err_log, failures);
		eembed_err_log->append_s(eembed_err_log, " failures in ");
		eembed_err_log->append_s(eembed_err_log, __FILE__);
		eembed_err_log->append_eol(eembed_err_log);
	}
	return failures;
}

ECHECK_TEST_MAIN(test_echeck_err_log)
