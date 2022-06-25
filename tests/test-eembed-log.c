/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020, 2021 Eric Herman <eric@freesa.org> */

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

	buf[0] = '\0';
	log = eembed_char_buf_log_init(NULL, &bctx, buf, buf_len);
	eembed_crash_if_false(log == NULL);

	log = eembed_char_buf_log_init(&llog, NULL, buf, buf_len);
	eembed_crash_if_false(log == NULL);

	log = eembed_char_buf_log_init(&llog, &bctx, NULL, buf_len);
	eembed_crash_if_false(log == NULL);

	buf[0] = '\0';
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
		eembed_err_log->append_s(eembed_err_log, "Expedcted '");
		eembed_err_log->append_s(eembed_err_log, expected);
		eembed_err_log->append_s(eembed_err_log, "' but was '");
		eembed_err_log->append_s(eembed_err_log, buf);
		eembed_err_log->append_s(eembed_err_log, "'");
		eembed_err_log->append_eol(eembed_err_log);
		eembed_err_log->append_s(eembed_err_log, "\tdiff of ");
		eembed_err_log->append_l(eembed_err_log, diff);
		eembed_err_log->append_eol(eembed_err_log);
		++failures;
	}

	eembed_memset(buf, 0x00, buf_len);
	log->append_f(log, -2.5);
	found = eembed_strstr(buf, "-2.5");
	if (!found) {
		eembed_err_log->append_s(eembed_err_log, "Did not find");
		eembed_err_log->append_s(eembed_err_log, " -2.5 in '");
		eembed_err_log->append_s(eembed_err_log, buf);
		eembed_err_log->append_s(eembed_err_log, "'");
		eembed_err_log->append_eol(eembed_err_log);
		++failures;
	}

	eembed_memset(buf, 0x00, buf_len);
	log->append_vp(log, NULL);
	found = eembed_strstr(buf, "0x00");
	if (!found) {
		found = eembed_strstr(buf, "null");
	}
	if (!found) {
		found = eembed_strstr(buf, "nil");
	}
	if (!found) {
		eembed_err_log->append_s(eembed_err_log, "Did not find");
		eembed_err_log->append_s(eembed_err_log, " (nil) in '");
		eembed_err_log->append_s(eembed_err_log, buf);
		eembed_err_log->append_s(eembed_err_log, "'");
		eembed_err_log->append_eol(eembed_err_log);
		++failures;
	}

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_log)
