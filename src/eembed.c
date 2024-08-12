/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020-2024 Eric Herman <eric@freesa.org> */

#include "eembed.h"

#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#if EEMBED_HOSTED
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifdef __linux__
#include <sys/random.h>
#endif
#endif

#if EEMBED_HOSTED
void (*eembed_exit)(int status) = exit;
void eembed_exit_failure(void)
{
	/* assertion failed, now we crash */
	eembed_exit(EXIT_FAILURE);
}

void (*eembed_assert_crash)(void) = eembed_exit_failure;
#else
void (*eembed_assert_crash)(void) = NULL;
#endif

void eembed_no_op_append_c(struct eembed_log *log, char c)
{
	(void)log;
	(void)c;
}

void eembed_no_op_append_s(struct eembed_log *log, const char *str)
{
	(void)log;
	(void)str;
}

void eembed_no_op_append_ul(struct eembed_log *log, uint64_t ul)
{
	(void)log;
	(void)ul;
}

void eembed_no_op_append_l(struct eembed_log *log, int64_t l)
{
	(void)log;
	(void)l;
}

void eembed_no_op_append_f(struct eembed_log *log, long double f)
{
	(void)log;
	(void)f;
}

void eembed_no_op_append_vp(struct eembed_log *log, const void *ptr)
{
	(void)log;
	(void)ptr;
}

void eembed_no_op_append_eol(struct eembed_log *log)
{
	(void)log;
}

struct eembed_log eembed_no_op_log = {
	NULL,
	eembed_no_op_append_c,
	eembed_no_op_append_s,
	eembed_no_op_append_ul,
	eembed_no_op_append_l,
	eembed_no_op_append_f,
	eembed_no_op_append_vp,
	eembed_no_op_append_eol,
};

struct eembed_log *eembed_null_log = &eembed_no_op_log;

void eembed_log_strbuf_append_s(struct eembed_log *log, const char *str)
{
	size_t used = 0;
	char *buf = NULL;
	size_t size = 0;
	size_t max = 0;
	struct eembed_str_buf *ctx = NULL;

	ctx = log ? (struct eembed_str_buf *)log->context : NULL;

	if (!ctx || !ctx->buf || !ctx->size) {
		return;
	}

	max = ctx->size - 1;
	used = eembed_strnlen(ctx->buf, ctx->size);
	if (used < max) {
		buf = ctx->buf + used;
		size = ctx->size - used;
		eembed_strcpy_safe(buf, size, str);
	}
	eembed_assert(ctx->buf[max] == '\0');
}

void eembed_log_str_append_c(struct eembed_log *log, char c)
{
	char str[2] = { '\0', '\0' };
	str[0] = c;
	log->append_s(log, str);
}

void eembed_log_str_append_ul(struct eembed_log *log, uint64_t ul)
{
	char str[25] = { '\0' };
	eembed_ulong_to_str(str, sizeof(str), ul);
	log->append_s(log, str);
}

void eembed_log_str_append_l(struct eembed_log *log, int64_t l)
{
	char str[25] = { '\0' };
	eembed_long_to_str(str, sizeof(str), l);
	log->append_s(log, str);
}

void eembed_log_str_append_f(struct eembed_log *log, long double f)
{
	char str[25] = { '\0' };
	eembed_float_to_str(str, sizeof(str), f);
	log->append_s(log, str);
}

void eembed_log_str_append_vp(struct eembed_log *log, const void *ptr)
{
	char str[25] = { '\0' };
	size_t zptr = (size_t)ptr;
	eembed_ulong_to_hex(str, sizeof(str), zptr);
	log->append_s(log, str);
}

void eembed_log_str_append_eol(struct eembed_log *log)
{
	log->append_s(log, "\n");
}

struct eembed_log *eembed_char_buf_log_init(struct eembed_log *log,
					    struct eembed_str_buf *ctx,
					    char *buf, size_t size)
{
	if (!log) {
		return NULL;
	}

	log->context = ctx;
	log->append_c = eembed_log_str_append_c;
	log->append_s = eembed_log_strbuf_append_s;
	log->append_ul = eembed_log_str_append_ul;
	log->append_l = eembed_log_str_append_l;
	log->append_f = eembed_log_str_append_f;
	log->append_vp = eembed_log_str_append_vp;
	log->append_eol = eembed_log_str_append_eol;

	if (!log->context) {
		return NULL;
	}

	ctx->buf = buf;
	if (!ctx->buf) {
		ctx->size = 0;
		return NULL;
	}

	ctx->size = size;

	return ctx->size ? log : NULL;
}

