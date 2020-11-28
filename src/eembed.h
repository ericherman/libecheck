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
	void (*append_s)(struct eembed_log *log, const char *str);
	void (*append_ul)(struct eembed_log *log, unsigned long ul);
	void (*append_z)(struct eembed_log *log, size_t z);
	void (*append_l)(struct eembed_log *log, long l);
	void (*append_f)(struct eembed_log *log, double f);
	void (*append_vp)(struct eembed_log *log, const void *ptr);
	void (*append_eol)(struct eembed_log *log);
};

struct eembed_str_buf {
	char *buf;
	size_t len;
};

struct eembed_log *eembed_char_buf_log_init(struct eembed_log *log,
					    struct eembed_str_buf *ctx,
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

/* default to "no op" versions if not EEMBED_HOSTED */
extern void *(*eembed_calloc)(size_t nmemb, size_t size);
extern void *(*eembed_malloc)(size_t size);
extern void *(*eembed_realloc)(void *ptr, size_t size);
extern void *(*eembed_reallocarray)(void *ptr, size_t nmemb, size_t size);
extern void (*eembed_free)(void *ptr);

struct eembed_allocator;
extern struct eembed_allocator *eembed_global_alloctor;

struct eembed_allocator {
	void *context;
	void *(*malloc)(struct eembed_allocator *ea, size_t size);
	void *(*calloc)(struct eembed_allocator *ea, size_t nmemb, size_t size);
	void *(*realloc)(struct eembed_allocator *ea, void *ptr, size_t size);
	void *(*reallocarray)(struct eembed_allocator *ea, void *ptr,
			      size_t nmemb, size_t size);
	void (*free)(struct eembed_allocator *ea, void *ptr);
};

#ifndef EEMBED_HOSTED
#ifdef ARDUINO
#define EEMBED_HOSTED 0
#endif
#endif

#ifndef EEMBED_HOSTED
/*
 __STDC_HOSTED__
 The integer constant 1 if the implementation is a hosted
 implementation or the integer constant 0 if it is not.

 C99 standard (section 6.10.8):
 http://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf

 C++11 standard (section 16.8):
 http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf

 "The standard also defines two environments for programs, a
 freestanding environment, required of all implementations and which
 may not have library facilities beyond those required of
 freestanding implementations, where the handling of program startup
 and termination are implementation-defined; and a hosted
 environment, which is not required, in which all the library
 facilities are provided and startup is through a function int main
 (void) or int main (int, char *[]). An OS kernel is an example of a
 program running in a freestanding environment; a program using the
 facilities of an operating system is an example of a program
 running in a hosted environment."
 https://gcc.gnu.org/onlinedocs/gcc/Standards.html
*/
#ifdef __STDC_HOSTED__
#define EEMBED_HOSTED __STDC_HOSTED__
#endif
#endif

#ifndef EEMBED_HOSTED
#error "EEMBED_HOSTED is not defined"
#endif

#ifndef EEMBED_NOP
#define EEMBED_NOP() do { ((void)0); } while (0)
#endif

#ifndef eembed_assert
#if EEMBED_HOSTED
#include <assert.h>
#define eembed_assert(expression) assert(expression)
#else
#ifdef NDEBUG
#define eembed_assert(expression) EEMBED_NOP()
#else
#define eembed_assert(expression) \
	do { \
		if (expression) { \
			EEMBED_NOP(); \
		} else { \
			char _eembed_itoa_buf[25]; \
			eembed_ulong_to_str(_eembed_itoa_buf, 25, __LINE__); \
			eembed_system_print(__FILE__); \
			eembed_system_print(":"); \
			eembed_system_print(_eembed_itoa_buf); \
			eembed_system_print(": ASSERTION assert("); \
			eembed_system_print(#expression); \
			eembed_system_print(") FAILED"); \
			eembed_system_println(); \
			eembed_assert_crash(); \
		} \
	} while (0)
#endif
#endif
#endif

Eembed_end_C_functions
#undef Eembed_end_C_functions
#endif /* EEMBED_H */
