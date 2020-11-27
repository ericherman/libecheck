/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed-io.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <stddef.h>
#include <limits.h>

#include "eembed.h"
#include "eembed-hosted.h"

void (*eembed_assert_crash)(void) = NULL;

#if EEMBED_HOSTED
#include <stdarg.h>
#include <stdio.h>

#ifndef EEMBDED_IO_HAVE_SNPRINTF
#if _XOPEN_SOURCE >= 500 || _ISOC99_SOURCE || _GNU_SOURCE || _BSD_SOURCE
#define EEMBDED_IO_HAVE_SNPRINTF 1
#else
#define EEMBDED_IO_HAVE_SNPRINTF 0
#endif
#endif

/* To avoid undefined behavior when writing to stderr, first flush
* stdout, thus ensuring stdout and stderr are "coordinated":
*
* "if two or more handles are used, and any one of them is a stream,
* the application shall ensure that their actions are coordinated as
* described below. If this is not done, the result is undefined."
*
* https://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_05_01
*/
static unsigned char eembed_posix1_2017_2_5_1 = 0;

void eembed_stdout_print(const char *str)
{
	fprintf(stdout, "%s", str);
}

void (*eembed_system_print)(const char *str) = eembed_stdout_print;

void eembed_stdout_println(void)
{
	fprintf(stdout, "\n");
	fflush(stdout);
}

void (*eembed_system_println)(void) = eembed_stdout_println;

static FILE *eembed_fprintf_context(struct eembed_log *log)
{
	FILE *stream = log->context ? (FILE *)log->context : stderr;
	if (stream == stderr && !eembed_posix1_2017_2_5_1) {
		fflush(stdout);
		eembed_posix1_2017_2_5_1 = 1;
	}
	return stream;
}

void eembed_fprintf_append_s(struct eembed_log *log, const char *str)
{
	fprintf(eembed_fprintf_context(log), "%s", str);
}

void eembed_fprintf_append_ul(struct eembed_log *log, unsigned long ul)
{
	fprintf(eembed_fprintf_context(log), "%lu", ul);
}

void eembed_fprintf_append_z(struct eembed_log *log, size_t z)
{
	fprintf(eembed_fprintf_context(log), "%lu", (unsigned long)z);
}

void eembed_fprintf_append_l(struct eembed_log *log, long l)
{
	fprintf(eembed_fprintf_context(log), "%ld", l);
}

void eembed_fprintf_append_f(struct eembed_log *log, double f)
{
	fprintf(eembed_fprintf_context(log), "%g", f);
}

void eembed_fprintf_append_vp(struct eembed_log *log, const void *ptr)
{
	fprintf(eembed_fprintf_context(log), "%p", (void *)ptr);
}

void eembed_fprintf_append_eol(struct eembed_log *log)
{
	FILE *stream = eembed_fprintf_context(log);
	fprintf(stream, "\n");
	fflush(stream);
	eembed_posix1_2017_2_5_1 = 0;
}

struct eembed_log eembed_default_fprintf_log = {
	NULL,
	eembed_fprintf_append_s,
	eembed_fprintf_append_ul,
	eembed_fprintf_append_z,
	eembed_fprintf_append_l,
	eembed_fprintf_append_f,
	eembed_fprintf_append_vp,
	eembed_fprintf_append_eol
};

struct eembed_log *eembed_err_log = &eembed_default_fprintf_log;

void eembed_sprintf_append(struct eembed_log *log, const char *format, ...)
{
	struct eembed_buf_context *ctx = NULL;
	size_t used = 0;
	char *buf = NULL;
	size_t len = 0;
	va_list ap;

	ctx = log ? (struct eembed_buf_context *)log->context : NULL;
	if (!ctx || !ctx->buf || !ctx->len) {
		return;
	}
	used = eembed_strnlen(ctx->buf, ctx->len);
	if (used < (ctx->len - 1)) {
		buf = ctx->buf + used;
		len = ctx->len - used;
		va_start(ap, format);
#if EEMBDED_IO_HAVE_SNPRINTF
		vsnprintf(buf, len, format, ap);
#else
		(void)len;
		vsprintf(buf, format, ap);
#endif
		va_end(ap);
	}
}

void eembed_sprintf_append_s(struct eembed_log *log, const char *str)
{
	eembed_sprintf_append(log, "%s", str);
}

void eembed_sprintf_append_ul(struct eembed_log *log, unsigned long ul)
{
	eembed_sprintf_append(log, "%lu", ul);
}

void eembed_sprintf_append_z(struct eembed_log *log, size_t z)
{
	eembed_sprintf_append(log, "%lu", (unsigned long)z);
}

void eembed_sprintf_append_l(struct eembed_log *log, long l)
{
	eembed_sprintf_append(log, "%ld", l);
}

void eembed_sprintf_append_f(struct eembed_log *log, double f)
{
	eembed_sprintf_append(log, "%g", f);
}

void eembed_sprintf_append_vp(struct eembed_log *log, const void *ptr)
{
	eembed_sprintf_append(log, "%p", (void *)ptr);
}