int eembed_strcpy_safe(char *buf, size_t size, const char *str)
{
	if (!buf || !size || !str) {
		return (str && str[0]) ? 1 : 0;
	}

	eembed_strncpy(buf, str, size);

	if (buf[size - 1]) {
		buf[size - 1] = '\0';
		return 1;
	}
	return 0;
}

#if EEMBED_HOSTED
/* function pointers for fprintf and fflush
 * it seems unlikely that these will be used often, but it is not high
 * over-head, and they are used in one test */
int (*eembed_fprintf)(FILE *stream, const char *format, ...) = fprintf;
int (*eembed_fflush)(FILE *stream) = fflush;

/* To avoid undefined behavior when writing to stderr, first flush
* stdout, thus ensuring stdout and stderr are "coordinated":
*
* "if two or more handles are used, and any one of them is a stream,
* the application shall ensure that their actions are coordinated as
* described below. If this is not done, the result is undefined."
*
* https://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_05_01
*/
static unsigned char eembed_posix1_2017_2_5_1_stdout_clean = 0;

void *eembed_stderr(void *p)
{
	(void)p;
	if (!eembed_posix1_2017_2_5_1_stdout_clean) {
		eembed_fflush(stdout);
		eembed_posix1_2017_2_5_1_stdout_clean = 1;
	}
	return stderr;
}

struct eembed_function_context eembed_stderr_context = { NULL, eembed_stderr };

void *eembed_stdout(void *p)
{
	(void)p;
	eembed_posix1_2017_2_5_1_stdout_clean = 0;
	return stdout;
}

struct eembed_function_context eembed_stdout_context = { NULL, eembed_stdout };

static FILE *eembed_log_get_stream_from_context(struct eembed_log *log)
{
	struct eembed_function_context *ctx
	    = (struct eembed_function_context *)log->context;
	return (FILE *)ctx->func(ctx->data);
}

void eembed_fprintf_append_c(struct eembed_log *log, char c)
{
	FILE *stream = eembed_log_get_stream_from_context(log);
	eembed_fprintf(stream, "%c", c);
}

void eembed_fprintf_append_s(struct eembed_log *log, const char *str)
{
	FILE *stream = eembed_log_get_stream_from_context(log);
	eembed_fprintf(stream, "%s", str);
}

void eembed_fprintf_append_ul(struct eembed_log *log, uint64_t ul)
{
	FILE *stream = eembed_log_get_stream_from_context(log);
	eembed_fprintf(stream, "%" PRIu64, ul);
}

void eembed_fprintf_append_l(struct eembed_log *log, int64_t l)
{
	FILE *stream = eembed_log_get_stream_from_context(log);
	eembed_fprintf(stream, "%" PRId64, l);
}

void eembed_fprintf_append_f(struct eembed_log *log, long double f)
{
	FILE *stream = eembed_log_get_stream_from_context(log);
	eembed_fprintf(stream, "%Lg", f);
}

void eembed_fprintf_append_vp(struct eembed_log *log, const void *ptr)
{
	FILE *stream = eembed_log_get_stream_from_context(log);
	eembed_fprintf(stream, "%p", ptr);
}

void eembed_fprintf_append_eol(struct eembed_log *log)
{
	FILE *stream = eembed_log_get_stream_from_context(log);
	eembed_fprintf(stream, "\n");
	eembed_fflush(stream);
	if (stream == stdout) {
		eembed_posix1_2017_2_5_1_stdout_clean = 1;
	}
}

struct eembed_log eembed_stderr_log = {
	&eembed_stderr_context,
	eembed_fprintf_append_c,
	eembed_fprintf_append_s,
	eembed_fprintf_append_ul,
	eembed_fprintf_append_l,
	eembed_fprintf_append_f,
	eembed_fprintf_append_vp,
	eembed_fprintf_append_eol
};

struct eembed_log *eembed_err_log = &eembed_stderr_log;

