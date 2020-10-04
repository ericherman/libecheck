/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "echeck.h"
#include <string.h>

FILE *echeck_ensure_stream(FILE *stream)
{
	FILE *ec_stream;

	/* To avoid undefined behavior when writing to stderr, first flush
	 * stdout, thus ensuring stdout and stderr are "coordinated":
	 *
	 * "if two or more handles are used, and any one of them is a stream,
	 * the application shall ensure that their actions are coordinated as
	 * described below. If this is not done, the result is undefined."
	 *
	 * https://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_05_01
	 */
	ec_stream = stream ? stream : stderr;
	if (ec_stream == stderr) {
		fflush(stdout);
	}

	return ec_stream;
}

static void echeck_print_fail(FILE *err, const char *msg)
{
	fprintf(err, "%s", "FAIL:");
	if (msg) {
		fprintf(err, "%s", " ");
		fprintf(err, "%s", msg);
		fprintf(err, "%s", ":");
	}
}

static void echeck_print_err_location(FILE *err, const char *func,
				      const char *file, int line)
{
	fprintf(err, "%s", "[");
	if (func) {
		fprintf(err, "%s", func);
		fprintf(err, "%s", " at ");
	}
	fprintf(err, "%s", file);
	fprintf(err, "%s", ":");
	fprintf(err, "%d", line);
	fprintf(err, "%s", "]");
}

int echeck_char_m(FILE *err, const char *func, const char *file, int line,
		  char actual, char expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_stream(err);

	echeck_print_fail(err, msg);
	fprintf(err, "%s", " Expected '");
	if (expected) {
		fprintf(err, "%c", expected);
	} else {
		fprintf(err, "%s", "\\0");
	}
	fprintf(err, "%s", "' but was '");
	if (actual) {
		fprintf(err, "%c", actual);
	} else {
		fprintf(err, "%s", "\\0");
	}
	fprintf(err, "%s", "' ");
	echeck_print_err_location(err, func, file, line);
	fprintf(err, "%s", "\n");

	return 1;
}

int echeck_unsigned_long_m(FILE *err, const char *func, const char *file,
			   int line, unsigned long actual,
			   unsigned long expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_stream(err);

	echeck_print_fail(err, msg);
	fprintf(err, "%s", " Expected ");
	fprintf(err, "%lu", expected);
	fprintf(err, "%s", " but was ");
	fprintf(err, "%lu", actual);
	fprintf(err, "%s", " ");
	echeck_print_err_location(err, func, file, line);
	fprintf(err, "%s", "\n");

	return 1;
}

int echeck_long_m(FILE *err, const char *func, const char *file, int line,
		  long actual, long expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_stream(err);

	echeck_print_fail(err, msg);
	fprintf(err, "%s", " Expected ");
	fprintf(err, "%ld", expected);
	fprintf(err, "%s", " but was ");
	fprintf(err, "%ld", actual);
	fprintf(err, "%s", " ");
	echeck_print_err_location(err, func, file, line);
	fprintf(err, "%s", "\n");

	return 1;
}

int echeck_size_t_m(FILE *err, const char *func, const char *file, int line,
		    size_t actual, size_t expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_stream(err);

	echeck_print_fail(err, msg);
	fprintf(err, "%s", " Expected ");
	fprintf(err, "%lu", (unsigned long)expected);
	fprintf(err, "%s", " but was ");
	fprintf(err, "%lu", (unsigned long)actual);
	fprintf(err, "%s", " ");
	echeck_print_err_location(err, func, file, line);
	fprintf(err, "%s", "\n");

	return 1;
}

int echeck_str_m(FILE *err, const char *func, const char *file, int line,
		 const char *actual, const char *expected, const char *msg)
{
	if (actual == expected) {
		return 0;
	}
	if (actual != NULL && expected != NULL && strcmp(expected, actual) == 0) {
		return 0;
	}

	err = echeck_ensure_stream(err);

	echeck_print_fail(err, msg);
	fprintf(err, "%s", " Expected ");
	if (expected) {
		fprintf(err, "%s", "'");
		fprintf(err, "%s", expected);
		fprintf(err, "%s", "'");
	} else {
		fprintf(err, "%s", "(null)");
	}
	fprintf(err, "%s", " but was ");
	if (actual) {
		fprintf(err, "%s", "'");
		fprintf(err, "%s", actual);
		fprintf(err, "%s", "'");
	} else {
		fprintf(err, "%s", "(null)");
	}
	fprintf(err, "%s", " ");
	echeck_print_err_location(err, func, file, line);
	fprintf(err, "%s", "\n");

	return 1;
}

