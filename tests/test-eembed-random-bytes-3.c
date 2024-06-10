/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020-2024 Eric Herman <eric@freesa.org> */

#include "eembed.h"

ssize_t eembed_diy_getrandom(void *buf, size_t bufsize, unsigned int flags);

/* if we're on an 8-bit CPU, reduce the time spent forever in the tests */
#ifndef Eembed_test_min_period
#define Eembed_test_min_period (sizeof(size_t) < 4 ? 1024 : 32768)
#endif

unsigned int test_eembed_diy_getrandom(void)
{
	unsigned int flags = 0;
	const size_t buf_len = 32;
	unsigned char buf1[32];
	unsigned char buf2[32];
	unsigned char buf3[64];

	size_t hexbuf_len = (2 + (2 * 32) + 1);
	char hexbuf[(2 + (2 * 32) + 1)];

	const size_t msg_len = 80 + (3 * 64);
	char msg[80 + (3 * 64)];

	int err = 0;
	size_t i = 0;
	size_t diff12 = 0;
	size_t diff13 = 0;
	size_t diff23 = 0;
	size_t same = 0;
	size_t offset = 0;
	size_t period = 0;
	int difference = 0;

	eembed_memset(msg, 0x00, msg_len);
	eembed_memset(buf1, 0x00, buf_len);
	eembed_memset(buf2, 0x00, buf_len);
	eembed_memset(buf3, 0x00, buf_len);

	eembed_strcat(msg, "buffers:\n");

	err = eembed_diy_getrandom(buf1, buf_len, flags);
	eembed_bytes_to_hex(hexbuf, hexbuf_len, buf1, buf_len);
	eembed_crash_if_false(err == 0);
	eembed_strcat(msg, "buf1: ");
	eembed_strcat(msg, hexbuf);
	eembed_strcat(msg, "\n");

	err = eembed_diy_getrandom(buf2, buf_len, flags);
	eembed_bytes_to_hex(hexbuf, hexbuf_len, buf2, buf_len);
	eembed_crash_if_false(err == 0);
	eembed_strcat(msg, "buf2: ");
	eembed_strcat(msg, hexbuf);
	eembed_strcat(msg, "\n");

	err = eembed_diy_getrandom(buf3, buf_len, flags);
	eembed_bytes_to_hex(hexbuf, hexbuf_len, buf3, buf_len);
	eembed_crash_if_false(err == 0);
	eembed_strcat(msg, "buf3: ");
	eembed_strcat(msg, hexbuf);
	eembed_strcat(msg, "\n");

	for (i = 0; i < buf_len; ++i) {
		if (buf1[i] != buf2[i]) {
			++diff12;
		}
		if (buf1[i] != buf3[i]) {
			++diff13;
		}
		if (buf2[i] != buf3[i]) {
			++diff23;
		}
	}

	eembed_crash_if_false((diff12 > 0 ? 1 : 0) == 1);
	eembed_crash_if_false((diff13 > 0 ? 1 : 0) == 1);
	eembed_crash_if_false((diff23 > 0 ? 1 : 0) == 1);

	eembed_err_log->append_s(eembed_err_log, msg);

	/* next we will test that the period is very long */
	/* we will assert that the buf3 is at least twice the buf_len */
	eembed_assert(sizeof(buf3) >= (2 * buf_len));

	eembed_diy_getrandom(buf1, buf_len, flags);
	eembed_diy_getrandom(buf3, buf_len, flags);
	eembed_diy_getrandom(buf3 + buf_len, buf_len, flags);
	same = 0;
	for (period = 1; !same && period < Eembed_test_min_period; ++period) {
		eembed_crash_if_false(err == 0);
		for (offset = 0; !same && offset < buf_len; ++offset) {
			difference = 0;
			for (i = 0; !difference && i < buf_len; ++i) {
				if (buf1[i] != buf3[offset + i]) {
					difference = 1;
				}
			}
			if (!difference) {
				same = period;
			}
		}
		/* shift buf_len to the left */
		eembed_memmove(buf3, buf3 + buf_len, buf_len);
		/* fetch buf_len more bytes */
		eembed_diy_getrandom(buf3 + buf_len, buf_len, flags);
	}
	eembed_crash_if_false(same == 0);

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_diy_getrandom)
