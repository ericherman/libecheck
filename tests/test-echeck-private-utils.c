/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include "test-echeck-private-utils.h"

#include <limits.h>

#if ECHECK_HOSTED
#include <stdio.h>
void echeck_test_stderr_prints(const char *s)
{
	fprintf(stderr, "%s", s);
}

void (*echeck_test_debug_prints)(const char *s) = echeck_test_stderr_prints;
#else
void echeck_test_silent_prints(const char *s)
{
	(void)s;
}

void (*echeck_test_debug_prints)(const char *s) = echeck_test_silent_prints;
#endif

void echeck_test_diy_printv(const void *v)
{
	char buf[25];
	echeck_test_u64_to_hex(buf, 25, (uint64_t) v);
	echeck_test_debug_prints(buf);
}

void (*echeck_test_debug_printv)(const void *v) = echeck_test_diy_printv;

void echeck_test_diy_pritntz(size_t z)
{
	char buf[25];
	echeck_test_ul_to_str(buf, 25, (unsigned long)z);
	echeck_test_debug_prints(buf);
}

void (*echeck_test_debug_printz)(size_t z) = echeck_test_diy_pritntz;

void echeck_test_diy_printeol(void)
{
	echeck_test_debug_prints("\n");
}

void (*echeck_test_debug_printeol)(void) = echeck_test_diy_printeol;

#define echeck_test_mem_log_len 1024
char echeck_test_mem_log[echeck_test_mem_log_len];

size_t echeck_diy_strnlen(const char *str, size_t buf_size)
{
	size_t i;

	for (i = 0; i < buf_size; ++i) {
		if (*(str + i) == '\0') {
			return i;
		}
	}

	return buf_size;
}

static char *echeck_diy_strcpy(char *dest, const char *src)
{
	size_t i;
	if (!dest) {
		return NULL;
	}
	i = 0;
	if (src) {
		while (*(src + i)) {
			*(dest + i) = *(src + i);
			++i;
		}
	}
	*(dest + i) = '\0';
	return dest;
}

static char echeck_test_nibble_to_hex(unsigned char nibble)
{
	if (nibble < 10) {
		return '0' + nibble;
	}

	return 'A' + nibble - 10;
}

static void echeck_test_byte_to_hex_chars(char *hi, char *lo,
					  unsigned char byte)
{
	*hi = echeck_test_nibble_to_hex((byte & 0xF0) >> 4);
	*lo = echeck_test_nibble_to_hex((byte & 0x0F));
}

char *echeck_test_u64_to_hex(char *buf, size_t len, uint64_t z)
{
	size_t i = 0;
	size_t pos = 0;
	size_t u64_bytes = sizeof(uint64_t);

	if (len < (2 + (2 * u64_bytes) + 1)) {
		return NULL;
	}

	buf[pos++] = '0';
	buf[pos++] = 'x';

	for (i = 0; i < u64_bytes; ++i) {
		char h = 0;
		char l = 0;
		unsigned char byte = 0;

		byte = (0xFF & (z >> (8 * ((u64_bytes - 1) - i))));
		echeck_test_byte_to_hex_chars(&h, &l, byte);
		buf[pos++] = h;
		buf[pos++] = l;
	}
	buf[pos] = '\0';
	return buf;
}

char *echeck_test_ul_to_str(char *buf, size_t len, unsigned long ul)
{
	char tmp[22];
	size_t i = 0;
	size_t j = 0;

	tmp[0] = '\0';

	if (!buf || !len) {
		return NULL;
	} else if (len == 1) {
		buf[0] = '\0';
		return NULL;
	}

	if (!ul) {
		buf[0] = '0';
		buf[1] = '\0';
		return buf;
	}

	for (i = 0; ul && i < 22; ++i) {
		tmp[i] = '0' + (ul % 10);
		ul = ul / 10;
	}
	for (j = 0; i && j < len; ++j, --i) {
		buf[j] = tmp[i - 1];
	}

	buf[j < len ? j : len - 1] = '\0';

	return buf;
}