int echeck_byte_array_m(FILE *err, const char *func, const char *file, int line,
			const unsigned char *actual, size_t actual_len,
			const unsigned char *expected, size_t expected_len,
			const char *msg)
{

	size_t i;
	size_t min_max;

	int fail = 0;

	err = echeck_ensure_stream(err);
	if (actual_len != expected_len) {
		fprintf(err, "%s", "actual/expected length mis-match ");
		fprintf(err, "%lu", (unsigned long)actual_len);
		fprintf(err, "%s", " != ");
		fprintf(err, "%lu", (unsigned long)expected_len);
		fprintf(err, "%s", " ");
		echeck_print_err_location(err, func, file, line);
		fprintf(err, "%s", "\n");
		fail = 1;
	}
	min_max = actual_len < expected_len ? actual_len : expected_len;
	for (i = 0; i < min_max; i++) {
		if (actual[i] != expected[i]) {
			fprintf(err, "%s", "buffers differ at ");
			fprintf(err, "%lu", (unsigned long)i);
			fprintf(err, "%s", "\n");
			fail = 1;
		}
	}
	if (!fail) {
		return 0;
	}

	echeck_print_fail(err, msg);
	fprintf(err, "%s", " ");
	echeck_print_err_location(err, func, file, line);
	fprintf(err, "%s", "\n");

	fprintf(err, "%s", "index\tactual\texpected");
	fprintf(err, "%s", "\n");
	for (i = 0; i < actual_len || i < expected_len; i++) {
		fprintf(err, "%lu", (unsigned long)i);
		fprintf(err, "%s", "\t");
		if (i < actual_len) {
			fprintf(err, "%02x", actual[i]);
		}
		if (i < expected_len) {
			fprintf(err, "%s", "\t");
			fprintf(err, "%02x", expected[i]);
		}
		fprintf(err, "%s", "\n");
	}
	return 1;
}

#include <stdio.h>

int echeck_double_m(FILE *err, const char *func, const char *file, int line,
		    double actual, double expected, double epsilon,
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

	err = echeck_ensure_stream(err);

	echeck_print_fail(err, msg);
	fprintf(err, "%s", " Expected ");
	fprintf(err, "%f", expected);
	fprintf(err, "%s", "(+/- ");
	fprintf(err, "%f", epsilon);
	fprintf(err, "%s", ")");
	fprintf(err, "%s", " but was ");
	fprintf(err, "%f", actual);
	fprintf(err, "%s", " ");
	echeck_print_err_location(err, func, file, line);
	fprintf(err, "%s", "\n");

	return 1;
}

int echeck_ptr_m(FILE *err, const char *func, const char *file, int line,
		 const void *actual, const void *expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	echeck_print_fail(err, msg);

	fprintf(err, "%s", " Expected ");
	fprintf(err, "%p", expected);
	fprintf(err, "%s", " but was ");
	fprintf(err, "%p", actual);
	fprintf(err, "%s", " ");
	echeck_print_err_location(err, func, file, line);
	fprintf(err, "%s", "\n");

	return 1;
}

char echeck_status_m(FILE *err, const char *func, const char *file, int line,
		     int val, const char *msg)
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
	err = echeck_ensure_stream(err);
	if (msg) {
		fprintf(err, "%s", msg);
		fprintf(err, "%s", " ");
	}
	fprintf(err, "%d", val);
	fprintf(err, "%s", " capped at ");
	fprintf(err, "%d", (int)c);
	fprintf(err, "%s", " ");
	echeck_print_err_location(err, func, file, line);
	fprintf(err, "%s", "\n");

	return c;
}
