/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed.h */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#ifndef EEMBED_H
#define EEMBED_H

#ifdef __cplusplus
#define Eembed_begin_C_functions extern "C" {
#define Eembed_end_C_functions }
#else
#define Eembed_begin_C_functions
#define Eembed_end_C_functions
#endif

Eembed_begin_C_functions
#undef Eembed_begin_C_functions
#include <stddef.h>		/* for size_t */
#include <stdint.h>		/* for uint64_t */
/* these wrap stdout in a hosted, NULL in freesetanding */
extern void (*eembed_system_print)(const char *str);
extern void (*eembed_system_println)(void);

char *eembed_long_to_str(char *buf, size_t len, long l);
char *eembed_ulong_to_str(char *buf, size_t len, unsigned long ul);
char *eembed_ulong_to_hex(char *buf, size_t len, unsigned long z);
char *eembed_float_to_str(char *buf, size_t len, double f);

struct eembed_log;
extern struct eembed_log *eembed_err_log;

struct eembed_log {
	void *context;
	void (*append_s)(struct eembed_log * log, const char *str);
	void (*append_ul)(struct eembed_log * log, unsigned long ul);
	void (*append_z)(struct eembed_log * log, size_t z);
	void (*append_l)(struct eembed_log * log, long l);
	void (*append_f)(struct eembed_log * log, double f);
	void (*append_vp)(struct eembed_log * log, const void *ptr);
	void (*append_eol)(struct eembed_log * log);
};

struct eembed_buf_context {
	char *buf;
	size_t len;
};

struct eembed_log *eembed_char_buf_log_init(struct eembed_log *log,
					    struct eembed_buf_context *ctx,
					    char *buf, size_t len);

extern void (*eembed_assert_crash)(void);

extern int (*eembed_memcmp)(const void *s1, const void *s2, size_t n);

extern void *(*eembed_memcpy)(void *dest, const void *src, size_t n);
extern void *(*eembed_memmove)(void *dest, const void *src, size_t n);

extern void *(*eembed_memset)(void *dest, int val, size_t n);

extern char *(*eembed_strcat)(char *dest, const char *src);
extern char *(*eembed_strncat)(char *dest, const char *src, size_t n);

extern int (*eembed_strcmp)(const char *s1, const char *s2);
extern int (*eembed_strncmp)(const char *s1, const char *s2, size_t n);

extern char *(*eembed_strcpy)(char *dest, const char *src);
extern char *(*eembed_strncpy)(char *dest, const char *src, size_t n);

extern size_t (*eembed_strlen)(const char *s);
extern size_t (*eembed_strnlen)(const char *s, size_t maxlen);

extern char *(*eembed_strstr)(const char *haystack, const char *needle);

Eembed_end_C_functions
#undef Eembed_end_C_functions
#endif /* EEMBED_H */