struct eembed_log eembed_stdout_log = {
	&eembed_stdout_context,
	eembed_fprintf_append_c,
	eembed_fprintf_append_s,
	eembed_fprintf_append_ul,
	eembed_fprintf_append_l,
	eembed_fprintf_append_f,
	eembed_fprintf_append_vp,
	eembed_fprintf_append_eol
};

struct eembed_log *eembed_out_log = &eembed_stdout_log;

char *eembed_long_to_str(char *buf, size_t size, int64_t l)
{
	char str[25];
	sprintf(str, "%" PRId64, l);
	return eembed_strcpy_safe(buf, size, str) ? NULL : buf;
}

char *eembed_ulong_to_str(char *buf, size_t size, uint64_t ul)
{
	char str[25];
	sprintf(str, "%" PRIu64, ul);
	return eembed_strcpy_safe(buf, size, str) ? NULL : buf;
}

char *eembed_ulong_to_hex(char *buf, size_t size, uint64_t ul)
{
	char str[25];
	sprintf(str, "%02" PRIX64, ul);
	return eembed_strcpy_safe(buf, size, str) ? NULL : buf;
}

char *eembed_float_to_str(char *buf, size_t size, long double f)
{
	char str[25];
	sprintf(str, "%Lg", f);
	return eembed_strcpy_safe(buf, size, str) ? NULL : buf;
}

#else /* #if EEMBED_HOSTED */

struct eembed_log *eembed_err_log = &eembed_no_op_log;
struct eembed_log *eembed_out_log = &eembed_no_op_log;

char *eembed_long_to_str(char *buf, size_t size, int64_t l)
{
	char *b;
	char *out;
	size_t bsize = 0;
	uint64_t ul = 0;

	if (!buf || !size) {
		return buf;
	}

	if (l < 0) {
		buf[0] = '-';
		b = buf + 1;
		bsize = size - 1;
		ul = (-l);
	} else {
		b = buf;
		bsize = size;
		ul = l;
	}
	if (bsize) {
		b[0] = '\0';
	}
	out = eembed_ulong_to_str(b, bsize, ul);
	return out ? buf : NULL;
}

