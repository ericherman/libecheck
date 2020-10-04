/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "echeck.h"

#ifndef ECHECK_HOSTED
/*
 __STDC_HOSTED__
 The integer constant 1 if the implementation is a hosted
 implementation or the integer constant 0 if it is not.

 C99 standard (section 6.10.8):
 http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf

 C++11 standard (section 16.8):
 http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf

 "The standard also defines two environments for programs, a
 freestanding environment, required of all implementations and which
 may not have library facilities beyond those required of
 freestanding implementations, where the handling of program startup
 and termination are implementation-defined; and a hosted
 environment, which is not required, in which all the library
 facilities are provided and startup is through a function int main
 (void) or int main (int, char *[]). An OS kernel is an example of a
 program running in a freestanding environment; a program using the
 facilities of an operating system is an example of a program
 running in a hosted environment."
 https://gcc.gnu.org/onlinedocs/gcc/Standards.html
*/
#ifdef __STDC_HOSTED__
#define ECHECK_HOSTED __STDC_HOSTED__
#else
/* guess? */
#define ECHECK_HOSTED 1
#endif
#endif

#if ECHECK_HOSTED
#include <stdio.h>

FILE *echeck_ensure_stream(void *context)
{
	FILE *stream = NULL;

	/* To avoid undefined behavior when writing to stderr, first flush
	 * stdout, thus ensuring stdout and stderr are "coordinated":
	 *
	 * "if two or more handles are used, and any one of them is a stream,
	 * the application shall ensure that their actions are coordinated as
	 * described below. If this is not done, the result is undefined."
	 *
	 * https://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_05_01
	 */
	stream = context ? (FILE *)context : stderr;
	if (stream == stderr) {
		fflush(stdout);
	}

	return stream;
}

void echeck_fprintf_append_s(struct echeck_log *log, const char *str)
{
	FILE *stream = echeck_ensure_stream(log->context);
	fprintf(stream, "%s", str);
}

void echeck_fprintf_append_ul(struct echeck_log *log, unsigned long ul)
{
	FILE *stream = echeck_ensure_stream(log->context);
	fprintf(stream, "%lu", ul);
}

void echeck_fprintf_append_z(struct echeck_log *log, size_t z)
{
	FILE *stream = echeck_ensure_stream(log->context);
	fprintf(stream, "%lu", (unsigned long)z);
}

void echeck_fprintf_append_l(struct echeck_log *log, long l)
{
	FILE *stream = echeck_ensure_stream(log->context);
	fprintf(stream, "%ld", l);
}

void echeck_fprintf_append_f(struct echeck_log *log, double f)
{
	FILE *stream = echeck_ensure_stream(log->context);
	fprintf(stream, "%g", f);
}

void echeck_fprintf_append_vp(struct echeck_log *log, const void *ptr)
{
	FILE *stream = echeck_ensure_stream(log->context);
	fprintf(stream, "%p", ptr);
}

void echeck_fprintf_append_eol(struct echeck_log *log)
{
	FILE *stream = echeck_ensure_stream(log->context);
	fprintf(stream, "\n");
}

struct echeck_log echeck_stderr_log = {
	NULL,
	echeck_fprintf_append_s,
	echeck_fprintf_append_ul,
	echeck_fprintf_append_z,
	echeck_fprintf_append_l,
	echeck_fprintf_append_f,
	echeck_fprintf_append_vp,
	echeck_fprintf_append_eol
};

struct echeck_log *echeck_default_log = &echeck_stderr_log;

#else /* ECHECK_HOSTED */

void echeck_noop_append_s(struct echeck_log *log, const char *str)
{
	(void)log;
	(void)str;
}

void echeck_noop_append_ul(struct echeck_log *log, unsigned long ul)
{
	(void)log;
	(void)ul;
}

void echeck_noop_append_z(struct echeck_log *log, size_t z)
{
	(void)log;
	(void)z;
}

void echeck_noop_append_l(struct echeck_log *log, long l)
{
	(void)log;
	(void)l;
}

void echeck_noop_append_f(struct echeck_log *log, double f)
{
	(void)log;
	(void)log;
}

void echeck_noop_append_vp(struct echeck_log *log, const void *ptr)
{
	(void)log;
	(void)ptr;
}

void echeck_noop_append_eol(struct echeck_log *log)
{
	(void)log;
}

struct echeck_log echeck_noop_log = {
	(void *)stderr,
	echeck_noop_append_s,
	echeck_noop_append_ul,
	echeck_noop_append_z,
	echeck_noop_append_l,
	echeck_noop_append_f,
	echeck_noop_append_vp,
	echeck_noop_append_eol
};

struct echeck_log *echeck_default_log = &echeck_noop_log;

#endif /* ECHECK_HOSTED */

struct echeck_log *echeck_ensure_log(struct echeck_log *err)
{
	if (err) {
		return err;
	}
	return echeck_default_log;
}

static void echeck_append_fail(struct echeck_log *err, const char *msg)
{
	err->append_s(err, "FAIL:");
	if (msg) {
		err->append_s(err, " ");
		err->append_s(err, msg);
		err->append_s(err, ":");
	}
}

static void echeck_append_func_file_line(struct echeck_log *err,
					 const char *func, const char *file,
					 int line)
{
	err->append_s(err, "[");
	if (func) {
		err->append_s(err, func);
		err->append_s(err, " at ");
	}
	err->append_s(err, file);
	err->append_s(err, ":");
	err->append_l(err, line);
	err->append_s(err, "]");
}

