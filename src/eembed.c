/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include "eembed.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#if EEMBED_HOSTED
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#if EEMBED_HOSTED
void eembed_exit_failure(void)
{
	exit(EXIT_FAILURE);
}

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
	struct eembed_str_buf *ctx = NULL;
	size_t used = 0;
	char *buf = NULL;
	size_t len = 0;
	va_list ap;

	ctx = log ? (struct eembed_str_buf *)log->context : NULL;
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
	return eembed_sprintf_to_str(buf, len, "0x%02lx", ul);
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

char *eembed_long_to_str(char *buf, size_t len, long l)
{
	char *b;
	char *out;
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
	out = eembed_ulong_to_str(b, blen, ul);
	return out ? buf : NULL;
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

#if (EEMBED_HOSTED && (_DEFAULT_SOURCE || _GNU_SOURCE))
#define EEMBED_HAVE_HOSTED_REALLOC_ARRAY 1
#else
#define EEMBED_HAVE_HOSTED_REALLOC_ARRAY 0
void *eembed_diy_reallocarray(void *ptr, size_t nmemb, size_t size)
{
	return eembed_realloc(ptr, (nmemb * size));
}

void *(*eembed_reallocarray)(void *ptr, size_t nmemb, size_t size) =
    eembed_diy_reallocarray;
#endif

#if EEMBED_HOSTED

void *(*eembed_malloc)(size_t size) = malloc;
void *(*eembed_realloc)(void *ptr, size_t size) = realloc;
void *(*eembed_calloc)(size_t nmemb, size_t size) = calloc;
void (*eembed_free)(void *ptr) = free;
#if EEMBED_HAVE_HOSTED_REALLOC_ARRAY
void *(*eembed_reallocarray)(void *ptr, size_t nmemb, size_t size) =
    reallocarray;
#endif

#else

void *eembed_noop_malloc(size_t size)
{
	(void)size;
	return NULL;
}

void *(*eembed_malloc)(size_t size) = eembed_noop_malloc;

/* It is a programmer error if this is called with a non-NULL pointer */
void *eembed_noop_realloc(void *ptr, size_t size)
{
	(void)ptr;
	(void)size;
	return NULL;
}

void *(*eembed_realloc)(void *ptr, size_t size) = eembed_noop_realloc;

void *eembed_noop_calloc(size_t nmemb, size_t size)
{
	(void)nmemb;
	(void)size;
	return NULL;
}

void *(*eembed_calloc)(size_t nmemb, size_t size) = eembed_noop_calloc;

/* It is a programmer error if this is called with a non-NULL pointer */
void eembed_noop_free(void *ptr)
{
	(void)ptr;
}

void (*eembed_free)(void *ptr) = eembed_noop_free;
#endif

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

	if (!a1 || !a2) {
		return a1 ? 1 : -1;
	}

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
	if (!s1 || !s2) {
		return s1 ? 1 : -1;
	}
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
	size_t i, j, found, len;

	if (!haystack) {
		return NULL;
	}

	len = 0;
	if (!needle || !needle[0]) {
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

char *(*eembed_strstr)(const char *haystack, const char *needle) =
    eembed_diy_strstr;
#endif

void *eembed_system_malloc(struct eembed_allocator *ea, size_t size)
{
	(void)ea;
	return eembed_malloc(size);
}

void *eembed_system_realloc(struct eembed_allocator *ea, void *ptr, size_t size)
{
	(void)ea;
	return eembed_realloc(ptr, size);
}

void *eembed_system_calloc(struct eembed_allocator *ea, size_t nmemb,
			   size_t size)
{
	(void)ea;
	return eembed_calloc(nmemb, size);
}

void *eembed_system_reallocarray(struct eembed_allocator *ea, void *ptr,
				 size_t nmemb, size_t size)
{
	(void)ea;
	return eembed_realloc(ptr, nmemb * size);
}

void eembed_system_free(struct eembed_allocator *ea, void *ptr)
{
	(void)ea;
	eembed_free(ptr);
}

struct eembed_allocator eembed_system_alloctor = {
	NULL,
	eembed_system_malloc,
	eembed_system_calloc,
	eembed_system_realloc,
	eembed_system_reallocarray,
	eembed_system_free
};

struct eembed_allocator *eembed_global_alloctor = &eembed_system_alloctor;