char *eembed_ulong_to_str(char *buf, size_t size, uint64_t ul)
{
	char tmp[22];
	size_t i = 0;
	size_t j = 0;

	tmp[0] = '\0';

	if (!buf || !size) {
		return NULL;
	} else if (size == 1) {
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
	for (j = 0; i && j < size; ++j, --i) {
		buf[j] = tmp[i - 1];
	}

	buf[j < size ? j : size - 1] = '\0';

	return buf;
}

char *eembed_ulong_to_hex(char *buf, size_t size, uint64_t z)
{
	const size_t ul_bytes_size = sizeof(uint64_t);
	unsigned char ul_bytes[sizeof(uint64_t)];
	size_t i = 0;

	if (!buf || size < eembed_bytes_to_hex_min_buf(ul_bytes_size)) {
		return NULL;
	}

	for (i = 0; i < sizeof(uint64_t); ++i) {
		unsigned char byte = 0;
		byte = (0xFF & (z >> (8 * ((sizeof(uint64_t) - 1) - i))));
		ul_bytes[i] = byte;
	}
	return eembed_bytes_to_hex(buf, size, ul_bytes, ul_bytes_size);
}

char *eembed_bogus_float_to_str(char *buf, size_t size, long double f)
{
	size_t pos = 0;
	size_t avail = 0;
	size_t i = 0;
	size_t max_digits = 6;
	long double min_to_print = 0.000001;
	long double add_for_rounding = (min_to_print / 2.0);
	char c[2];
	unsigned long ul = 0;

	c[0] = 0;
	c[1] = 0;
	if (buf && size) {
		buf[0] = '\0';
		buf[size - 1] = '\0';
	}

	if (!buf || size < 2) {
		return NULL;
	}
	avail = size - 1;

	if (f != f) {
		eembed_strncat(buf, "nan", avail < 3 ? avail : 3);
		return avail < 3 ? NULL : buf;
	} else if (f == 0.0) {
		eembed_strncat(buf, "0.0", avail < 3 ? avail : 3);
		return avail < 3 ? NULL : buf;
	}

	if (f < 0.0) {
		eembed_strcat(buf, "-");
		f = -f;
		--avail;
		if (!avail) {
			return NULL;
		}
	}

	if (f == FLT_MAX || f == DBL_MAX || f == LDBL_MAX) {
		eembed_strncat(buf, "max", avail < 3 ? avail : 3);
		return avail < 3 ? NULL : buf;
	} else if (f == FLT_MIN || f == DBL_MIN || f == LDBL_MIN) {
		eembed_strncat(buf, "min", avail < 3 ? avail : 3);
		return avail < 3 ? NULL : buf;
	}

	if ((f / 2) == f) {
		eembed_strncat(buf, "inf", avail < 3 ? avail : 3);
		return avail < 3 ? NULL : buf;
	}

	if ((f - (long double)ULONG_MAX) >= 1.0) {
		eembed_strncat(buf, "big", avail < 3 ? avail : 3);
		return avail < 3 ? NULL : buf;
	} else if (f < min_to_print) {
		eembed_strncat(buf, "wee", avail < 3 ? avail : 3);
		return avail < 3 ? NULL : buf;
	}

	ul = (unsigned long)f;
	pos = eembed_strnlen(buf, size);
	avail = (pos > size) ? 0 : size - pos;
	eembed_ulong_to_str(buf + pos, avail, ul);
	f = f - (long double)ul;
	if (eembed_strnlen(buf, size) < (size - 1)) {
		eembed_strncat(buf, ".", 1);
	} else {
		return f > min_to_print ? NULL : buf;
	}
	pos = eembed_strnlen(buf, size);
	avail = (pos > (size - 1)) ? 0 : (size - 1) - pos;
	if (!avail) {
		return f > min_to_print ? NULL : buf;
	}

	if (max_digits > avail) {
		max_digits = avail;
	}
	f += add_for_rounding;
	for (i = 0; (i < max_digits) && (i == 0 || f >= min_to_print); ++i) {
		f = f * 10;
		ul = (unsigned long)f;
		c[0] = '0' + (ul % 10);
		eembed_strncat(buf, c, 1);
		f = f - ul;
		min_to_print = min_to_print * 10;
	}
	return buf;
}

char *eembed_float_to_str(char *buf, size_t size, long double f)
{
	return eembed_bogus_float_to_str(buf, size, f);
}
#endif

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

char *eembed_bytes_to_hex(char *buf, size_t buf_len, unsigned char *bytes,
			  size_t bytes_len)
{
	size_t i = 0;
	size_t pos = 0;

	eembed_assert(buf);

	if (buf_len < eembed_bytes_to_hex_min_buf(bytes_len)) {
		return NULL;
	}

	/*
	   buf[pos++] = '0';
	   buf[pos++] = 'x';
	 */

	for (i = 0; i < bytes_len; ++i) {
		char h = 0;
		char l = 0;
		eembed_byte_to_hex_chars(&h, &l, bytes[i]);
		buf[pos++] = h;
		buf[pos++] = l;
	}
	buf[pos] = '\0';
	return buf;
}

#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
static char *eembed_ignore_const_s(const char *str)
{
	return (char *)(str);
}

#if __GNUC__
#pragma GCC diagnostic pop
#endif

uint64_t eembed_diy_str_to_u64(const char *str, char **endptr, int pbase)
{
	uint64_t base = (uint64_t)pbase;
	uint64_t val = 0;
	unsigned end = 0;
	char ascii_end1 = 0;
	char ascii_end2 = 0;
	char ascii_end3 = 0;

	eembed_assert(str);
	eembed_assert(base >= 2);
	eembed_assert(base <= 36);

	if (base <= 10) {
		ascii_end1 = '0' + (base - 1);
		ascii_end2 = 0;
		ascii_end3 = 0;
	} else {
		ascii_end1 = '9';
		ascii_end2 = 'A' + (base - 11);
		ascii_end3 = 'a' + (base - 11);
	}

	end = 0;
	val = 0;
	while (*str && !end) {
		if (*str >= '0' && *str <= ascii_end1) {
			val = val * base;
			val = val + (*str - '0');
			++str;
		} else if (*str >= 'A' && *str <= ascii_end2) {
			val = val * base;
			val = val + 10 + (*str - 'A');
			++str;
		} else if (*str >= 'a' && *str <= ascii_end3) {
			val = val * base;
			val = val + 10 + (*str - 'a');
			++str;
		} else {
			end = 1;
		}
	}

	if (endptr) {
		*endptr = eembed_ignore_const_s(str);
	}
	return val;
}

int64_t eembed_diy_str_to_i64(const char *str, char **endptr, int base)
{
	int64_t val = 0;
	int negate = 0;

	eembed_assert(str);
	eembed_assert(base >= 2);
	eembed_assert(base <= 36);

	if (*str == '-') {
		str++;
		negate = -1;
	} else {
		negate = 0;
	}

	val = eembed_diy_str_to_u64(str, endptr, base);
	return negate ? -val : val;
}

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

int eembed_diy_memcmp(const void *a1, const void *a2, size_t n)
{
	size_t i;
	const unsigned char *s1;
	const unsigned char *s2;
	int d;

	/* glibc explodes on NULL */
	eembed_assert(a1);
	eembed_assert(a2);

	if (a1 == a2 || n == 0) {
		return 0;
	}

	s1 = (const unsigned char *)a1;
	s2 = (const unsigned char *)a2;
	for (i = 0; i < n; ++i) {
		d = s1[i] - s2[i];
		if (d) {
			return d;
		}
	}
	return 0;
}

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

void *eembed_diy_memcpy(void *d, const void *s, size_t n)
{
	return eembed_diy_memmove(d, s, n);
}

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

#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
char *eembed_diy_strcat(char *dest, const char *src)
{
	eembed_assert(src);
	return eembed_strncat(dest, src, eembed_strlen(src));
}

#if __GNUC__
#pragma GCC diagnostic pop
#endif

/*
	If src contains n or more bytes, strncat() writes n+1 bytes to dest
	(n from src plus  the  terminating null byte).  Therefore, the size
	of dest must be at least strlen(dest)+n+1.
*/
char *eembed_diy_strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = 0;
	size_t i = 0;

	/* glibc crashes on NULL */
	eembed_assert(dest);
	eembed_assert(src);

	dest_len = eembed_strlen(dest);
	while (i < n && src[i] != '\0') {
		dest[dest_len + i] = src[i];
		++i;
	}
	dest[dest_len + i] = '\0';

	return dest;
}

