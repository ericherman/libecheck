/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

unsigned int test_eembed_log(void)
{
	size_t buf_len = 250;
	char buf[250];
	struct eembed_str_buf bctx;
	struct eembed_log llog;
	struct eembed_log *log = NULL;
	int diff = 0;
	int failures = 0;
	const char *expected = NULL;
	char *found = NULL;
	size_t i = 0;

	buf[0] = '\0';
	log = eembed_char_buf_log_init(NULL, &bctx, buf, buf_len);
	eembed_crash_if_false(log == NULL);

	log = eembed_char_buf_log_init(&llog, NULL, buf, buf_len);
	eembed_crash_if_false(log == NULL);

	log = eembed_char_buf_log_init(&llog, &bctx, NULL, buf_len);
	eembed_crash_if_false(log == NULL);

	buf[0] = '\0';
	log = eembed_char_buf_log_init(&llog, &bctx, buf, 0);
	if (log) {
		++failures;
	}
	log = &llog;
	log->append_s(log, "foo ");

	log = eembed_char_buf_log_init(&llog, &bctx, buf, buf_len);
	if (!log) {
		++failures;
		return 0;
	}

	log->append_s(log, "foo ");
	log->append_ul(log, 4);
	log->append_s(log, " bar");
	log->append_c(log, ' ');
	log->append_c(log, 'z');
	log->append_ul(log, 23);
	log->append_l(log, -2);
	log->append_ul(log, 4);
	log->append_eol(log);
	expected = "foo 4 bar z23-24\n";

	diff = eembed_strncmp(expected, buf, buf_len);
	if (diff) {
		print_err_s("Expedcted '");
		print_err_s(expected);
		print_err_s("' but was '");
		print_err_s(buf);
		print_err_s("'");
		print_err_eol();
		print_err_s("\tdiff of ");
		print_err_i(diff);
		print_err_eol();
		++failures;
	}

	eembed_memset(buf, 0x00, buf_len);
	log->append_f(log, -2.5);
	found = eembed_strstr(buf, "-2.5");
	if (!found) {
		print_err_s("Did not find");
		print_err_s(" -2.5 in '");
		print_err_s(buf);
		print_err_s("'");
		print_err_eol();
		++failures;
	}

	eembed_memset(buf, 0x00, buf_len);
	log->append_fd(log, -3.5, 3);
	found = eembed_strstr(buf, "-3.500");
	if (!found) {
		print_err_s("Did not find");
		print_err_s(" -3.500 in '");
		print_err_s(buf);
		print_err_s("'");
		print_err_eol();
		++failures;
	}

	eembed_memset(buf, 0x00, buf_len);
	log->append_fd(log, 20.0 / 3.0, 3);
	found = eembed_strstr(buf, "6.667");
	if (!found) {
		print_err_s("Did not find");
		print_err_s(" 6.667 in '");
		print_err_s(buf);
		print_err_s("'");
		print_err_eol();
		++failures;
	}

	eembed_memset(buf, 0x00, buf_len);
	log->append_vp(log, NULL);
	found = eembed_strstr(buf, "00");
	if (!found) {
		found = eembed_strstr(buf, "null");
	}
	if (!found) {
		found = eembed_strstr(buf, "nil");
	}
	if (!found) {
		print_err_s("Did not find");
		print_err_s(" (nil) in '");
		print_err_s(buf);
		print_err_s("'");
		print_err_eol();
		++failures;
	}

	for (i = 0; i < 10; ++i) {
		log->append_s(log, "this will run out of space but not crash");
	}

	eembed_null_log->append_c(eembed_null_log, 'c');
	eembed_null_log->append_s(eembed_null_log, "str");
	eembed_null_log->append_ul(eembed_null_log, 4);
	eembed_null_log->append_l(eembed_null_log, -2);
	eembed_null_log->append_f(eembed_null_log, 4.0);
	eembed_null_log->append_fd(eembed_null_log, 20.0 / 3.0, 2);
	eembed_null_log->append_vp(eembed_null_log, NULL);
	eembed_null_log->append_eol(eembed_null_log);

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_log)