char *echeck_test_bogus_float_to_str(char *buf, size_t len, double f)
{
	size_t pos = 0;
	size_t i = 0;
	size_t max = 5;
	char c;
	unsigned long ul = 0;

	buf[0] = '\0';
	buf[len - 1] = '\0';
	if (f < 0.0) {
		buf[pos++] = '-';
		f = -f;
	}
	ul = (unsigned long)f;
	echeck_test_ul_to_str(buf + pos, len - pos, ul);
	pos = echeck_diy_strnlen(buf, len);
	f = f - (double)ul;
	if (pos < len) {
		buf[pos++] = '.';
	}
	if ((len - pos) < max) {
		max = len - pos;
	}
	for (i = 0; i < max; ++i) {
		f = f * 10;
		ul = (unsigned long)f;
		c = '0' + (ul % 10);
		buf[pos++] = c;
		buf[pos] = '\0';
	}
	return buf;
}

void echeck_test_buf_append_s(struct echeck_log *log, const char *str)
{
	size_t len = 0;
	char *dest = NULL;
	(void)log;

	len = echeck_diy_strnlen(echeck_test_mem_log, echeck_test_mem_log_len);
	dest = echeck_test_mem_log + len;
	echeck_diy_strcpy(dest, str);
}

void echeck_test_buf_append_ul(struct echeck_log *log, unsigned long ul)
{
	char ulbuf[22];

	echeck_test_ul_to_str(ulbuf, 22, ul);
	echeck_test_buf_append_s(log, ulbuf);
}

void echeck_test_buf_append_z(struct echeck_log *log, size_t z)
{
	echeck_test_buf_append_ul(log, (unsigned long)z);
}

void echeck_test_buf_append_l(struct echeck_log *log, long l)
{
	unsigned long ul;
	if (l < 0) {
		echeck_test_buf_append_s(log, "-");
		ul = (unsigned long)-l;
	} else {
		ul = (unsigned long)l;
	}
	echeck_test_buf_append_ul(log, ul);
}

void echeck_test_buf_append_f(struct echeck_log *log, double f)
{
	char buf[25];
	echeck_test_bogus_float_to_str(buf, 25, f);
	echeck_test_buf_append_s(log, buf);
}

void echeck_test_buf_append_vp(struct echeck_log *log, const void *ptr)
{
	char buf[25];
	echeck_test_u64_to_hex(buf, 25, (uint64_t) ptr);
	echeck_test_buf_append_s(log, buf);
}

void echeck_test_buf_append_eol(struct echeck_log *log)
{
	echeck_test_buf_append_s(log, "\n");
}

struct echeck_log echeck_test_buf_log = {
	NULL,
	echeck_test_buf_append_s,
	echeck_test_buf_append_ul,
	echeck_test_buf_append_z,
	echeck_test_buf_append_l,
	echeck_test_buf_append_f,
	echeck_test_buf_append_vp,
	echeck_test_buf_append_eol
};

struct echeck_log *echeck_test_log_capture(void)
{
	struct echeck_log *orig = echeck_default_log;
	echeck_default_log = &echeck_test_buf_log;
	echeck_test_mem_log[0] = '\0';
	return orig;
}

void echeck_test_log_release(struct echeck_log *orig)
{
	echeck_default_log = orig;
}

static char *echeck_diy_strstr(const char *haystack, const char *needle)
{
	size_t i, j, found, len;

	if (!haystack) {
		return NULL;
	}

	len = 0;
	if (!needle || !(len = echeck_diy_strnlen(needle, INT_MAX))) {
		return (char *)haystack;
	}

	for (i = 0; haystack[i]; ++i) {
		found = 1;
		for (j = 0; found && j < len; ++j) {
			if (haystack[i + j] != needle[j]) {
				found = 0;
			}
		}
		if (found) {
			return (char *)(haystack + i);
		}
	}
	return NULL;
}

int err_contains(const char *expected[], size_t expected_len)
{
	size_t i;
	unsigned int msg_found;

	msg_found = 0;
	for (i = 0; i < expected_len; i++) {
		if (echeck_diy_strstr(echeck_test_mem_log, expected[i])) {
			msg_found++;
		} else {
			echeck_test_debug_prints("'");
			echeck_test_debug_prints(expected[i]);
			echeck_test_debug_prints("' not in '");
			echeck_test_debug_prints(echeck_test_mem_log);
			echeck_test_debug_prints("'");
			echeck_test_debug_printeol();
		}
	}
	return expected_len - msg_found;
}

void echeck_test_debug_print_failures(unsigned failures, const char *where)
{
	echeck_test_debug_printz(failures);
	echeck_test_debug_prints(" failures in ");
	echeck_test_debug_prints(where);
	echeck_test_debug_printeol();
}