int eembed_diy_strcmp(const char *s1, const char *s2)
{
	return eembed_strncmp(s1, s2, 1 + eembed_strlen(s1));
}

int eembed_diy_strncmp(const char *s1, const char *s2, size_t max_len)
{
	size_t i;
	int d;

	/* glibc explodes on NULL, do all libc memcpy? */
	eembed_assert(s1);
	eembed_assert(s2);

	if (s1 == s2 || max_len == 0) {
		return 0;
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

#if __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif
char *eembed_diy_strcpy(char *dest, const char *src)
{
	eembed_assert(src);
	return eembed_strncpy(dest, src, 1 + eembed_strlen(src));
}

#if __GNUC__
#pragma GCC diagnostic pop
#endif

/*
   If the length of src is less than n, strncpy() writes additional null
   bytes to dest to ensure that a total of n bytes are written.
*/
char *eembed_diy_strncpy(char *dest, const char *src, size_t n)
{
	size_t i = 0;

	/* glibc crashes on NULL */
	eembed_assert(dest);
	eembed_assert(src);

	while (i < n && src[i] != '\0') {
		dest[i] = src[i];
		++i;
	}
	if (i < n) {
		eembed_memset(dest + i, 0x00, n - i);
	}

	return dest;
}

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

size_t eembed_diy_strlen(const char *s)
{
	return eembed_strnlen(s, SIZE_MAX);
}

char *eembed_diy_strstr(const char *haystack, const char *needle)
{
	size_t i = 0;
	size_t j = 0;
	size_t found = 0;
	size_t nlen = 0;
	size_t hlen = 0;

	/* glibc crashes on NULL */
	eembed_assert(haystack);
	eembed_assert(needle);

	nlen = eembed_strlen(needle);
	if (!nlen) {
		return eembed_ignore_const_s(haystack);
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
			return eembed_ignore_const_s(haystack + i);
		}
	}
	return NULL;
}

/* seed is initialized to any arbitrary number */
#ifndef Eembed_lcg_pseudo_random_seed
#define Eembed_lcg_pseudo_random_seed 15541
#endif
uint32_t eembed_lcg_pseudo_random_last = Eembed_lcg_pseudo_random_seed;

/* https://en.wikipedia.org/wiki/Linear_congruential_generator */
int eembed_lcg_pseudo_random_bytes(unsigned char *buf, size_t size)
{
	/* constants from VMS's MTH$RANDOM, old versions of glibc */
	/* see: wiki/Linear_congruential_generator#Parameters_in_common_use */
	const uint32_t a = 69069;
	const uint32_t c = 1;

	/* Note that while signed integer overflow is undefined, unsigned */
	/* is defined, thus it should be safe to use uint32_t */
	/* See section 6.2.5:9 (Types) in the standard: */
	/* https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf */

	size_t i = 0;
	size_t j = 0;

	while (i < size) {
		uint32_t x = (eembed_lcg_pseudo_random_last * a) + c;
		for (j = 0; j < sizeof(x) && i < size; ++j) {
			buf[i++] = (x >> (j * 8));
		}
		eembed_lcg_pseudo_random_last = x;
	}

	return 0;
}

ssize_t eembed_diy_getrandom(void *buf, size_t bufsize, unsigned int flags)
{
	(void)flags;
	return eembed_lcg_pseudo_random_bytes((unsigned char *)buf, bufsize);
}

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))

