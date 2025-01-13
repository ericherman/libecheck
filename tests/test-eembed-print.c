/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2021-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

/* rather than actually logging, during this test, some globals will be changed
 * to write into a very small buffer which gets reset before each write, and
 * then the contents of which can used with a standard string compare. */
static char *global_buf = NULL;
static size_t global_buf_size = 0;

/* reset the globals before crashing, so crashing will be normal */
static void eembed_reset_global_logs(void);
#define reset_globals_and_crash_if_false(x) \
	if (!(x)) { \
		eembed_reset_global_logs(); \
		eembed_crash_if_false(x); \
	}

/* in the HOSTED case, we know the underlying implementation uses fprintf and
 * fflush; we can test all the various functions on the eembed_log struct */
#if EEMBED_HOSTED
#include <stdarg.h>
#include <inttypes.h>
#include <stdio.h>

extern int (*eembed_fprintf)(FILE *stream, const char *format, ...);
extern int (*eembed_fflush)(FILE *stream);

static int (*orig_fprintf)(FILE *stream, const char *format, ...) = NULL;
static int (*orig_fflush)(FILE *stream) = NULL;

static unsigned int flush_stdout_cnt = 0;
static unsigned int flush_stderr_cnt = 0;

static FILE *expect_stream = NULL;
static FILE *actual_stream = NULL;

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
	}
	reset_globals_and_crash_if_false(((stream == stdout)
					  || (stream == stderr)));
	return 0;
}

#else /* EEMBED_HOSTED */

/* in the FREESTANDING case, we know nothing about the underlying
 * implementation and so we we will swap the whole function in the string case
 * and not bother with all of the others because but this does not have the
 * same utility of a test as the HOSTED case. */

static void (*orig_out_append_s)(struct eembed_log *log, const char *str) =
    NULL;
static void (*orig_err_append_s)(struct eembed_log *log, const char *str) =
    NULL;

static void test_eembed_global_append_s(struct eembed_log *log, const char *str)
{
	(void)log;
	eembed_memset(global_buf, 0x00, global_buf_size);
	eembed_strncpy(global_buf, str, global_buf_size);
}

#endif /* EEMBED_HOSTED */

static void eembed_setup_global_logs(char *buf, size_t buf_size)
{
	global_buf = buf;
	global_buf_size = buf_size;

#if EEMBED_HOSTED
	orig_fprintf = eembed_fprintf;
	eembed_fprintf = test_eembed_fprintf;

	orig_fflush = eembed_fflush;
	eembed_fflush = test_eembed_fflush;

	flush_stdout_cnt = 0;
	flush_stderr_cnt = 0;
	expect_stream = stdout;
#else
	orig_out_append_s = eembed_out_log->append_s;
	eembed_out_log->append_s = test_eembed_global_append_s;

	orig_err_append_s = eembed_err_log->append_s;
	eembed_err_log->append_s = test_eembed_global_append_s;
#endif /* EEMBED_HOSTED */
}

static void eembed_reset_global_logs(void)
{
#if EEMBED_HOSTED
	if (orig_fprintf) {
		eembed_fprintf = orig_fprintf;
	}
	if (orig_fflush) {
		eembed_fflush = orig_fflush;
	}
#else
	if (orig_out_append_s) {
		eembed_out_log->append_s = orig_out_append_s;
	}
	if (orig_err_append_s) {
		eembed_err_log->append_s = orig_err_append_s;
	}
#endif /* EEMBED_HOSTED */
}

unsigned test_eembed_print(void)
{
	char buf[24];
	char *found = NULL;

	buf[0] = '\0';

	eembed_null_log->append_s(eembed_null_log, "\n\n\n\n"
				  "=============================================\n"
				  __FILE__ " eembed_out_log->append_s\n"
				  "ERROR! WE SHOULD NOT SEE OUTPUT!\n"
				  "=============================================\n"
				  "\n\n\n\n");

	eembed_setup_global_logs(buf, sizeof(buf));

	eembed_out_log->append_s(eembed_out_log, "foo");
	found = eembed_strstr(buf, "foo");
	reset_globals_and_crash_if_false(found == buf);
	reset_globals_and_crash_if_false(eembed_strcmp(buf, "foo") == 0);
#if EEMBED_HOSTED
	reset_globals_and_crash_if_false(flush_stdout_cnt == 0);
	reset_globals_and_crash_if_false(expect_stream == actual_stream);
#endif /* EEMBED_HOSTED */

#if EEMBED_HOSTED
	expect_stream = stderr;
#endif /* EEMBED_HOSTED */
	buf[0] = '\0';
	eembed_err_log->append_s(eembed_err_log, "bar");
	reset_globals_and_crash_if_false(eembed_strcmp(buf, "bar") == 0);

#if EEMBED_HOSTED
	reset_globals_and_crash_if_false(flush_stdout_cnt == 1);
	reset_globals_and_crash_if_false(expect_stream == actual_stream);

	eembed_err_log->append_c(eembed_err_log, 'r');
	reset_globals_and_crash_if_false(eembed_strcmp(buf, "r") == 0);

	eembed_err_log->append_ul(eembed_err_log, 7);
	reset_globals_and_crash_if_false(eembed_strcmp(buf, "7") == 0);

	eembed_err_log->append_l(eembed_err_log, -23);
	reset_globals_and_crash_if_false(eembed_strcmp(buf, "-23") == 0);

	eembed_err_log->append_f(eembed_err_log, 1.5);
	found = eembed_strstr(buf, "1.5");
	reset_globals_and_crash_if_false(found == buf);

	eembed_err_log->append_fd(eembed_err_log, 20.0 / 3.0, 3);
	found = eembed_strstr(buf, "6.667");
	if (!found) {
		found = eembed_strstr(buf, "6.666");
	}
	if (found != buf) {
		print_err_eol();
		print_err_s(__FILE__);
		print_err_s(" +");
		print_err_i(__LINE__);
		print_err_s(" :   buf '");
		print_err_s(buf);
		print_err_s("'");
		print_err_eol();
		print_err_s(__FILE__);
		print_err_s(" +");
		print_err_i(__LINE__);
		print_err_s(" : found '");
		print_err_s(found);
		print_err_s("'");
		print_err_eol();
	}

	reset_globals_and_crash_if_false(found == buf);
	reset_globals_and_crash_if_false(eembed_strlen(found) == 5);

	eembed_err_log->append_vp(eembed_err_log, NULL);
	found = eembed_strstr(buf, "00");
	if (!found) {
		found = eembed_strstr(buf, "nil");
	}
	if (!found) {
		found = eembed_strstr(buf, "null");
	}
	reset_globals_and_crash_if_false(found != NULL);

	eembed_err_log->append_eol(eembed_err_log);
	reset_globals_and_crash_if_false(eembed_strcmp(buf, "\n") == 0);
	reset_globals_and_crash_if_false(flush_stderr_cnt == 1);

	expect_stream = stdout;
	eembed_out_log->append_c(eembed_out_log, 'c');
	reset_globals_and_crash_if_false(eembed_strcmp(buf, "c") == 0);
	reset_globals_and_crash_if_false(expect_stream == actual_stream);

	eembed_out_log->append_eol(eembed_out_log);
	reset_globals_and_crash_if_false(eembed_strcmp(buf, "\n") == 0);
#endif /* EEMBED_HOSTED */

	eembed_reset_global_logs();

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_print)