void eembed_sprintf_append_eol(struct eembed_log *log)
{
	eembed_sprintf_append(log, "\n");
}

struct eembed_log *eembed_char_buf_log_init(struct eembed_log *log,
					    struct eembed_buf_context *ctx,
					    char *buf, size_t len)
{
	if (!log) {
		return NULL;
	}

	log->context = ctx;
	log->append_s = eembed_sprintf_append_s;
	log->append_ul = eembed_sprintf_append_ul;
	log->append_z = eembed_sprintf_append_z;
	log->append_l = eembed_sprintf_append_l;
	log->append_f = eembed_sprintf_append_f;
	log->append_vp = eembed_sprintf_append_vp;
	log->append_eol = eembed_sprintf_append_eol;

	if (!log->context) {
		return NULL;
	}

	ctx->buf = buf;
	if (!ctx->buf) {
		ctx->len = 0;
		return NULL;
	}

	ctx->len = len;

	return ctx->len ? log : NULL;
}

char *eembed_sprintf_to_str(char *buf, size_t len, const char *format, ...)
{
	va_list ap;
	int printed = 0;

	if (!buf || !len) {
		return buf;
	}

	va_start(ap, format);
#if EEMBDED_IO_HAVE_SNPRINTF
	printed = vsnprintf(buf, len, format, ap);
#else
	(void)len;
	printed = vsprintf(buf, format, ap);
#endif
	va_end(ap);

	if (printed < 0) {
		buf[0] = '\0';
		return NULL;
	}

	buf[len - 1] = '\0';
	return buf;
}

char *eembed_long_to_str(char *buf, size_t len, long l)
{
	return eembed_sprintf_to_str(buf, len, "%ld", l);
}

char *eembed_ulong_to_str(char *buf, size_t len, unsigned long ul)
{
	return eembed_sprintf_to_str(buf, len, "%lu", ul);
}

char *eembed_ulong_to_hex(char *buf, size_t len, unsigned long ul)
{
	return eembed_sprintf_to_str(buf, len, "%lx", ul);
}

char *eembed_float_to_str(char *buf, size_t len, double f)
{
	return eembed_sprintf_to_str(buf, len, "%g", f);
}

#else /* #if EEMBED_HOSTED */

void eembed_no_op_print(const char *str)
{
	(void)str;
}

void (*eembed_system_print)(const char *str) = eembed_no_op_print;

void eembed_sys_print_println(void)
{
	eembed_system_print("\n");
}

void (*eembed_system_println)(void) = eembed_sys_print_println;

void eembed_sys_print_str_append_s(struct eembed_log *log, const char *str)
{
	(void)log;
	eembed_system_print(str);
}

void eembed_sys_print_str_append_ul(struct eembed_log *log, unsigned long ul)
{
	char buf[25];

	(void)log;
	buf[0] = '\0';

	eembed_system_print(eembed_ulong_to_str(buf, 25, ul));
}

void eembed_sys_print_str_append_z(struct eembed_log *log, size_t z)
{
	char buf[25];

	(void)log;
	buf[0] = '\0';

	eembed_system_print(eembed_ulong_to_str(buf, 25, z));
}

void eembed_sys_print_str_append_l(struct eembed_log *log, long l)
{
	char buf[25];

	(void)log;
	buf[0] = '\0';

	eembed_system_print(eembed_long_to_str(buf, 25, l));
}

void eembed_sys_print_str_append_f(struct eembed_log *log, double f)
{
	char buf[25];

	(void)log;
	buf[0] = '\0';

	eembed_system_print(eembed_float_to_str(buf, 25, f));
}

void eembed_sys_print_str_append_vp(struct eembed_log *log, const void *ptr)
{
	char buf[25];
	size_t z = (size_t)ptr;

	(void)log;
	buf[0] = '\0';

	eembed_system_print(eembed_ulong_to_hex(buf, 25, z));
}

void eembed_sys_print_str_append_eol(struct eembed_log *log)
{
	(void)log;
	eembed_system_println();
}

struct eembed_log eembed_default_sys_print_str_log = {
	NULL,
	eembed_sys_print_str_append_s,
	eembed_sys_print_str_append_ul,
	eembed_sys_print_str_append_z,
	eembed_sys_print_str_append_l,
	eembed_sys_print_str_append_f,
	eembed_sys_print_str_append_vp,
	eembed_sys_print_str_append_eol
};

struct eembed_log *eembed_err_log = &eembed_default_sys_print_str_log;

void eembed_ctx_strcpy_append_s(struct eembed_log *log, const char *str)
{
	size_t used = 0;
	char *buf = NULL;
	size_t len = 0;
	size_t len2 = 0;
	size_t min = 0;
	struct eembed_buf_context *ctx = NULL;

	ctx = log ? (struct eembed_buf_context *)log->context : NULL;
	if (!ctx || !ctx->buf || !ctx->len) {
		return;
	}
	used = eembed_strnlen(ctx->buf, ctx->len);
	if (used < (ctx->len - 1)) {
		buf = ctx->buf + used;
		len = ctx->len - used;
		len2 = eembed_strlen(str);
		min = len2 < len ? len2 : len;
		eembed_strncpy(buf, str, min);
		if (min < len) {
			buf[min] = '\0';
		}
	}
	ctx->buf[ctx->len - 1] = '\0';
}