#ifdef __linux__
ssize_t (*eembed_system_getrandom)(void *buf, size_t buflen,
				   unsigned int flags) = getrandom;
#else
ssize_t (*eembed_system_getrandom)(void *buf, size_t buflen,
				   unsigned int flags) = eembed_diy_getrandom;
#endif

int eembed_system_getrandom_bytes(unsigned char *buf, size_t buf_size)
{
	ssize_t bytes_read = 0;

#ifdef SSIZE_MAX
	eembed_assert(buf_size <= SSIZE_MAX);
#endif

	while (bytes_read < ((ssize_t)buf_size)) {
		unsigned char *dest = buf + bytes_read;
		size_t size = buf_size - bytes_read;
		const unsigned int flags = 0;
		ssize_t result = eembed_system_getrandom(dest, size, flags);
		if (result < 0) {
			int save_errno = errno;
			struct eembed_log *log = eembed_err_log;
			if (log) {
				log->append_s(log, __FILE__);
				log->append_s(log, ":");
				log->append_ul(log, __LINE__);
				log->append_s(log, ": ");
				log->append_s(log, "getrandom() failed");
				log->append_s(log, ", errno: ");
				log->append_l(log, save_errno);
				log->append_s(log, " (");
				log->append_s(log, strerror(save_errno));
				log->append_s(log, ")");
				log->append_eol(log);
			}
			return save_errno ? save_errno : 1;
		}
		bytes_read += result;
	}
	return 0;
}
#endif /* #if (EEMBED_HOSTED && (!(FAUX_FREESTANDING))) */

struct eembed_alloc_chunk {
	unsigned char *start;
	size_t available_size;
	unsigned char in_use;
	struct eembed_alloc_chunk *prev;
	struct eembed_alloc_chunk *next;
};

static struct eembed_alloc_chunk *eembed_alloc_chunk_init(unsigned char *bytes,
							  size_t available_size)
{
	struct eembed_alloc_chunk *chunk = (struct eembed_alloc_chunk *)bytes;
	size_t size = eembed_align(sizeof(struct eembed_alloc_chunk));

	eembed_assert(bytes);
	eembed_assert(available_size >= size);

	chunk->start = bytes + size;
	chunk->available_size = available_size - size;
	chunk->in_use = 0;
	chunk->prev = (struct eembed_alloc_chunk *)NULL;
	chunk->next = (struct eembed_alloc_chunk *)NULL;

	return chunk;
}

