/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020, 2021 Eric Herman <eric@freesa.org> */

#include "eembed.h"

#if EEMBED_HOSTED
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/random.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

int eembed_dev_urandom_bytes(unsigned char *buf, size_t len);

extern ssize_t (*eembed_system_read)(int fd, void *buf, size_t count);
extern int (*eembed_system_close)(int fd);
extern int (*eembed_system_open)(const char *pathname, int flags, ...);
extern int (*eembed_system_ioctl)(int fd, unsigned long request, ...);

const char *actual_pathname = NULL;
int actual_flags = 0;
int urandom_fd = 0;
static int fake_open(const char *pathname, int flags, ...)
{
	actual_pathname = pathname;
	actual_flags = flags;
	return urandom_fd;
}

int actual_fd = 0;
unsigned long actual_request = 0;
int ioctl_entropy = INT_MAX / 4;
int ioctl_rv = 0;
int fake_ioctl(int fd, unsigned long request, ...)
{
	int *entropy = NULL;
	va_list ap;

	va_start(ap, request);
	entropy = va_arg(ap, int *);
	va_end(ap);

	actual_fd = fd;
	actual_request = request;
	*entropy = ioctl_entropy;
	return ioctl_rv;
}

unsigned read_fails = 0;
int read_really_broken = 0;
int actual_fd2 = 0;
size_t actual_count = 0;
size_t fill_amount = 100;
unsigned read_success = 0;
static ssize_t fake_read(int fd, void *vbuf, size_t count)
{
	size_t i = 0;
	char *buf = (char *)vbuf;
	actual_fd2 = fd;
	actual_count = count;

	if (read_fails || read_really_broken) {
		--read_fails;
		if (read_really_broken) {
			errno = EBADF;
		} else {
			errno = EINTR;
		}
		return -1;
	}

	for (i = 0; i < count && i < (read_success * fill_amount); ++i) {
		buf[i] = (uint8_t)i;
	}

	++read_success;
	return i;
}

int close_rv = 0;
int actual_fd3 = 0;
static int fake_close(int fd)
{
	actual_fd3 = fd;
	return close_rv;
}

unsigned int test_dev_urandom_bytes(void)
{
	ssize_t (*orig_system_read)(int fd, void *buf, size_t count) =
	    eembed_system_read;
	int (*orig_system_close)(int fd) = eembed_system_close;
	int (*orig_system_open)(const char *pathname, int flags, ...) =
	    eembed_system_open;
	int (*orig_system_ioctl)(int fd, unsigned long request, ...) =
	    eembed_system_ioctl;
	struct eembed_log *orig_err_log = eembed_err_log;

	struct eembed_str_buf log_ctx;
	struct eembed_log buf_log;
	const size_t buf_size = 1000;
	char buf[1000];

	const size_t bytes_buf_size = 32;
	unsigned char bytes_buf[32];
	int err = 0;

	eembed_memset(buf, 0x00, buf_size);
	eembed_char_buf_log_init(&buf_log, &log_ctx, buf, buf_size);

	eembed_system_open = fake_open;
	eembed_system_read = fake_read;
	eembed_system_ioctl = fake_ioctl;
	eembed_system_close = fake_close;

	/* fake open() fails and returns a -1 */
	urandom_fd = -1;
	eembed_memset(buf, 0x00, buf_size);
	eembed_err_log = &buf_log;
	err = eembed_dev_urandom_bytes(bytes_buf, bytes_buf_size);
	eembed_err_log = orig_err_log;
	eembed_crash_if_false((err == 0 ? 0 : 1) == 1);
	eembed_crash_if_false(eembed_strcmp(actual_pathname, "/dev/urandom") ==
			      0);
	eembed_crash_if_false(actual_flags == O_RDONLY);
	eembed_crash_if_false(eembed_strstr(buf, "open"));
	eembed_crash_if_false(eembed_strstr(buf, "fail"));
	urandom_fd = 0;

	/* fake ioctl() fails and returns a -1, as does close */
	ioctl_rv = -1;
	close_rv = -1;
	eembed_memset(buf, 0x00, buf_size);
	eembed_err_log = &buf_log;
	err = eembed_dev_urandom_bytes(bytes_buf, bytes_buf_size);
	eembed_err_log = orig_err_log;
	eembed_crash_if_false((err == 0 ? 0 : 1) == 1);
	eembed_crash_if_false(eembed_strstr(buf, "ioctl"));
	eembed_crash_if_false(eembed_strstr(buf, "fail"));
	ioctl_rv = 0;
	close_rv = 0;

	/* fake ioctl() has very low entropy */
	ioctl_entropy = 2;
	eembed_memset(buf, 0x00, buf_size);
	eembed_err_log = &buf_log;
	err = eembed_dev_urandom_bytes(bytes_buf, bytes_buf_size);
	eembed_err_log = orig_err_log;
	eembed_crash_if_false((err == 0 ? 0 : 1) == 1);
	eembed_crash_if_false(eembed_strstr(buf, "entropy"));
	ioctl_entropy = INT_MAX / 4;

	/* fake read() is really failing badly */
	read_fails = 1;
	read_really_broken = 1;
	eembed_memset(buf, 0x00, buf_size);
	eembed_err_log = &buf_log;
	err = eembed_dev_urandom_bytes(bytes_buf, bytes_buf_size);
	eembed_err_log = orig_err_log;
	eembed_crash_if_false((err == 0 ? 0 : 1) == 1);
	eembed_crash_if_false(eembed_strstr(buf, "read"));
	read_fails = 0;
	read_really_broken = 0;

	/* fake read() is interrupted once, but then completes */
	urandom_fd = 7;
	read_fails = 1;
	err = eembed_dev_urandom_bytes(bytes_buf, bytes_buf_size);
	eembed_crash_if_false(err == 0);
	eembed_crash_if_false(eembed_strcmp(actual_pathname, "/dev/urandom") ==
			      0);
	eembed_crash_if_false(actual_flags == O_RDONLY);
	eembed_crash_if_false(actual_fd == urandom_fd);
	eembed_crash_if_false(actual_request == RNDGETENTCNT);
	eembed_crash_if_false(actual_fd2 == urandom_fd);
	eembed_crash_if_false(actual_fd3 == urandom_fd);

	eembed_err_log = orig_err_log;
	eembed_system_open = orig_system_open;
	eembed_system_read = orig_system_read;
	eembed_system_ioctl = orig_system_ioctl;
	eembed_system_close = orig_system_close;

	return 0;
}
#else
unsigned int test_dev_urandom_bytes(void)
{
	return 0;
}
#endif

EEMBED_FUNC_MAIN(test_dev_urandom_bytes)
