/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "echeck.h"

struct eembed_log *echeck_ensure_log(struct eembed_log *err)
{
	if (err) {
		return err;
	}
	return eembed_err_log;
}

static void echeck_append_fail(struct eembed_log *err, const char *msg)
{
	err->append_s(err, "FAIL:");
	if (msg) {
		err->append_s(err, " ");
		err->append_s(err, msg);
		err->append_s(err, ":");
	}
}

static void echeck_append_func_file_line(struct eembed_log *err,
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

unsigned char echeck_char_m(struct eembed_log *err, const char *func,
			    const char *file, int line, char actual,
			    char expected, const char *msg)
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

unsigned char echeck_unsigned_long_m(struct eembed_log *err, const char *func,
				     const char *file, int line,
				     unsigned long actual,
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

unsigned char echeck_long_m(struct eembed_log *err, const char *func,
			    const char *file, int line, long actual,
			    long expected, const char *msg)
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

unsigned char echeck_size_t_m(struct eembed_log *err, const char *func,
			      const char *file, int line, size_t actual,
			      size_t expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}

	err = echeck_ensure_log(err);

	echeck_append_fail(err, msg);
	err->append_s(err, " Expected ");
	err->append_z(err, expected);
	err->append_s(err, " but was ");
	err->append_z(err, actual);
	err->append_s(err, " ");
	echeck_append_func_file_line(err, func, file, line);
	err->append_eol(err);

	return 1;
}

unsigned char echeck_str_m(struct eembed_log *err, const char *func,
			   const char *file, int line, const char *actual,
			   const char *expected, const char *msg)
{
	if (actual == expected) {
		return 0;
	}
	if (actual != NULL && expected != NULL
	    && eembed_strcmp(expected, actual) == 0) {
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

unsigned char echeck_byte_array_m(struct eembed_log *err, const char *func,
				  const char *file, int line,
				  const unsigned char *actual,
				  size_t actual_len,
				  const unsigned char *expected,
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

unsigned char echeck_double_m(struct eembed_log *err, const char *func,
			      const char *file, int line, double actual,
			      double expected, double epsilon, const char *msg)
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

unsigned char echeck_ptr_m(struct eembed_log *err, const char *func,
			   const char *file, int line, const void *actual,
			   const void *expected, const char *msg)
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

char echeck_status_m(struct eembed_log *err, const char *func, const char *file,
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
