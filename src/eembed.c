/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include "eembed.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#if EEMBED_HOSTED
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#if EEMBED_HOSTED
/* LCOV_EXCL_START */
void eembed_exit_failure(void)
{
	/* assertion failed, now we crash */
	exit(EXIT_FAILURE);
}

/* LCOV_EXCL_STOP */

void (*eembed_assert_crash)(void) = eembed_exit_failure;
#else
void (*eembed_assert_crash)(void) = NULL;
#endif

#if EEMBED_HOSTED

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
	/* LCOV_EXCL_START */
	if (stream == stderr && !eembed_posix1_2017_2_5_1) {
		fflush(stdout);
		eembed_posix1_2017_2_5_1 = 1;
	}
	/* LCOV_EXCL_STOP */
	return stream;
}

void eembed_fprintf_append_s(struct eembed_log *log, const char *str)
{
	fprintf(eembed_fprintf_context(log), "%s", str);
}

void eembed_fprintf_append_ul(struct eembed_log *log, uint64_t ul)
{
	fprintf(eembed_fprintf_context(log), "%" PRIu64, ul);
}

void eembed_fprintf_append_z(struct eembed_log *log, size_t z)
{
	fprintf(eembed_fprintf_context(log), "%" PRIu64, (uint64_t)z);
}

void eembed_fprintf_append_l(struct eembed_log *log, int64_t l)
{
	fprintf(eembed_fprintf_context(log), "%" PRId64, l);
}

