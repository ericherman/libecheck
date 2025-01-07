/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/random.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

int eembed_system_getrandom_bytes(unsigned char *buf, size_t buf_size);

extern ssize_t (*eembed_system_getrandom)(void *buf, size_t buflen,
					  unsigned int flags);

unsigned char *fake_random_source = NULL;
size_t fake_random_size = 0;
size_t fake_random_pos = 0;
ssize_t fake_getrandom(void *buf, size_t buflen, unsigned int flags)
{
	size_t size = (fake_random_size - fake_random_pos);
	unsigned char *source = fake_random_source + fake_random_pos;

	(void)flags;

	if (buflen > size) {
		errno = EAGAIN;
		return -1;
	}

	eembed_memcpy(buf, source, buflen);
	fake_random_pos += buflen;
	return buflen;
}

unsigned int test_sys_random_bytes(void)
{
	ssize_t (*orig_sys_getrandom)(void *buf, size_t buflen,
				      unsigned int flags) =
	    eembed_system_getrandom;

	struct eembed_log *orig_err_log = eembed_err_log;

	struct eembed_str_buf log_ctx;
	struct eembed_log buf_log;
	const size_t buf_size = 1000;
	char buf[1000];

	const size_t faux_rand_size = 32;
	unsigned char faux_rand_bytes[32];

	int err = 0;
	const size_t bytes_buf_size = 32;
	unsigned char bytes_buf[32];
	size_t i;

	for (i = 0; i < faux_rand_size; ++i) {
		faux_rand_bytes[i] = (unsigned char)i;
	}
	fake_random_source = faux_rand_bytes;
	fake_random_size = faux_rand_size;
	fake_random_pos = 0;

	eembed_memset(buf, 0x00, buf_size);
	eembed_char_buf_log_init(&buf_log, &log_ctx, buf, buf_size);

	eembed_system_getrandom = fake_getrandom;

	/* first read succeeds */

	eembed_memset(buf, 0x00, buf_size);
	eembed_err_log = &buf_log;
	err = eembed_system_getrandom_bytes(bytes_buf, bytes_buf_size);
	eembed_err_log = orig_err_log;
	eembed_crash_if_false(err == 0);

	/* second read fails */
	eembed_memset(buf, 0x00, buf_size);
	eembed_err_log = &buf_log;
	err = eembed_system_getrandom_bytes(bytes_buf, bytes_buf_size);
	eembed_err_log = orig_err_log;
	eembed_crash_if_false(err != 0);
	eembed_crash_if_false(eembed_strstr(buf, "getrandom() failed"));

	eembed_err_log = orig_err_log;
	eembed_system_getrandom = orig_sys_getrandom;

	return 0;
}
#else
unsigned int test_sys_random_bytes(void)
{
	return 0;
}
#endif

EEMBED_FUNC_MAIN(test_sys_random_bytes)