static void eembed_alloc_chunk_split(struct eembed_alloc_chunk *from,
				     size_t request)
{
	size_t remaining_available_size = 0;
	size_t aligned_request = 0;
	struct eembed_alloc_chunk *orig_next = NULL;
	size_t min_size = eembed_align(sizeof(struct eembed_alloc_chunk)) +
	    eembed_align(sizeof(size_t)) + eembed_align(1);

	aligned_request = eembed_align(request);
	from->in_use = 1;

	if ((aligned_request + min_size) >= from->available_size) {
		return;
	}

	remaining_available_size = from->available_size - aligned_request;
	eembed_assert(remaining_available_size >= min_size);

	from->available_size = aligned_request;
	orig_next = from->next;
	from->next =
	    eembed_alloc_chunk_init((from->start + from->available_size),
				    remaining_available_size);
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

	if (!size) {
		return NULL;
	}

	while (chunk != NULL) {
		if (chunk->in_use == 0) {
			if (chunk->available_size >= size) {
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
	size_t additional_available_size = 0;

	next = chunk->next;
	if (!next) {
		return;
	}

	if (next->in_use) {
		return;
	}

	chunk->next = next->next;
	additional_available_size =
	    eembed_align(sizeof(struct eembed_alloc_chunk)) +
	    next->available_size;
	chunk->available_size += additional_available_size;
	if (chunk->next) {
		chunk->next->prev = chunk;
	}
	if (!chunk->in_use) {
		eembed_memset(chunk->start, 0x00, chunk->available_size);
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

	eembed_assert(chunk);

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
			old_size = chunk->available_size;
		} else {
			chunk = chunk->next;
		}
	}

	eembed_assert(found);
#ifdef NDEBUG
	if (!found) {
		return NULL;
	}
#endif

	if (old_size >= size) {
		eembed_alloc_chunk_split(chunk, size);
		return ptr;
	}

	if (chunk->next && chunk->next->in_use == 0) {
		eembed_alloc_chunk_join_next(chunk);
		if (chunk->available_size >= size) {
			eembed_memset(((unsigned char *)ptr) + old_size, 0x00,
				      chunk->available_size - old_size);
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
			  size_t msize)
{
	/* this could overflow, but we elect to not care */
	size_t size = nmemb * msize;
	void *ptr = NULL;
	ptr = size ? ea->malloc(ea, size) : NULL;
	if (ptr) {
		eembed_memset(ptr, 0x00, size);
	}
	return ptr;
}

void *eembed_chunk_reallocarray(struct eembed_allocator *ea, void *ptr,
				size_t nmemb, size_t msize)
{
	/* this could overflow, but we elect to not care */
	size_t size = nmemb * msize;
	return size ? ea->realloc(ea, ptr, size) : NULL;
}

void eembed_chunk_free(struct eembed_allocator *ea, void *ptr)
{
	struct eembed_alloc_chunk *chunk =
	    (struct eembed_alloc_chunk *)ea->context;
	size_t size = 0;

	if (!ptr) {
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
			size = chunk->available_size;
			eembed_memset(chunk->start, 0x00, size);
			return;
		}
		chunk = chunk->next;
	}
}

void eembed_bytes_allocator_dump(struct eembed_log *log,
				 struct eembed_allocator *bytes_allocator)
{
	void *ctx = (bytes_allocator) ? bytes_allocator->context : NULL;
	struct eembed_alloc_chunk *chunk = (struct eembed_alloc_chunk *)ctx;
	char hexaddr[2 + (2 * sizeof(uint64_t)) + 1];
	eembed_memset(hexaddr, 0x00, sizeof(hexaddr));
	while (chunk) {
		eembed_ulong_to_hex(hexaddr, sizeof(hexaddr), (uint64_t)chunk);
		log->append_s(log, hexaddr);
		log->append_s(log, ": {");
		log->append_eol(log);

		eembed_ulong_to_hex(hexaddr, sizeof(hexaddr),
				    (uint64_t)chunk->start);
		log->append_s(log, "\tstart: ");
		log->append_s(log, hexaddr);
		log->append_c(log, ',');
		log->append_eol(log);

		log->append_s(log, "\tavailable_size: ");
		log->append_ul(log, (uint64_t)chunk->available_size);
		log->append_c(log, ',');
		log->append_eol(log);

		log->append_s(log, "\tin use: ");
		log->append_ul(log, (uint64_t)chunk->in_use);
		log->append_c(log, ',');
		log->append_eol(log);

		eembed_ulong_to_hex(hexaddr, sizeof(hexaddr),
				    (uint64_t)chunk->prev);
		log->append_s(log, "\tprev: ");
		log->append_s(log, hexaddr);
		log->append_c(log, ',');
		log->append_eol(log);

		eembed_ulong_to_hex(hexaddr, sizeof(hexaddr),
				    (uint64_t)chunk->next);
		log->append_s(log, "\tnext: ");
		log->append_s(log, hexaddr);
		log->append_eol(log);
		log->append_c(log, '}');
		log->append_eol(log);

		chunk = chunk->next;
	}
}

static size_t eembed_bytes_allocator_visual_inner(struct eembed_log *log,
						  size_t pos, const char *str,
						  char fill,
						  size_t size, size_t width)
{
	size_t i;
	int str_end_found = !str;
	for (i = 0; i < size; ++i) {
		char c = (str_end_found) ? fill : str[i];
		if (!c) {
			c = '0';
			str_end_found = 1;
		}
		log->append_c(log, c);
		++pos;
		if ((pos % width) == 0) {
			log->append_eol(log);
		}
	}
	return pos;
}

void eembed_bytes_allocator_visual(struct eembed_log *log,
				   struct eembed_allocator *bytes_allocator,
				   int strinify_contents, size_t width)
{
	void *ctx = (bytes_allocator) ? bytes_allocator->context : NULL;
	struct eembed_alloc_chunk *chunk = (struct eembed_alloc_chunk *)ctx;
	const char *str = NULL;
	char fill = 'A';
	size_t size = eembed_align(sizeof(struct eembed_allocator));
	size_t pos = 0;

	pos =
	    eembed_bytes_allocator_visual_inner(log, pos, str, fill, size,
						width);

	while (chunk) {
		str = NULL;
		fill = 'O';
		size = eembed_align(sizeof(struct eembed_alloc_chunk));
		pos =
		    eembed_bytes_allocator_visual_inner(log, pos, str, fill,
							size, width);
		size = chunk->available_size;
		if (chunk->in_use) {
			if (strinify_contents) {
				str = (const char *)chunk->start;
			}
			fill = '_';
		} else {
			fill = '.';
		}
		pos =
		    eembed_bytes_allocator_visual_inner(log, pos, str, fill,
							size, width);

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

const size_t eembed_bytes_allocator_min_buf_size =
eembed_align(sizeof(struct eembed_allocator)) +
eembed_align(sizeof(struct eembed_alloc_chunk)) +
eembed_align(sizeof(size_t)) + eembed_align(1);

struct eembed_allocator *eembed_bytes_allocator(unsigned char *bytes,
						size_t size)
{
	struct eembed_allocator *ea = NULL;
	struct eembed_alloc_chunk *chunk = NULL;
	size_t used = 0;

	eembed_assert(bytes);
	eembed_assert(size >= eembed_bytes_allocator_min_buf_size);

	ea = (struct eembed_allocator *)bytes;
	used = eembed_align(sizeof(struct eembed_allocator));

	chunk = eembed_alloc_chunk_init(bytes + used, size - used);

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
				 size_t nmemb, size_t msize)
{
	size_t size;

	(void)ea;
#if (_DEFAULT_SOURCE || _GNU_SOURCE)
	(void)size;
	return reallocarray(ptr, nmemb, msize);
#else
	/* this could overflow, but we elect to not care */
	size = nmemb * msize;
	return size ? realloc(ptr, size) : NULL;
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

#if (FAUX_FREESTANDING)
int printf(const char *format, ...);
void eembed_faux_freestanding_append_s(struct eembed_log *log, const char *str)
{
	(void)log;
	printf("%s", str);
}

struct eembed_log eembed_faux_freestanding_log = {
	NULL,
	eembed_log_str_append_c,
	eembed_faux_freestanding_append_s,
	eembed_log_str_append_ul,
	eembed_log_str_append_l,
	eembed_log_str_append_f,
	eembed_log_str_append_vp,
	eembed_log_str_append_eol,
};

void eembed_system_print_init(void)
{
	if (FAUX_FREESTANDING) {
		eembed_out_log = &eembed_faux_freestanding_log;
		eembed_err_log = &eembed_faux_freestanding_log;
	}
}
#else
void eembed_system_print_init(void)
{
}

#endif