void eembed_fprintf_append_f(struct eembed_log *log, long double f)
{
	fprintf(eembed_fprintf_context(log), "%Lg", f);
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
	struct eembed_str_buf *ctx = NULL;
	size_t used = 0;
	char *buf = NULL;
	size_t len = 0;
	va_list ap;

	ctx = log ? (struct eembed_str_buf *)log->context : NULL;
	/* LCOV_EXCL_START */
	/* this would be a programmer error */
	if (!ctx || !ctx->buf || !ctx->len) {
		return;
	}
	/* LCOV_EXCL_STOP */
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

void eembed_sprintf_append_ul(struct eembed_log *log, uint64_t ul)
{
	eembed_sprintf_append(log, "%" PRIu64, ul);
}

void eembed_sprintf_append_z(struct eembed_log *log, size_t z)
{
	eembed_sprintf_append(log, "%" PRIu64, (uint64_t)z);
}

void eembed_sprintf_append_l(struct eembed_log *log, int64_t l)
{
	eembed_sprintf_append(log, "%" PRId64, l);
}

void eembed_sprintf_append_f(struct eembed_log *log, long double f)
{
	eembed_sprintf_append(log, "%Lg", f);
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
					    struct eembed_str_buf *ctx,
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

	/* LCOV_EXCL_START */
	/* Something *very* strange is afoot, bail out! */
	if (printed < 0) {
		buf[0] = '\0';
		return NULL;
	}
	/* LCOV_EXCL_STOP */

	buf[len - 1] = '\0';
	return buf;
}

char *eembed_long_to_str(char *buf, size_t len, int64_t l)
{
	return eembed_sprintf_to_str(buf, len, "%" PRId64, l);
}

char *eembed_ulong_to_str(char *buf, size_t len, uint64_t ul)
{
	return eembed_sprintf_to_str(buf, len, "%" PRIu64, ul);
}

char *eembed_ulong_to_hex(char *buf, size_t len, uint64_t ul)
{
	return eembed_sprintf_to_str(buf, len, "0x%02" PRIX64, ul);
}

char *eembed_float_to_str(char *buf, size_t len, long double f)
{
	return eembed_sprintf_to_str(buf, len, "%Lg", f);
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

void eembed_sys_print_str_append_ul(struct eembed_log *log, uint64_t ul)
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

void eembed_sys_print_str_append_l(struct eembed_log *log, int64_t l)
{
	char buf[25];

	(void)log;
	buf[0] = '\0';

	eembed_system_print(eembed_long_to_str(buf, 25, l));
}

void eembed_sys_print_str_append_f(struct eembed_log *log, long double f)
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
	struct eembed_str_buf *ctx = NULL;

	ctx = log ? (struct eembed_str_buf *)log->context : NULL;
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

void eembed_ctx_strcpy_append_ul(struct eembed_log *log, uint64_t ul)
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

void eembed_ctx_strcpy_append_l(struct eembed_log *log, int64_t l)
{
	char buf[25];

	buf[0] = '\0';
	eembed_ctx_strcpy_append_s(log, eembed_long_to_str(buf, 25, l));
}

void eembed_ctx_strcpy_append_f(struct eembed_log *log, long double f)
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
					    struct eembed_str_buf *ctx,
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

char *eembed_long_to_str(char *buf, size_t len, int64_t l)
{
	char *b;
	char *out;
	size_t blen = 0;
	uint64_t ul = 0;

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
	out = eembed_ulong_to_str(b, blen, ul);
	return out ? buf : NULL;
}

char *eembed_ulong_to_str(char *buf, size_t len, uint64_t ul)
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

char *eembed_ulong_to_hex(char *buf, size_t len, uint64_t z)
{
	size_t i = 0;
	size_t pos = 0;
	size_t ul_bytes = sizeof(uint64_t);

	if (!buf) {
		return NULL;
	}

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

char *eembed_bogus_float_to_str(char *buf, size_t len, long double f)
{
	size_t pos = 0;
	size_t i = 0;
	size_t max_digits = 6;
	long double min_to_print = 0.000001;
	long double add_for_rounding = (min_to_print / 2.0);
	char c;
	unsigned long ul = 0;

	if (!buf || !len) {
		return NULL;
	}

	buf[0] = '\0';
	buf[len - 1] = '\0';
	if (f != f) {
		if (pos < len) {
			buf[pos++] = 'n';
		}
		if (pos < len) {
			buf[pos++] = 'a';
		}
		if (pos < len) {
			buf[pos++] = 'n';
		}
		buf[pos] = '\0';
		return buf;
	}
	if (f < 0.0) {
		buf[pos++] = '-';
		f = -f;
	}
	if ((f - (long double)ULONG_MAX) >= 1.0) {
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
	f = f - (long double)ul;
	if (pos < len) {
		buf[pos++] = '.';
	}
	if ((len - pos) < max_digits) {
		max_digits = len - pos;
	}
	f += add_for_rounding;
	for (i = 0; (i < max_digits) && (i == 0 || f >= min_to_print); ++i) {
		f = f * 10;
		ul = (unsigned long)f;
		c = '0' + (ul % 10);
		buf[pos++] = c;
		buf[pos] = '\0';
		f = f - ul;
		min_to_print = min_to_print * 10;
	}
	return buf;
}

char *eembed_float_to_str(char *buf, size_t len, long double f)
{
	return eembed_bogus_float_to_str(buf, len, f);
}
#endif

void *eembed_malloc(size_t size)
{
	struct eembed_allocator *ea = eembed_global_allocator;
	return ea ? ea->malloc(ea, size) : NULL;
}

void *eembed_realloc(void *ptr, size_t size)
{
	struct eembed_allocator *ea = eembed_global_allocator;
	return ea ? ea->realloc(ea, ptr, size) : NULL;
}

void *eembed_calloc(size_t nmemb, size_t size)
{
	struct eembed_allocator *ea = eembed_global_allocator;
	return ea ? ea->calloc(ea, nmemb, size) : NULL;
}

void *eembed_reallocarray(void *ptr, size_t nmemb, size_t size)
{
	struct eembed_allocator *ea = eembed_global_allocator;
	return ea ? ea->reallocarray(ea, ptr, nmemb, size) : NULL;
}

void eembed_free(void *ptr)
{
	struct eembed_allocator *ea = eembed_global_allocator;
	if (ea) {
		ea->free(ea, ptr);
	}
}

#if EEMBED_HOSTED

int (*eembed_memcmp)(const void *s1, const void *s2, size_t n) = memcmp;

#else

int eembed_diy_memcmp(const void *a1, const void *a2, size_t n)
{
	size_t i;
	const unsigned char *s1;
	const unsigned char *s2;
	int d;

	if (a1 == a2 || n == 0) {
		return 0;
	}

	/* LCOV_EXCL_START */
	/* glibc explodes on NULL */
	if (!a1 || !a2) {
		return a1 ? 1 : -1;
	}
	/* LCOV_EXCL_STOP */

	s1 = (unsigned char *)a1;
	s2 = (unsigned char *)a2;
	for (i = 0; i < n; ++i) {
		d = s1[i] - s2[i];
		if (d) {
			return d;
		}
	}
	return 0;
}

int (*eembed_memcmp)(const void *s1, const void *s2, size_t n) =
    eembed_diy_memcmp;
#endif

#if EEMBED_HOSTED

void *(*eembed_memcpy)(void *dest, const void *src, size_t n) = memcpy;
void *(*eembed_memmove)(void *dest, const void *src, size_t n) = memmove;

#else

void *eembed_diy_memmove(void *d, const void *s, size_t n)
{
	const unsigned char *src = (const unsigned char *)s;
	unsigned char *dest = (unsigned char *)d;
	size_t i;

	if (src < dest && dest < src + n) {
		for (i = n; i; i--) {
			dest[i - 1] = src[i - 1];
		}
	} else {
		for (i = 0; i < n; ++i) {
			dest[i] = src[i];
		}
	}
	return d;
}

void *(*eembed_memcpy)(void *dest, const void *src, size_t n) =
    eembed_diy_memmove;
void *(*eembed_memmove)(void *dest, const void *src, size_t n) =
    eembed_diy_memmove;

#endif

#if EEMBED_HOSTED

void *(*eembed_memset)(void *dest, int c, size_t n) = memset;

#else

void *eembed_diy_memset(void *dest, int val, size_t n)
{
	unsigned char *d;
	unsigned char v;
	if (!n || !dest) {
		return dest;
	}

	d = (unsigned char *)dest;
	v = (unsigned char)val;
	while (n--) {
		d[n] = v;
	}

	return dest;
}

void *(*eembed_memset)(void *dest, int c, size_t n) = eembed_diy_memset;

#endif

#if EEMBED_HOSTED

char *(*eembed_strcat)(char *dest, const char *src) = strcat;
char *(*eembed_strncat)(char *dest, const char *src, size_t n) = strncat;

#else

char *eembed_diy_strcat(char *dest, const char *src)
{
	return eembed_strncat(dest, src, SIZE_MAX);
}

char *(*eembed_strcat)(char *dest, const char *src) = eembed_diy_strcat;

char *eembed_diy_strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = 0;
	size_t i = 0;

	if (!dest) {
		return NULL;
	}

	dest_len = eembed_strlen(dest);
	if (src) {
		while (i < n && src[i] != '\0') {
			dest[dest_len + i] = src[i];
			++i;
		}
	}
	dest[dest_len + i] = '\0';

	return dest;
}

char *(*eembed_strncat)(char *dest, const char *src, size_t n) =
    eembed_diy_strncat;

#endif

#if EEMBED_HOSTED

int (*eembed_strcmp)(const char *s1, const char *s2) = strcmp;
int (*eembed_strncmp)(const char *s1, const char *s2, size_t n) = strncmp;

#else

int echeck_diy_strcmp(const char *s1, const char *s2)
{
	return eembed_strncmp(s1, s2, SIZE_MAX);
}

int (*eembed_strcmp)(const char *s1, const char *s2) = echeck_diy_strcmp;

int echeck_diy_strncmp(const char *s1, const char *s2, size_t max_len)
{
	size_t i;
	int d;

	if (s1 == s2 || max_len == 0) {
		return 0;
	}
	/* LCOV_EXCL_START */
	/* glibc explodes on NULL */
	if (!s1 || !s2) {
		return s1 ? 1 : -1;
	}
	/* LCOV_EXCL_STOP */

	for (i = 0; i < max_len; ++i) {
		d = s1[i] - s2[i];
		if (d) {
			return d;
		} else if (s1[i] == '\0') {
			return 0;
		}
	}
	return 0;
}

int (*eembed_strncmp)(const char *s1, const char *s2, size_t n) =
    echeck_diy_strncmp;
#endif

#if EEMBED_HOSTED

char *(*eembed_strcpy)(char *dest, const char *src) = strcpy;
char *(*eembed_strncpy)(char *dest, const char *src, size_t n) = strncpy;

#else

char *eembedd_diy_strcpy(char *dest, const char *src)
{
	return eembed_strncpy(dest, src, SIZE_MAX);
}

char *(*eembed_strcpy)(char *dest, const char *src) = eembedd_diy_strcpy;

char *eembed_diy_strncpy(char *dest, const char *src, size_t n)
{
	size_t i = 0;

	if (!dest) {
		return NULL;
	}

	if (src) {
		while (i < n && src[i] != '\0') {
			dest[i] = src[i];
			++i;
		}
	}
	if (i < n) {
		dest[i] = '\0';
	}

	return dest;
}

char *(*eembed_strncpy)(char *dest, const char *src, size_t n) =
    eembed_diy_strncpy;

#endif

#if (!((_POSIX_C_SOURCE >= 200809L) && (EEMBED_HOSTED)))
#define Eembed_use_diy_strnlen 1
size_t eembed_diy_strnlen(const char *str, size_t buf_size)
{
	size_t i;

	for (i = 0; i < buf_size; ++i) {
		if (str[i] == '\0') {
			return i;
		}
	}

	return buf_size;
}
#else
#define Eembed_use_diy_strnlen 0
#endif

#if EEMBED_HOSTED

size_t (*eembed_strlen)(const char *s) = strlen;
#if Eembed_use_diy_strnlen
size_t (*eembed_strnlen)(const char *s, size_t maxlen) = eembed_diy_strnlen;
#else
size_t (*eembed_strnlen)(const char *s, size_t maxlen) = strnlen;
#endif

#else

size_t eembed_diy_strlen(const char *s)
{
	return eembed_strnlen(s, SIZE_MAX);
}

size_t (*eembed_strlen)(const char *s) = eembed_diy_strlen;

size_t (*eembed_strnlen)(const char *s, size_t maxlen) = eembed_diy_strnlen;

#endif

#if EEMBED_HOSTED
char *(*eembed_strstr)(const char *haystack, const char *needle) = strstr;
#else
char *eembed_diy_strstr(const char *haystack, const char *needle)
{
	size_t i = 0;
	size_t j = 0;
	size_t found = 0;
	size_t nlen = 0;
	size_t hlen = 0;

	if (!haystack || !needle) {
		return NULL;
	}

	nlen = eembed_strlen(needle);
	if (!nlen) {
		return (char *)(haystack);
	}
	hlen = eembed_strlen(haystack);
	if (nlen > hlen) {
		return NULL;
	}
	for (i = 0; i < (hlen - (nlen - 1)); ++i) {
		found = 1;
		for (j = 0; found && j < nlen; ++j) {
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

char *(*eembed_strstr)(const char *haystack, const char *needle) =
    eembed_diy_strstr;
#endif

struct eembed_alloc_chunk {
	unsigned char *start;
	size_t available_length;
	unsigned char in_use;
	struct eembed_alloc_chunk *prev;
	struct eembed_alloc_chunk *next;
};

static struct eembed_alloc_chunk *eembed_alloc_chunk_init(unsigned char *bytes,
							  size_t
							  available_length)
{
	struct eembed_alloc_chunk *chunk = (struct eembed_alloc_chunk *)bytes;
	size_t size = eembed_align(sizeof(struct eembed_alloc_chunk));

	if (!bytes || available_length < size) {
		return NULL;
	}
	chunk->start = bytes + size;
	chunk->available_length = available_length - size;
	chunk->in_use = 0;
	chunk->prev = (struct eembed_alloc_chunk *)NULL;
	chunk->next = (struct eembed_alloc_chunk *)NULL;

	return chunk;
}

static void eembed_alloc_chunk_split(struct eembed_alloc_chunk *from,
				     size_t request)
{
	size_t remaining_available_length = 0;
	size_t aligned_request = 0;
	struct eembed_alloc_chunk *orig_next = NULL;
	size_t min_size = eembed_align(sizeof(struct eembed_alloc_chunk)) +
	    eembed_align(sizeof(size_t)) + eembed_align(1);

	aligned_request = eembed_align(request);
	from->in_use = 1;

	if ((aligned_request + min_size) >= from->available_length) {
		return;
	}

	remaining_available_length = from->available_length - aligned_request;
	if (remaining_available_length <= min_size) {
		return;
	}

	from->available_length = aligned_request;
	orig_next = from->next;
	from->next =
	    eembed_alloc_chunk_init((from->start + from->available_length),
				    remaining_available_length);
	from->next->prev = from;
	if (orig_next) {
		from->next->next = orig_next;
		orig_next->prev = from->next;
	} else {
		from->next->next = NULL;
	}
}

void *eembed_chunk_malloc(struct eembed_allocator *ea, size_t size)
{
	struct eembed_alloc_chunk *chunk =
	    (struct eembed_alloc_chunk *)ea->context;

	if (!chunk || !size) {
		return NULL;
	}

	while (chunk != NULL) {
		if (chunk->in_use == 0) {
			if (chunk->available_length >= size) {
				eembed_alloc_chunk_split(chunk, size);
				return chunk->start;
			}
		}
		chunk = chunk->next;
	}

	return NULL;
}

static void eembed_alloc_chunk_join_next(struct eembed_alloc_chunk *chunk)
{
	struct eembed_alloc_chunk *next = NULL;
	size_t additional_available_length = 0;

	next = chunk->next;
	if (!next) {
		return;
	}

	if (next->in_use) {
		return;
	}

	chunk->next = next->next;
	additional_available_length =
	    eembed_align(sizeof(struct eembed_alloc_chunk)) +
	    next->available_length;
	chunk->available_length += additional_available_length;
	if (chunk->next) {
		chunk->next->prev = chunk;
	}
	if (!chunk->in_use) {
		eembed_memset(chunk->start, 0x00, chunk->available_length);
	}
}

/* The  realloc()  function  changes  the  size  of  the memory
 * chunk pointed to by ptr to size bytes.  The contents will be
 * unchanged in the range from the start of the region up to the
 * minimum of the old and new sizes.  If the new size is larger
 * than the old size, the added memory will not be initialized.
 * If ptr is NULL, then the call is equivalent to malloc(size),
 * for all values of size; if size is equal to zero, and ptr is
 * not NULL, then the call is equivalent to  free(ptr).   Unless
 * ptr is NULL, it must have been returned by an earlier call to
 * malloc(), calloc() or realloc().  If the area pointed to was
 * moved, a free(ptr) is done.  */
void *eembed_chunk_realloc(struct eembed_allocator *ea, void *ptr, size_t size)
{
	struct eembed_alloc_chunk *chunk =
	    (struct eembed_alloc_chunk *)ea->context;
	size_t old_size = 0;
	int found = 0;
	void *new_ptr = NULL;

	if (!chunk) {
		return NULL;
	}

	if (!ptr) {
		return ea->malloc(ea, size);
	}
	if (size == 0) {
		ea->free(ea, ptr);
		return NULL;
	}

	found = 0;
	while (chunk != NULL && !found) {
		if (ptr == chunk->start) {
			found = 1;
			old_size = chunk->available_length;
		} else {
			chunk = chunk->next;
		}
	}

	if (!found) {
		return NULL;
	}

	if (old_size >= size) {
		eembed_alloc_chunk_split(chunk, size);
		return ptr;
	}

	if (chunk->next && chunk->next->in_use == 0) {
		eembed_alloc_chunk_join_next(chunk);
		if (chunk->available_length >= size) {
			eembed_memset(((unsigned char *)ptr) + old_size, 0x00,
				      chunk->available_length - old_size);
			eembed_alloc_chunk_split(chunk, size);
			return ptr;
		}
	}

	new_ptr = ea->malloc(ea, size);
	if (!new_ptr) {
		return NULL;
	}
	eembed_memset(new_ptr, 0x00, size);
	eembed_memcpy(new_ptr, ptr, old_size <= size ? old_size : size);

	ea->free(ea, ptr);

	return new_ptr;
}

void *eembed_chunk_calloc(struct eembed_allocator *ea, size_t nmemb,
			  size_t size)
{
	/* this could overflow, but we elect to not care */
	size_t len = nmemb * size;
	void *ptr = NULL;
	ptr = len ? ea->malloc(ea, len) : NULL;
	if (ptr) {
		eembed_memset(ptr, 0x00, len);
	}
	return ptr;
}

void *eembed_chunk_reallocarray(struct eembed_allocator *ea, void *ptr,
				size_t nmemb, size_t size)
{
	/* this could overflow, but we elect to not care */
	size_t len = nmemb * size;
	return len ? ea->realloc(ea, ptr, len) : NULL;
}

void eembed_chunk_free(struct eembed_allocator *ea, void *ptr)
{
	struct eembed_alloc_chunk *chunk =
	    (struct eembed_alloc_chunk *)ea->context;
	size_t len = 0;

	if (!chunk || !ptr) {
		return;
	}

	while (chunk != NULL) {
		if (chunk->start == ptr) {
			chunk->in_use = 0;
			eembed_alloc_chunk_join_next(chunk);
			while (chunk->prev && chunk->prev->in_use == 0) {
				chunk = chunk->prev;
				eembed_alloc_chunk_join_next(chunk);
			}
			len = chunk->available_length;
			eembed_memset(chunk->start, 0x00, len);
			return;
		}
		chunk = chunk->next;
	}
}

struct eembed_allocator eembed_null_chunk_allocator = {
	NULL,
	eembed_chunk_malloc,
	eembed_chunk_calloc,
	eembed_chunk_realloc,
	eembed_chunk_reallocarray,
	eembed_chunk_free
};

struct eembed_allocator *eembed_null_allocator = &eembed_null_chunk_allocator;

struct eembed_allocator *eembed_bytes_allocator(unsigned char *bytes,
						size_t len)
{
	struct eembed_allocator *ea = NULL;
	struct eembed_alloc_chunk *chunk = NULL;
	size_t used = 0;
	size_t min_buf_size =
	    eembed_align(sizeof(struct eembed_allocator)) +
	    eembed_align(sizeof(struct eembed_alloc_chunk)) +
	    eembed_align(sizeof(size_t)) + eembed_align(1);

	if (!bytes || len < min_buf_size) {
		return NULL;
	}

	ea = (struct eembed_allocator *)bytes;
	used = eembed_align(sizeof(struct eembed_allocator));

	chunk = eembed_alloc_chunk_init(bytes + used, len - used);

	ea->context = chunk;

	ea->malloc = eembed_chunk_malloc;
	ea->calloc = eembed_chunk_calloc;
	ea->realloc = eembed_chunk_realloc;
	ea->reallocarray = eembed_chunk_reallocarray;
	ea->free = eembed_chunk_free;

	return ea;
}

#if EEMBED_HOSTED
void *eembed_system_malloc(struct eembed_allocator *ea, size_t size)
{
	(void)ea;
	return size ? malloc(size) : NULL;
}

void *eembed_system_realloc(struct eembed_allocator *ea, void *ptr, size_t size)
{
	(void)ea;
	return realloc(ptr, size);
}

void *eembed_system_calloc(struct eembed_allocator *ea, size_t nmemb,
			   size_t size)
{
	(void)ea;
	return size ? calloc(nmemb, size) : NULL;
}

void *eembed_system_reallocarray(struct eembed_allocator *ea, void *ptr,
				 size_t nmemb, size_t size)
{
	size_t len;

	(void)ea;
#if (_DEFAULT_SOURCE || _GNU_SOURCE)
	(void)len;
	return reallocarray(ptr, nmemb, size);
#else
	/* this could overflow, but we elect to not care */
	len = nmemb * size;
	return len ? realloc(ptr, len) : NULL;
#endif
}

void eembed_system_free(struct eembed_allocator *ea, void *ptr)
{
	(void)ea;
	free(ptr);
}

struct eembed_allocator eembed_system_alloctor = {
	NULL,
	eembed_system_malloc,
	eembed_system_calloc,
	eembed_system_realloc,
	eembed_system_reallocarray,
	eembed_system_free
};

struct eembed_allocator *eembed_global_allocator = &eembed_system_alloctor;

#else

struct eembed_allocator *eembed_global_allocator = &eembed_null_chunk_allocator;

#endif