void eembed_ctx_strcpy_append_ul(struct eembed_log *log, unsigned long ul)
{
	char buf[25];

	buf[0] = '\0';
	eembed_ctx_strcpy_append_s(log, eembed_ulong_to_str(buf, 25, ul));
}

void eembed_ctx_strcpy_append_z(struct eembed_log *log, size_t z)
{
	char buf[25];

	buf[0] = '\0';
	eembed_ctx_strcpy_append_s(log, eembed_ulong_to_str(buf, 25, z));
}

void eembed_ctx_strcpy_append_l(struct eembed_log *log, long l)
{
	char buf[25];

	buf[0] = '\0';
	eembed_ctx_strcpy_append_s(log, eembed_long_to_str(buf, 25, l));
}

void eembed_ctx_strcpy_append_f(struct eembed_log *log, double f)
{
	char buf[25];

	buf[0] = '\0';
	eembed_ctx_strcpy_append_s(log, eembed_float_to_str(buf, 25, f));
}

void eembed_ctx_strcpy_append_vp(struct eembed_log *log, const void *ptr)
{
	char buf[25];
	size_t zptr = (size_t)ptr;

	buf[0] = '\0';
	eembed_ctx_strcpy_append_s(log, eembed_ulong_to_hex(buf, 25, zptr));
}

void eembed_ctx_strcpy_append_eol(struct eembed_log *log)
{
	eembed_ctx_strcpy_append_s(log, "\n");
}

struct eembed_log *eembed_char_buf_log_init(struct eembed_log *log,
					    struct eembed_buf_context *ctx,
					    char *buf, size_t len)
{
	if (!log) {
		return NULL;
	}

	log->context = ctx;
	log->append_s = eembed_ctx_strcpy_append_s;
	log->append_ul = eembed_ctx_strcpy_append_ul;
	log->append_z = eembed_ctx_strcpy_append_z;
	log->append_l = eembed_ctx_strcpy_append_l;
	log->append_f = eembed_ctx_strcpy_append_f;
	log->append_vp = eembed_ctx_strcpy_append_vp;
	log->append_eol = eembed_ctx_strcpy_append_eol;

	if (!log->context) {
		return NULL;
	}

	ctx->buf = buf;
	if (!ctx->buf) {
		ctx->len = 0;
		return NULL;
	}

	ctx->len = len;

	return ctx->len ? log : NULL;
}

char *eembed_long_to_str(char *buf, size_t len, long l)
{
	char *b;
	size_t blen = 0;
	unsigned long ul = 0;

	if (!buf || !len) {
		return buf;
	}

	if (l < 0) {
		buf[0] = '-';
		b = buf + 1;
		blen = len - 1;
		ul = (-l);
	} else {
		b = buf;
		blen = 25;
		ul = l;
	}
	if (blen) {
		b[0] = '\0';
	}
	return eembed_ulong_to_str(b, blen, ul);
}

char *eembed_ulong_to_str(char *buf, size_t len, unsigned long ul)
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

static char eembed_nibble_to_hex(unsigned char nibble)
{
	if (nibble < 10) {
		return '0' + nibble;
	}

	return 'A' + (nibble - 10);
}

static void eembed_byte_to_hex_chars(char *hi, char *lo, unsigned char byte)
{
	*hi = eembed_nibble_to_hex((byte & 0xF0) >> 4);
	*lo = eembed_nibble_to_hex((byte & 0x0F));
}

char *eembed_ulong_to_hex(char *buf, size_t len, unsigned long z)
{
	size_t i = 0;
	size_t pos = 0;
	size_t ul_bytes = sizeof(unsigned long);

	if (len < (2 + (2 * ul_bytes) + 1)) {
		return NULL;
	}

	buf[pos++] = '0';
	buf[pos++] = 'x';

	for (i = 0; i < ul_bytes; ++i) {
		char h = 0;
		char l = 0;
		unsigned char byte = 0;

		byte = (0xFF & (z >> (8 * ((ul_bytes - 1) - i))));
		eembed_byte_to_hex_chars(&h, &l, byte);
		buf[pos++] = h;
		buf[pos++] = l;
	}
	buf[pos] = '\0';
	return buf;
}

char *eembed_bogus_float_to_str(char *buf, size_t len, double f)
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
	if (f > (double)ULONG_MAX) {
		if (pos < len) {
			buf[pos++] = 'b';
		}
		if (pos < len) {
			buf[pos++] = 'i';
		}
		if (pos < len) {
			buf[pos++] = 'g';
		}
		buf[pos] = '\0';
		return buf;
	}

	ul = (unsigned long)f;
	eembed_ulong_to_str(buf + pos, len - pos, ul);
	pos = eembed_strnlen(buf, len);
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
		f = f - ul;
	}
	return buf;
}

char *eembed_float_to_str(char *buf, size_t len, double f)
{
	return eembed_bogus_float_to_str(buf, len, f);
}
#endif
