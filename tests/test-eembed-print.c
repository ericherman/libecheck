/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include "eembed.h"

char *global_buf = NULL;
size_t global_buf_size = 0;

#if EEMBED_HOSTED
#include <stdarg.h>
#include <inttypes.h>
#include <stdio.h>

extern int (*eembed_fprintf)(FILE *stream, const char *format, ...);
extern int (*eembed_fflush)(FILE *stream);

extern void eembed_stream_eol(FILE *stream);

unsigned int flush_stdout_cnt = 0;
unsigned int flush_stderr_cnt = 0;

FILE *expect_stream = NULL;
FILE *actual_stream = NULL;

static int test_eembed_fprintf(FILE *stream, const char *format, ...)
{
	int rv = 0;
	va_list ap;

	actual_stream = stream;

	eembed_memset(global_buf, 0x00, global_buf_size);

	va_start(ap, format);
	rv = vsprintf(global_buf, format, ap);
	va_end(ap);

	return rv;
}

static int test_eembed_fflush(FILE *stream)
{
	if (stream == stdout) {
		++flush_stdout_cnt;
	} else if (stream == stderr) {
		++flush_stderr_cnt;
	} else {
		fprintf(stderr, "%s:%d unknown stream: %p?\n",
			__FILE__, __LINE__, (void *)stream);
		eembed_crash_if_false(1 == 0);
	}
	return 0;
}
#else /* EEMBED_HOSTED */

extern void eembed_no_op_append_s(struct eembed_log *log, const char *str);

static void test_eembed_global_append_s(struct eembed_log *log, const char *str)
{
	(void)log;
	eembed_memset(global_buf, 0x00, global_buf_size);
	eembed_strncpy(global_buf, str, global_buf_size);
}

#endif /* EEMBED_HOSTED */

unsigned int test_eembed_print(void)
{
#if EEMBED_HOSTED
	int (*orig_fprintf)(FILE *stream, const char *format, ...) =
	    eembed_fprintf;
	int (*orig_fflush)(FILE *stream) = eembed_fflush;
#else
	void (*orig_append_s)(struct eembed_log *log, const char *str) =
	    eembed_out_log->append_s;
#endif /* EEMBED_HOSTED */

	char buf[24];
	char *found = NULL;

	global_buf = buf;
	global_buf_size = 24;

#if EEMBED_HOSTED
	eembed_fprintf = test_eembed_fprintf;
	eembed_fflush = test_eembed_fflush;

	flush_stdout_cnt = 0;
	flush_stderr_cnt = 0;
	expect_stream = stdout;
#else
	eembed_no_op_append_s(eembed_out_log, "\n\n\n\n"
			      "=============================================\n"
			      __FILE__ " eembed_out_log->append_s\n"
			      "ERROR! WE SHOULD NOT SEE OUTPUT!\n"
			      "=============================================\n"
			      "\n\n\n\n");
	eembed_out_log->append_s = test_eembed_global_append_s;
#endif /* EEMBED_HOSTED */

	buf[0] = '\0';
	eembed_out_log->append_s(eembed_out_log, "foo");
	eembed_crash_if_false(eembed_strcmp(buf, "foo") == 0);
#if EEMBED_HOSTED
	eembed_crash_if_false(flush_stdout_cnt == 0);
	eembed_crash_if_false(expect_stream == actual_stream);
#endif /* EEMBED_HOSTED */

#if EEMBED_HOSTED
	expect_stream = stderr;
#endif /* EEMBED_HOSTED */
	buf[0] = '\0';
	eembed_err_log->append_s(eembed_err_log, "bar");
	eembed_crash_if_false(eembed_strcmp(buf, "bar") == 0);
#if EEMBED_HOSTED
	eembed_crash_if_false(flush_stdout_cnt == 1);
	eembed_crash_if_false(expect_stream == actual_stream);
#endif /* EEMBED_HOSTED */

	eembed_err_log->append_c(eembed_err_log, 'r');
	eembed_crash_if_false(eembed_strcmp(buf, "r") == 0);

	eembed_err_log->append_ul(eembed_err_log, 7);
	eembed_crash_if_false(eembed_strcmp(buf, "7") == 0);

	eembed_err_log->append_l(eembed_err_log, -23);
	eembed_crash_if_false(eembed_strcmp(buf, "-23") == 0);

	eembed_err_log->append_f(eembed_err_log, 1.5);
	found = eembed_strstr(buf, "1.5");
	eembed_crash_if_false(found == buf);

	eembed_err_log->append_vp(eembed_err_log, NULL);
	found = eembed_strstr(buf, "00");
	if (!found) {
		found = eembed_strstr(buf, "nil");
	}
	if (!found) {
		found = eembed_strstr(buf, "null");
	}
	eembed_crash_if_false(found != NULL);

	eembed_err_log->append_eol(eembed_err_log);
	eembed_crash_if_false(eembed_strcmp(buf, "\n") == 0);
#if EEMBED_HOSTED
	eembed_crash_if_false(flush_stderr_cnt == 1);
#endif /* EEMBED_HOSTED */

#if EEMBED_HOSTED
	expect_stream = stdout;
#endif /* EEMBED_HOSTED */
	eembed_out_log->append_c(eembed_out_log, 'c');
	eembed_crash_if_false(eembed_strcmp(buf, "c") == 0);
#if EEMBED_HOSTED
	eembed_crash_if_false(expect_stream == actual_stream);
#endif /* EEMBED_HOSTED */

	eembed_out_log->append_eol(eembed_out_log);
	eembed_crash_if_false(eembed_strcmp(buf, "\n") == 0);

#if EEMBED_HOSTED
	eembed_fprintf = orig_fprintf;
	eembed_fflush = orig_fflush;
#else
	eembed_out_log->append_s = orig_append_s;
#endif /* EEMBED_HOSTED */

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_print)