int echeck_char_m(struct echeck_log *err, const char *func, const char *file,
		  int line, char actual, char expected, const char *msg)
{
	char buf[2];
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected '");
	if (expected) {
		buf[0] = expected;
		buf[1] = '\0';
		err->append_s(err, buf);
	} else {
		err->append_s(err, "\\0");
	}
	err->append_s(err, "' but was '");
	if (actual) {
		buf[0] = actual;
		buf[1] = '\0';
		err->append_s(err, buf);
	} else {
		err->append_s(err, "\\0");
	}
	err->append_s(err, "' ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

int echeck_unsigned_long_m(struct echeck_log *err, const char *func,
			   const char *file, int line, unsigned long actual,
			   unsigned long expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_ul(err, expected);
	err->append_s(err, " but was ");
	err->append_ul(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

int echeck_long_m(struct echeck_log *err, const char *func, const char *file,
		  int line, long actual, long expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_l(err, expected);
	err->append_s(err, " but was ");
	err->append_l(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

int echeck_size_t_m(struct echeck_log *err, const char *func, const char *file,
		    int line, size_t actual, size_t expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_ul(err, (unsigned long)expected);
	err->append_s(err, " but was ");
	err->append_ul(err, (unsigned long)actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

int echeck_diy_strcmp(const char *s1, const char *s2)
{
	if (s1 == s2) {
		return 0;
	} else if (s1 == NULL) {
		return -1;
	} else if (s2 == NULL) {
		return 1;
	}

	for (; *s1 || *s2; ++s1, ++s2) {
		if (*s1 != *s2) {
			return *s1 - *s2;
		}
	}
	return *s1 - *s2;
}

int echeck_str_m(struct echeck_log *err, const char *func, const char *file,
		 int line, const char *actual, const char *expected,
		 const char *msg)
{
	if (actual == expected) {
		return 0;
	}
	if (actual != NULL && expected != NULL
	    && echeck_diy_strcmp(expected, actual) == 0) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	if (expected) {
		err->append_s(err, "'");
		err->append_s(err, expected);
		err->append_s(err, "'");
	} else {
		err->append_s(err, "(null)");
	}
	err->append_s(err, " but was ");
	if (actual) {
		err->append_s(err, "'");
		err->append_s(err, actual);
		err->append_s(err, "'");
	} else {
		err->append_s(err, "(null)");
	}
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

int echeck_byte_array_m(struct echeck_log *err, const char *func,
			const char *file, int line, const unsigned char *actual,
			size_t actual_len, const unsigned char *expected,
			size_t expected_len, const char *msg)
{

	size_t i = 0;
	size_t min_max = 0;
	int fail = 0;

	err = echeck_ensure_log(err);

	if (actual_len != expected_len) {
		err->append_s(err, "actual/expected length mis-match ");
		err->append_ul(err, (unsigned long)actual_len);
		err->append_s(err, " != ");
		err->append_ul(err, (unsigned long)expected_len);
		err->append_s(err, " ");
		echeck_append_func_file_line(err, func, file, line);
		err->append_eol(err);
		fail = 1;
	}
	min_max = actual_len < expected_len ? actual_len : expected_len;
	for (i = 0; i < min_max; i++) {
		if (actual[i] != expected[i]) {
			err->append_s(err, "buffers differ at ");
			err->append_ul(err, (unsigned long)i);
			err->append_eol(err);
			fail = 1;
		}
	}
	if (!fail) {
		return 0;
	}

	echeck_append_fail(err, msg);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	err->append_s(err, "index:\tactual\texpected");
	err->append_eol(err);
	for (i = 0; i < actual_len || i < expected_len; i++) {
		err->append_ul(err, i);
		err->append_s(err, ":\t");
		if (i < actual_len) {
			err->append_ul(err, actual[i]);
		}
		if (i < expected_len) {
			err->append_s(err, "\t");
			err->append_ul(err, expected[i]);
		}
		err->append_eol(err);
	}
	return 1;
}

int echeck_double_m(struct echeck_log *err, const char *func, const char *file,
		    int line, double actual, double expected, double epsilon,
		    const char *msg)
{
	double from, to, swap;

	if (actual == expected) {
		return 0;
	}

	epsilon = (epsilon < 0.0) ? -epsilon : epsilon;
	from = expected + epsilon;
	to = expected - epsilon;
	if (from > to) {
		swap = from;
		from = to;
		to = swap;
	}

	if (actual >= from && actual <= to) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_f(err, expected);
	err->append_s(err, "(+/- ");
	err->append_f(err, epsilon);
	err->append_s(err, ")");
	err->append_s(err, " but was ");
	err->append_f(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

int echeck_ptr_m(struct echeck_log *err, const char *func, const char *file,
		 int line, const void *actual, const void *expected,
		 const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_vp(err, expected);
	err->append_s(err, " but was ");
	err->append_vp(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

char echeck_status_m(struct echeck_log *err, const char *func, const char *file,
		     int line, int val, const char *msg)
{
	char c;

	if ((-128 <= val) && (val <= 127)) {
		c = (char)val;
		return val;
	}
	if (val < -128) {
		c = -128;
	} else {
		c = 127;
	}
	err = echeck_ensure_log(err);
	if (msg) {
		err->append_s(err, msg);
		err->append_s(err, " ");
	}
	err->append_l(err, val);
	err->append_s(err, " capped at ");
	err->append_l(err, c);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return c;
}
