/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2021 Eric Herman <eric@freesa.org> */

#include "echeck.h"

char *global_buf = NULL;
size_t global_buf_size = 0;

#if EEMBED_HOSTED
#include <stdarg.h>
#include <inttypes.h>
#include <stdio.h>

extern int (*eembed_system_fprintf)(FILE *stream, const char *format, ...);
extern int (*eembed_system_fflush)(FILE *stream);

extern void eembed_stream_eol(FILE *stream);

unsigned int *global_failures = NULL;

unsigned int flush_stdout_cnt = 0;
unsigned int flush_stderr_cnt = 0;

FILE *expect_stream = NULL;
FILE *actual_stream = NULL;

static int test_eembed_system_fprintf(FILE *stream, const char *format, ...)
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

static int test_eembed_system_fflush(FILE *stream)
{
	if (stream == stdout) {
		++flush_stdout_cnt;
	} else if (stream == stderr) {
		++flush_stderr_cnt;
	} else {
		fprintf(stderr, "%s:%d unknown stream: %p?\n",
			__FILE__, __LINE__, (void *)stream);
		++(*global_failures);
	}
	return 0;
}
#else /* EEMBED_HOSTED */

extern void eembed_no_op_print(const char *str);
extern void (*eembed_system_print)(const char *str);

static void test_eembed_system_print(const char *str)
{
	eembed_memset(global_buf, 0x00, global_buf_size);
	eembed_strncpy(global_buf, str, global_buf_size);
}

#endif /* EEMBED_HOSTED */

unsigned int test_eembed_print(void)
{
#if EEMBED_HOSTED
	int (*orig_fprintf)(FILE *stream, const char *format, ...) =
	    eembed_system_fprintf;
	int (*orig_fflush)(FILE *stream) = eembed_system_fflush;
#else
	void (*orig_system_print)(const char *str) = eembed_system_print;
#endif /* EEMBED_HOSTED */

	char buf[24];
	char *found = NULL;

	unsigned failures = 0;

	global_buf = buf;
	global_buf_size = 24;

#if EEMBED_HOSTED
	global_failures = &failures;
	eembed_system_fprintf = test_eembed_system_fprintf;
	eembed_system_fflush = test_eembed_system_fflush;

	flush_stdout_cnt = 0;
	flush_stderr_cnt = 0;
	expect_stream = stdout;
#else
	eembed_no_op_print("\n\n\n\n"
			   "===============================================\n"
			   __FILE__ " eembed_no_op_print()\n"
			   "ERROR! WE SHOULD NOT SEE OUTPUT!\n"
			   "===============================================\n"
			   "\n\n\n\n");
	eembed_system_print = test_eembed_system_print;
#endif /* EEMBED_HOSTED */

	buf[0] = '\0';
	eembed_system_print("foo");
	failures += check_str(buf, "foo");
#if EEMBED_HOSTED
	failures += check_unsigned_long(flush_stdout_cnt, 0);
	failures += check_ptr(expect_stream, actual_stream);
#endif /* EEMBED_HOSTED */

#if EEMBED_HOSTED
	expect_stream = stderr;
#endif /* EEMBED_HOSTED */
	buf[0] = '\0';
	eembed_err_log->append_s(eembed_err_log, "bar");
	failures += check_str(buf, "bar");
#if EEMBED_HOSTED
	failures += check_unsigned_long(flush_stdout_cnt, 1);
	failures += check_ptr(expect_stream, actual_stream);
#endif /* EEMBED_HOSTED */

	eembed_err_log->append_c(eembed_err_log, 'r');
	failures += check_str(buf, "r");

	eembed_err_log->append_ul(eembed_err_log, 7);
	failures += check_str(buf, "7");

	eembed_err_log->append_l(eembed_err_log, -23);
	failures += check_str(buf, "-23");

	eembed_err_log->append_f(eembed_err_log, 1.5);
	found = eembed_strstr(buf, "1.5");
	failures += check_ptr(found, buf);

	eembed_err_log->append_vp(eembed_err_log, NULL);
	found = eembed_strstr(buf, "0x00");
	if (!found) {
		found = eembed_strstr(buf, "nil");
	}
	if (!found) {
		found = eembed_strstr(buf, "null");
	}
	failures += check_ptr_not_null_m(found, buf);

	eembed_err_log->append_eol(eembed_err_log);
	failures += check_str(buf, "\n");
#if EEMBED_HOSTED
	failures += check_unsigned_long(flush_stderr_cnt, 1);
#endif /* EEMBED_HOSTED */

#if EEMBED_HOSTED
	expect_stream = stdout;
#endif /* EEMBED_HOSTED */
	eembed_system_printc('c');
	failures += check_str(buf, "c");
#if EEMBED_HOSTED
	failures += check_ptr(expect_stream, actual_stream);
#endif /* EEMBED_HOSTED */

	eembed_system_println();
	failures += check_str(buf, "\n");

#if EEMBED_HOSTED
	eembed_system_fprintf = orig_fprintf;
	eembed_system_fflush = orig_fflush;
#else
	eembed_system_print = orig_system_print;
#endif /* EEMBED_HOSTED */

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_print)
