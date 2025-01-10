/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed.h */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020-2025 Eric Herman <eric@freesa.org> */

#ifndef EEMBED_H
#define EEMBED_H

#ifdef ARDUINO
#include "eembed-arduino.h"
#endif

#ifndef ARDUINO
#ifdef __cplusplus
#define Eembed_begin_C_functions extern "C" {
#define Eembed_end_C_functions }
#endif
#endif
#ifndef Eembed_begin_C_functions
#define Eembed_begin_C_functions
#endif
#ifndef Eembed_end_C_functions
#define Eembed_end_C_functions
#endif

Eembed_begin_C_functions
#undef Eembed_begin_C_functions
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
#ifndef EEMBED_HOSTED
#ifdef __STDC_HOSTED__
#define EEMBED_HOSTED __STDC_HOSTED__
#endif
#endif
#ifndef EEMBED_HOSTED
#error "EEMBED_HOSTED is not defined"
#endif
#ifndef FAUX_FREESTANDING
#define FAUX_FREESTANDING 0
#endif
/***************************************************************************\
 * The headers required by freestanding environments, are mostly those that
 * define macros and types.
\***************************************************************************/
#include <float.h>		/* FLT_EPSILON */
#include <limits.h>		/* UINT_MAX */
#if __STDC_VERSION__ >= 201112L
#include <stdalign.h>		/* algignof, alignas */
#endif
#include <stdarg.h>		/* va_list, va_start, va_end */
#include <stddef.h>		/* size_t */
#include <stdint.h>		/* uint64_t */
/* #include <stdbool.h> */
/* #include <iso646.h */
/* #include <stdnoreturn.h> */
/***************************************************************************\
 * With at least one toolchain, we discovered ssize_t was not defined, and
 * thus neither was SSIZE_MAX, but we can work around that
\***************************************************************************/
/* Check if ssize_t is not defined */
#if EEMBED_HOSTED
#ifdef _POSIX_VERSION
#include <sys/types.h>
/* older POSIX systems may define ssize_t in <unistd.h> */
#include <unistd.h>
#endif
#endif
#ifndef _SSIZE_T_DEFINED
#ifndef _SSIZE_T_DEFINED_
#ifndef __ssize_t_defined
#ifndef __SSIZE_T_TYPE
/* Define ssize_t as intptr_t if it is not defined */
#define __SSIZE_T_TYPE intptr_t
#endif
    typedef __SSIZE_T_TYPE ssize_t;

/* Define the macros to indicate ssize_t is defined */
#define _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED_
#define __ssize_t_defined

#endif /* #ifndef __ssize_t_defined */
#endif /* #ifndef _SSIZE_T_DEFINED_ */
#endif /* #ifndef _SSIZE_T_DEFINED */
/* End of ssize_t defined */

#ifndef EEMBED_SSIZE_MAX
#ifdef SSIZE_MAX
#define EEMBED_SSIZE_MAX SSIZE_MAX
#elif defined(INTPTR_MAX) && (INTPTR_MAX < SIZE_MAX)
#define EEMBED_SSIZE_MAX INTPTR_MAX
#else
#define EEMBED_SSIZE_MAX (SIZE_MAX / 2)
#endif
#endif

/***************************************************************************\
 * The biggest variation between __STDC_HOSTED__ and Freestanding platforms
 * is the lack of FILE functions like fprintf(stderr, fmt, ...).
 *
 * These #defines provide a way for calling code to do simple printing in a
 * form that will work in both environments.
\***************************************************************************/

#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))

#include <stdio.h>
#include <stdlib.h>

#ifndef eembed_out
#define eembed_out stdout
#endif

#ifndef eembed_err
#define eembed_err stderr
#endif

#ifndef print_s
#define print_s(s) printf("%s", s)
#endif

#ifndef print_err_s
#define print_err_s(s) fprintf(eembed_err, "%s", s)
#endif

#ifndef print_i
#define print_i(i) printf("%d", i)
#endif

#ifndef print_err_i
#define print_err_i(i) fprintf(eembed_err, "%d", i)
#endif

#ifndef print_l
#define print_l(l) printf("%ld", l)
#endif

#ifndef print_err_l
#define print_err_l(l) fprintf(eembed_err, "%ld", l)
#endif

#ifndef print_ll
#define print_ll(ll) printf("%ll", ll)
#endif

#ifndef print_err_ll
#define print_err_ll(ll) fprintf(eembed_err, "%ll", ll)
#endif

#ifndef print_u
#define print_u(u) printf("%u", u)
#endif

#ifndef print_err_u
#define print_err_u(u) fprintf(eembed_err, "%u", u)
#endif

#ifndef print_ul
#define print_ul(ul) printf("%lu", ul)
#endif

#ifndef print_err_ul
#define print_err_ul(ul) fprintf(eembed_err, "%lu", ul)
#endif

#ifndef print_ull
#define print_ull(ull) printf("%llu", ull)
#endif

#ifndef print_err_ull
#define print_err_ull(ull) fprintf(eembed_err, "%llu", ull)
#endif

#ifndef print_z
#define print_z(z) printf("%zu", z)
#endif

#ifndef print_err_z
#define print_err_z(z) fprintf(eembed_err, "%zu", z)
#endif

#ifndef print_c
#define print_c(c) printf("%c", c)
#endif

#ifndef print_err_c
#define print_err_c(c) fprintf(eembed_err, "%c", c)
#endif

#ifndef print_f
#define print_f(f) printf("%g", f)
#endif

#ifndef print_err_f
#define print_err_f(f) fprintf(eembed_err, "%g", f)
#endif

#ifndef print_lf
#define print_lf(f) printf("%Lg", f)
#endif

#ifndef print_err_lf
#define print_err_lf(f) fprintf(eembed_err, "%Lg", f)
#endif

#ifndef print_fd
#define print_fd(f,d) printf("%."#d"f", f)
#endif

#ifndef print_err_fd
#define print_err_fd(f,d) fprintf(eembed_err, "%."#d"f", f)
#endif

#ifndef print_lfd
#define print_lfd(lf,d) printf("%."#d"L", lf)
#endif

#ifndef print_err_lfd
#define print_err_lfd(lf,d) fprintf(eembed_err, "%."#d"L", lf)
#endif

#ifndef print_x
#define print_x(x) printf("%x", x)
#endif

#ifndef print_err_x
#define print_err_x(x) fprintf(eembed_err, "%x", x)
#endif

#ifndef print_lx
#define print_lx(lx) printf("%lx", lx)
#endif

#ifndef print_err_lx
#define print_err_lx(lx) fprintf(eembed_err, "%lx", lx)
#endif

#ifndef print_stream_u8_bin
#define print_stream_u8_bin(stream, u8) \
	do { \
		uint8_t eembed_tmp_u8 = (u8); \
		fprintf(stream, "%c%c%c%c%c%c%c%c", \
			(eembed_tmp_u8 & (1 << 7)) ? '1' : '0', \
			(eembed_tmp_u8 & (1 << 6)) ? '1' : '0', \
			(eembed_tmp_u8 & (1 << 5)) ? '1' : '0', \
			(eembed_tmp_u8 & (1 << 4)) ? '1' : '0', \
			(eembed_tmp_u8 & (1 << 3)) ? '1' : '0', \
			(eembed_tmp_u8 & (1 << 2)) ? '1' : '0', \
			(eembed_tmp_u8 & (1 << 1)) ? '1' : '0', \
			(eembed_tmp_u8 & (1 << 0)) ? '1' : '0'); \
	} while (0)
#endif

#ifndef print_u8_bin
#define print_u8_bin(u8) print_stream_u8_bin(eembed_out, u8)
#endif

#ifndef print_err_u8_bin
#define print_err_u8_bin(u8) print_stream_u8_bin(eembed_err, u8)
#endif

#ifndef print_stream_u16_bin
#define print_stream_u16_bin(stream, u16) \
	do { \
		uint16_t eembed_tmp_u16 = (u16); \
		print_stream_u8_bin(stream, 0xFF & (eembed_tmp_u16 >> 8)); \
		print_stream_u8_bin(stream, 0xFF & eembed_tmp_u16); \
	} while (0)
#endif

#ifndef print_u16_bin
#define print_u16_bin(u16) print_stream_u16_bin(eembed_out, u16)
#endif

#ifndef print_err_u16_bin
#define print_err_u16_bin(u16) print_stream_u16_bin(eembed_err, u16)
#endif

#ifndef print_stream_u32_bin
#define print_stream_u32_bin(stream, u32) \
	do { \
		uint32_t eembed_tmp_u32 = (u32); \
		print_stream_u16_bin(stream, 0xFFFF & (eembed_tmp_u32 >> 16)); \
		print_stream_u16_bin(stream, 0xFFFF & eembed_tmp_u32); \
	} while (0)
#endif

#ifndef print_u32_bin
#define print_u32_bin(u32) print_stream_u32_bin(eembed_out, u32)
#endif

#ifndef print_err_u32_bin
#define print_err_u32_bin(u32) print_stream_u32_bin(eembed_err, u32)
#endif

#ifndef print_stream_u64_bin
#define print_stream_u64_bin(s, u64) \
	do { \
		uint64_t eembed_tmp_u64 = (u64); \
		print_stream_u32_bin(s, 0xFFFFFFFF & (eembed_tmp_u64 >> 32)); \
		print_stream_u32_bin(s, 0xFFFFFFFF & eembed_tmp_u64); \
	} while (0)
#endif

#ifndef print_u64_bin
#define print_u64_bin(u64) print_stream_u64_bin(eembed_out, u64)
#endif

#ifndef print_err_u64_bin
#define print_err_u64_bin(u64) print_stream_u64_bin(eembed_err, u64)
#endif

#ifndef print_u8_hex
#define print_u8_hex(u8) printf("%02x", 0xFF & (u8))
#endif

#ifndef print_err_u8_hex
#define print_err_u8_hex(u8) fprintf(eembed_err, "%02x", 0xFF & (u8))
#endif

#ifndef print_vp_hex
#define print_vp_hex(p) printf("%"PRIXPTR, (uintptr_t)p)
#endif

#ifndef print_err_vp_hex
#define print_err_vp_hex(p) fprintf(eembed_err, "%"PRIXPTR, (uintptr_t)p)
#endif

#ifndef print_eol
#define print_eol(void) printf("\n")
#endif

#ifndef print_err_eol
#define print_err_eol(void) fprintf(eembed_err, "\n")
#endif

#ifndef eembed_long_to_str
#define eembed_long_to_str(buf, size, l) \
	eembed_sprintf_long_to_str(buf, size, l)
#endif

#ifndef eembed_ulong_to_str
#define eembed_ulong_to_str(buf, size, ul) \
	eembed_sprintf_ulong_to_str(buf, size, ul)
#endif

#ifndef eembed_float_to_str
#define eembed_float_to_str(buf, size, f) \
	eembed_sprintf_float_to_str(buf, size, f)
#endif

#ifndef eembed_ulong_to_hex
#define eembed_ulong_to_hex(buf, size, z) \
	eembed_sprintf_ulong_to_hex(buf, size, z)
#endif

char *eembed_sprintf_long_to_str(char *buf, size_t size, int64_t l);
char *eembed_sprintf_ulong_to_str(char *buf, size_t size, uint64_t ul);
char *eembed_sprintf_float_to_str(char *buf, size_t size, long double f);
char *eembed_sprintf_ulong_to_hex(char *buf, size_t size, uint64_t z);

#endif /* #if (EEMBED_HOSTED && (!(FAUX_FREESTANDING))) */

char *eembed_diy_long_to_str(char *buf, size_t size, int64_t l);
char *eembed_diy_ulong_to_str(char *buf, size_t size, uint64_t ul);
char *eembed_diy_float_to_str(char *buf, size_t size, long double f);
char *eembed_diy_ulong_to_hex(char *buf, size_t size, uint64_t z);

/* ((two hex digits per byte) + NULL terminator) */
#define eembed_bytes_to_hex_min_buf(num_bytes) ((2 * num_bytes) + 1)

char *eembed_bytes_to_hex(char *buf, size_t buf_size, unsigned char *bytes,
			  size_t bytes_len);

#ifndef eembed_long_to_str
#define eembed_long_to_str(buf, size, l) \
	eembed_diy_long_to_str(buf, size, l)
#endif

#ifndef eembed_ulong_to_str
#define eembed_ulong_to_str(buf, size, ul) \
	eembed_diy_ulong_to_str(buf, size, ul)
#endif

#ifndef eembed_float_to_str
#define eembed_float_to_str(buf, size, f) \
	eembed_diy_float_to_str(buf, size, f)
#endif

#ifndef eembed_ulong_to_hex
#define eembed_ulong_to_hex(buf, size, z) \
	eembed_diy_ulong_to_hex(buf, size, z)
#endif

#ifndef Eembed_use_diy_str_to_64
#if (LONG_MAX <= INT64_MAX)
#if (!(_ISOC99_SOURCE))
#define Eembed_use_diy_str_to_64 1
#endif
#endif
#endif

#ifndef Eembed_use_diy_str_to_64
#define Eembed_use_diy_str_to_64 ((!EEMBED_HOSTED) || FAUX_FREESTANDING)
#endif

#ifndef eembed_str_to_i64
#if (Eembed_use_diy_str_to_64)
#define eembed_str_to_i64(str, endptr, base) \
	eembed_diy_str_to_i64(str, endptr, base)
#elif (ULONG_MAX >= UINT64_MAX)
#define eembed_str_to_i64(str, endptr, base) \
	strtol(str, endptr, base)
#else
#define eembed_str_to_i64(str, endptr, base) \
	strtoll(str, endptr, base)
#endif
#endif

#ifndef eembed_str_to_u64
#if (Eembed_use_diy_str_to_64)
#define eembed_str_to_u64(str, endptr, base) \
	eembed_diy_str_to_u64(str, endptr, base)
#elif (ULONG_MAX >= UINT64_MAX)
#define eembed_str_to_u64(str, endptr, base) \
	strtoul(str, endptr, base)
#else
#define eembed_str_to_u64(str, endptr, base) \
	strtoull(str, endptr, base)
#endif
#endif

#define eembed_base10 10
#define eembed_base16 16

#ifndef eembed_str_to_long
#define eembed_str_to_long(str, endptr) \
	eembed_str_to_i64(str, endptr, eembed_base10)
#endif

#ifndef eembed_str_to_ulong
#define eembed_str_to_ulong(str, endptr)\
	eembed_str_to_u64(str, endptr, eembed_base10);
#endif

#ifndef eembed_hex_to_ulong
#define eembed_hex_to_ulong(str, endptr) \
	eembed_str_to_u64(str, endptr, eembed_base16)
#endif

/***************************************************************************\
 * A source variation between __STDC_HOSTED__ and Freestanding platforms
 * is the possible lack of string.h functions like strcmp and memcpy.
 *
 * These #defines provide a way for calling code to use the standard library
 * and provide "eembed_diy_" versions for freestanding environments.
\***************************************************************************/
#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
#include <string.h>

#ifndef eembed_memcmp
#define eembed_memcmp memcmp
#endif

#ifndef eembed_memcpy
#define eembed_memcpy memcpy
#endif

#ifndef eembed_memmove
#define eembed_memmove memmove
#endif

#ifndef eembed_memset
#define eembed_memset memset
#endif

#ifndef eembed_strcat
#define eembed_strcat strcat
#endif

#ifndef eembed_strncat
#define eembed_strncat strncat
#endif

#ifndef eembed_strcmp
#define eembed_strcmp strcmp
#endif

#ifndef eembed_strncmp
#define eembed_strncmp strncmp
#endif

#ifndef eembed_strcpy
#define eembed_strcpy strcpy
#endif

#ifndef eembed_strncpy
#define eembed_strncpy strncpy
#endif

#ifndef eembed_strlen
#define eembed_strlen strlen
#endif

#ifndef eembed_strnlen
#if (_POSIX_C_SOURCE >= 200809L)
#define eembed_strnlen strnlen
#endif
#endif

#ifndef eembed_strstr
#define eembed_strstr strstr
#endif

#endif /* #if (EEMBED_HOSTED && (!(FAUX_FREESTANDING))) */

/* a strncpy()-like function which always NULL-terminates.
 * returns 0 on success or non-zero if the value was truncated. */
int eembed_strcpy_safe(char *buf, size_t size, const char *str);

int eembed_diy_memcmp(const void *s1, const void *s2, size_t n);

void *eembed_diy_memcpy(void *dest, const void *src, size_t n);
void *eembed_diy_memmove(void *dest, const void *src, size_t n);

void *eembed_diy_memset(void *dest, int val, size_t n);

char *eembed_diy_strcat(char *dest, const char *src);
char *eembed_diy_strncat(char *dest, const char *src, size_t n);

int eembed_diy_strcmp(const char *s1, const char *s2);
int eembed_diy_strncmp(const char *s1, const char *s2, size_t n);

char *eembed_diy_strcpy(char *dest, const char *src);
char *eembed_diy_strncpy(char *dest, const char *src, size_t n);

size_t eembed_diy_strlen(const char *s);
size_t eembed_diy_strnlen(const char *s, size_t maxlen);

char *eembed_diy_strstr(const char *haystack, const char *needle);

int64_t eembed_diy_str_to_i64(const char *str, char **endptr, int base);
uint64_t eembed_diy_str_to_u64(const char *str, char **endptr, int pbase);

#ifndef eembed_memcmp
#define eembed_memcmp eembed_diy_memcmp
#endif

#ifndef eembed_memcpy
#define eembed_memcpy eembed_diy_memcpy
#endif

#ifndef eembed_memmove
#define eembed_memmove eembed_diy_memmove
#endif

#ifndef eembed_memset
#define eembed_memset eembed_diy_memset
#endif

#ifndef eembed_strcat
#define eembed_strcat eembed_diy_strcat
#endif

#ifndef eembed_strncat
#define eembed_strncat eembed_diy_strncat
#endif

#ifndef eembed_strcmp
#define eembed_strcmp eembed_diy_strcmp
#endif

#ifndef eembed_strncmp
#define eembed_strncmp eembed_diy_strncmp
#endif

#ifndef eembed_strcpy
#define eembed_strcpy eembed_diy_strcpy
#endif

#ifndef eembed_strncpy
#define eembed_strncpy eembed_diy_strncpy
#endif

#ifndef eembed_strlen
#define eembed_strlen eembed_diy_strlen
#endif

#ifndef eembed_strnlen
#define eembed_strnlen eembed_diy_strnlen
#endif

#ifndef eembed_strstr
#define eembed_strstr eembed_diy_strstr
#endif

/***************************************************************************\
 * Even on __STDC_HOSTED__ systems, there is not a uniform way to get
 * randomness. Linux has getrandom(2), POSIX has /dev/random, etc.
 *
 * The eembed_random_bytes define provides a uniform way for calling code to
 * get random bytes, and provides an implementation, if needed.
\***************************************************************************/
int eembed_lcg_pseudo_random_bytes(unsigned char *buf, size_t size);
#if (EEMBED_HOSTED && (!(FAUX_FREESTANDING)))
int eembed_system_getrandom_bytes(unsigned char *buf, size_t buf_size);
#ifndef eembed_random_bytes
#define eembed_random_bytes eembed_system_getrandom_bytes
#endif
#endif /* #if (EEMBED_HOSTED && (!(FAUX_FREESTANDING))) */

#ifndef eembed_random_bytes
#define eembed_random_bytes eembed_lcg_pseudo_random_bytes
#endif

/***************************************************************************\
 * Often timing is important for embedded firmware, and a desire for a
 * simple pause() function is desired.
 *
 * Each platform may have its own. Arduino has delay(uint16_t milliseconds),
 * POSIX systems have nanosleep(timespec). Others may be added.
 *
 * Here, we #define a delay_ms_u16(millis) macro to provide a common interface.
 * It should be noted that the uint16_t which is provided by Arduino has
 * a max sleep time of about 65 seconds since 65535/1000 is 65.535, thus
 * it might be wise to use multiple calls if hoping to sleep for more than
 * a minute.
\***************************************************************************/

#if (EEMBED_HOSTED || FAUX_FREESTANDING)
#ifndef delay_ms_u16
void eembed_hosted_delay_ms_u16(uint16_t milliseconds);
#define delay_ms_u16(milliseconds) eembed_hosted_delay_ms_u16(milliseconds)
#endif

#ifndef uptime_ms
uint64_t eembed_hosted_uptime_ms(void);
#define uptime_ms() eembed_hosted_uptime_ms()
#endif
#endif /* (EEMBED_HOSTED || FAUX_FREESTANDING) */

/***************************************************************************\
 * On __STDC_HOSTED__ systems, memory allocation is taken for granted.
 * On Freestanding systems, there may be no malloc() function.
 *
 * The eembed_bytes_allocator has proven useful on hosted systems.
\***************************************************************************/

/* default to "no op" versions if eembed_global_allocator is NULL */
void *eembed_malloc(size_t size);
void *eembed_calloc(size_t nmemb, size_t size);
void *eembed_realloc(void *ptr, size_t size);
void *eembed_reallocarray(void *ptr, size_t nmemb, size_t size);
void eembed_free(void *ptr);

struct eembed_allocator;

/* eembed_global_allocator may be the null_allocator, if not EEMBED_HOSTED */
extern struct eembed_allocator *eembed_global_allocator;
extern struct eembed_allocator *eembed_null_allocator;

struct eembed_allocator {
	void *context;
	void *(*malloc)(struct eembed_allocator *ea, size_t size);
	void *(*calloc)(struct eembed_allocator *ea, size_t nmemb, size_t size);
	void *(*realloc)(struct eembed_allocator *ea, void *ptr, size_t size);
	void *(*reallocarray)(struct eembed_allocator *ea, void *ptr,
			      size_t nmemb, size_t size);
	void (*free)(struct eembed_allocator *ea, void *ptr);
};

extern const size_t eembed_bytes_allocator_min_buf_size;
struct eembed_allocator *eembed_bytes_allocator(unsigned char *bytes,
						size_t len);
/* forward declaring */
struct eembed_log;

void eembed_bytes_allocator_dump(struct eembed_log *log,
				 struct eembed_allocator *bytes_allocator);
void eembed_bytes_allocator_visual(struct eembed_log *log,
				   struct eembed_allocator *bytes_allocator,
				   int strinify_contents, size_t width);

/***************************************************************************\
 * Verifying that correct information is logged in crash situations is often
 * tedious and challenging.
 *
 * The eembed_assert and eembed_log provide a mechanism by which code can
 * be written such that it will assertion failures can be more easily tested
\***************************************************************************/
/* On a hosted system this is exit(EXIT_FAILURE),
   on a freestanding system, this is something that will cause the system to
   halt or restart, e.g.: writing to a NULL pointer. */
extern void (*eembed_assert_crash)(void);

/* eembed_out_log and eembed_err_log may point to the same struct */
extern struct eembed_log *eembed_out_log;
extern struct eembed_log *eembed_err_log;
extern struct eembed_log *eembed_null_log;

struct eembed_log {
	void *context;
	void (*append_c)(struct eembed_log *log, char c);
	void (*append_s)(struct eembed_log *log, const char *str);
	void (*append_ul)(struct eembed_log *log, uint64_t ul);
	void (*append_l)(struct eembed_log *log, int64_t l);
	void (*append_f)(struct eembed_log *log, long double f);
	void (*append_vp)(struct eembed_log *log, const void *ptr);
	void (*append_eol)(struct eembed_log *log);
};

struct eembed_str_buf {
	char *buf;
	size_t size;
};

struct eembed_log *eembed_char_buf_log_init(struct eembed_log *log,
					    struct eembed_str_buf *ctx,
					    char *buf, size_t size);

#ifndef print_s
#define print_s(s) eembed_out_log->append_s(eembed_out_log, s)
#endif

#ifndef print_err_s
#define print_err_s(s) eembed_err_log->append_s(eembed_err_log, s)
#endif

#ifndef print_i
#define print_i(i) eembed_out_log->append_l(eembed_out_log, i)
#endif

#ifndef print_err_i
#define print_err_i(i) eembed_err_log->append_l(eembed_err_log, i)
#endif

#ifndef print_l
#define print_l(l) eembed_out_log->append_l(eembed_out_log, l)
#endif

#ifndef print_err_l
#define print_err_l(l) eembed_err_log->append_l(eembed_err_log, l)
#endif

#ifndef print_ll
#define print_ll(ll) eembed_out_log->append_l(eembed_out_log, ll)
#endif

#ifndef print_err_ll
#define print_err_ll(ll) eembed_err_log->append_l(eembed_err_log, ll)
#endif

#ifndef print_u
#define print_u(u) eembed_out_log->append_ul(eembed_out_log, u)
#endif

#ifndef print_err_u
#define print_err_u(u) eembed_err_log->append_ul(eembed_err_log, u)
#endif

#ifndef print_ul
#define print_ul(ul) eembed_out_log->append_ul(eembed_out_log, ul)
#endif

#ifndef print_err_ul
#define print_err_ul(ul) eembed_err_log->append_ul(eembed_err_log, ul)
#endif

#ifndef print_ull
#define print_ull(ull) eembed_out_log->append_ul(eembed_out_log, ull)
#endif

#ifndef print_err_ull
#define print_err_ull(ull) eembed_err_log->append_ul(eembed_err_log, ull)
#endif

#ifndef print_z
#define print_z(z) eembed_out_log->append_ul(eembed_out_log, z)
#endif

#ifndef print_err_z
#define print_err_z(z) eembed_err_log->append_ul(eembed_err_log, z)
#endif

#ifndef print_c
#define print_c(c) eembed_out_log->append_c(eembed_out_log, c)
#endif

#ifndef print_err_c
#define print_err_c(c) eembed_err_log->append_c(eembed_err_log, c)
#endif

#ifndef print_f
#define print_f(f) eembed_out_log->append_f(eembed_out_log, f)
#endif

#ifndef print_err_f
#define print_err_f(f) eembed_err_log->append_f(eembed_err_log, f)
#endif

#ifndef print_lf
#define print_lf(lf) eembed_out_log->append_f(eembed_out_log, lf)
#endif

#ifndef print_err_lf
#define print_err_lf(lf) eembed_err_log->append_f(eembed_err_log, lf)
#endif

#ifndef print_fd
/* TODO: this should be easy to add to eembed_log, but has not been done */
#define print_fd(f,d) eembed_out_log->append_f(eembed_out_log, f)
#endif

#ifndef print_err_fd
/* TODO: this should be easy to add to eembed_log, but has not been done */
#define print_err_fd(f,d) eembed_err_log->append_f(eembed_err_log, f)
#endif

#ifndef print_lfd
/* TODO: this should be easy to add to eembed_log, but has not been done */
#define print_lfd(lf,d) eembed_out_log->append_f(eembed_out_log, lf)
#endif

#ifndef print_err_lfd
/* TODO: this should be easy to add to eembed_log, but has not been done */
#define print_err_lfd(lf,d) eembed_out_log->append_f(eembed_out_log, lf)
#endif

#ifndef print_log_x
#define print_log_x(log, x) \
	do { \
		char eembed_x_str[25] = { '\0' }; \
		eembed_ulong_to_hex(eembed_x_str, sizeof(eembed_x_str), x); \
		log->append_s(log, str); \
	} while (0)
#endif

#ifndef print_x
#define print_x(x) print_log_x(eembed_out_log, (uint64_t)x)
#endif

#ifndef print_err_x
#define print_err_x(x) print_log_x(eembed_err_log, (uint64_t)x)
#endif

#ifndef print_lx
#define print_lx(lx) print_log_x(eembed_out_log, (uint64_t)lx)
#endif

#ifndef print_err_lx
#define print_err_lx(lx) print_log_x(eembed_err_log, (uint64_t)lx)
#endif

#ifndef print_log_u8_bin
#define print_log_u8_bin(log, u8) \
	do { \
		uint8_t eembed_tmp_u8 = (u8); \
		log->append_c(log, (eembed_tmp_u8 & (1 << 7)) ? '1' : '0'); \
		log->append_c(log, (eembed_tmp_u8 & (1 << 6)) ? '1' : '0'); \
		log->append_c(log, (eembed_tmp_u8 & (1 << 5)) ? '1' : '0'); \
		log->append_c(log, (eembed_tmp_u8 & (1 << 4)) ? '1' : '0'); \
		log->append_c(log, (eembed_tmp_u8 & (1 << 3)) ? '1' : '0'); \
		log->append_c(log, (eembed_tmp_u8 & (1 << 2)) ? '1' : '0'); \
		log->append_c(log, (eembed_tmp_u8 & (1 << 1)) ? '1' : '0'); \
		log->append_c(log, (eembed_tmp_u8 & (1 << 0)) ? '1' : '0'); \
	} while (0)
#endif

#ifndef print_u8_bin
#define print_u8_bin(u8) print_log_u8_bin(eembed_out_log, u8)
#endif

#ifndef print_err_u8_bin
#define print_err_u8_bin(u8) print_log_u8_bin(eembed_err_log, u8)
#endif

#ifndef print_log_u16_bin
#define print_log_u16_bin(log, u16) \
	do { \
		uint16_t eembed_tmp_u16 = (u16); \
		print_log_u8_bin(log, 0xFF & (eembed_tmp_u16 >> 8)); \
		print_log_u8_bin(log, 0xFF & eembed_tmp_u16); \
	} while (0)
#endif

#ifndef print_u16_bin
#define print_u16_bin(u16) print_log_u16_bin(eembed_out_log, u16)
#endif

#ifndef print_err_u16_bin
#define print_err_u16_bin(u16) print_log_u16_bin(eembed_err_log, u16)
#endif

#ifndef print_log_u32_bin
#define print_log_u32_bin(log, u32) \
	do { \
		uint32_t eembed_tmp_u32 = (u32); \
		print_log_u16_bin(log, 0xFFFF & (eembed_tmp_u32 >> 16)); \
		print_log_u16_bin(log, 0xFFFF & eembed_tmp_u32); \
	} while (0)
#endif

#ifndef print_u32_bin
#define print_u32_bin(u32) print_log_u32_bin(eembed_out_log, u32)
#endif

#ifndef print_err_u32_bin
#define print_err_u32_bin(u32) print_log_u32_bin(eembed_err_log, u32)
#endif

#ifndef print_log_u64_bin
#define print_log_u64_bin(log, u64) \
	do { \
		uint64_t eembed_tmp_u64 = (u64); \
		print_log_u32_bin(log, 0xFFFFFFFF & (eembed_tmp_u64 >> 32)); \
		print_log_u32_bin(log, 0xFFFFFFFF & eembed_tmp_u64); \
	} while (0)
#endif

#ifndef print_u64_bin
#define print_u64_bin(u64) print_log_u64_bin(eembed_out_log, u64)
#endif

#ifndef print_err_u64_bin
#define print_err_u64_bin(u64) print_log_u64_bin(eembed_err_log, u64)
#endif

#ifndef print_u8_hex
#define print_u8_hex(u8) print_log_x(eembed_out_log, 0xFF & (u8))
#endif

#ifndef print_err_u8_hex
#define print_err_u8_hex(u8) print_log_x(eembed_err_log, 0xFF & (u8))
#endif

#ifndef print_vp_hex
#define print_vp_hex(p) eembed_out_log->append_vp(eembed_out_log, p)
#endif

#ifndef print_err_vp_hex
#define print_err_vp_hex(p) eembed_err_log->append_vp(eembed_err_log, p)
#endif

#ifndef print_eol
#define print_eol(void) eembed_out_log->append_eol(eembed_out_log)
#endif

#ifndef print_err_eol
#define print_err_eol(void) eembed_err_log->append_eol(eembed_err_log)
#endif

#ifndef EEMBED_NOP
#define EEMBED_NOP() do { ((void)0); } while (0)
#endif

#define eembed_crash_if_false(expression) \
	do { \
		if (!(expression)) { \
			struct eembed_log *el = eembed_err_log; \
			if (el) { \
				el->append_s(el, __FILE__); \
				el->append_s(el, ":"); \
				el->append_ul(el, __LINE__); \
				el->append_s(el, ": ASSERTION assert("); \
				el->append_s(el, #expression); \
				el->append_s(el, ") FAILED"); \
				el->append_eol(el); \
			} \
			eembed_assert_crash(); \
		} \
	} while (0)

#ifndef eembed_assert
#ifdef NDEBUG
#define eembed_assert(expression) EEMBED_NOP()
#else
#define eembed_assert(expression) eembed_crash_if_false(expression)
#endif
#endif

#define eembed_concat(a,b) a##b

/* without the two layers here, eembed_concat(foo, __LINE__) would result in
 * `foo__LINE__`, thus we do an expansion first to get `foo123`*/
#define eembed_concat_expanded(a,b) eembed_concat(a,b)

/* if the expression is false, the compiler errors on array of negative size */
/* e.g.: error: size of array ‘eembed_static_assert_97_a’ is negative */
#define eembed_static_assert_struct(expression, name) \
	typedef struct name { \
		char eembed_concat(name, _a)[(expression) ? 1 : -1]; \
	} eembed_concat(name,_s)

#if __STDC_VERSION__ >= 201112L
#define eembed_static_assert(expression, msg) _Static_assert(expression, msg)
#endif

#ifndef eembed_static_assert
#define eembed_static_assert(expression, msg) \
	eembed_static_assert_struct(expression, \
		eembed_concat_expanded(eembed_static_assert_, __LINE__))
#endif

#define eembed_static_assert_equals(a,b) \
	eembed_static_assert(a == b, "assertion ( " #a " == " #b " ) failed")

#define eembed_static_assert_sizes_match(a,b) \
	eembed_static_assert_equals(sizeof(a), sizeof(b))

/***************************************************************************\
 * POSIX, Windows, and almost all currently supported platforms define
 * CHAR_BIT to be 8. Odds are very good that your code can safely assume
 * that bytes are 8 bits.
 *
 * It seems that the Texas Instruments C54x series DSPs have CHAR_BIT 16
 * in some C compilers. It might be interesting to look at these.
\***************************************************************************/
#ifndef EEMBED_CHAR_BIT
#ifdef CHAR_BIT
#define EEMBED_CHAR_BIT CHAR_BIT
#else
#define EEMBED_CHAR_BIT 8
#endif
#endif

/***************************************************************************\
 * Alignment rarely comes up, but sometimes "alignas" and "alignof" are
 * needed for doing things like aligning a data structures for SIMD.
 * More generally, arrays of structs which are poorly aligned can impact
 * performance. As always, measuring is better than guessing.
\***************************************************************************/
#ifndef EEMBED_WORD_LEN
#ifdef __WORDSIZE
#if (__WORDSIZE >= EEMBED_CHAR_BIT)
#define EEMBED_WORD_LEN (__WORDSIZE / EEMBED_CHAR_BIT)
#endif
#endif
#endif

#ifndef EEMBED_WORD_LEN
#define EEMBED_WORD_LEN (sizeof(void *))
#endif

#define eembed_align_to(x, y) \
		(((x) + ((y) - 1)) \
		     & ~((y) - 1))

#define eembed_align(x) eembed_align_to(x, EEMBED_WORD_LEN)

/***************************************************************************\
 * A generic structure for easily passing a function and data together to
 * functions which accept a single void* for context.
 * This is used internally, and exposed as it is reused elsewhere.
\***************************************************************************/
struct eembed_function_context {
	void *data;
	void *(*func)(void *data);
};

/***************************************************************************\
 * Hosted programs typically have a main() function which exits.
 * Firmware in freestanding environments typically is an infinite loop.
 * Further, the "main" function may be hidden from the developer, as is the
 * case with the default Arduino build system.
 *
 * EEMBED_FUNC_MAIN(name_of_main_inner) provides a way to handle both cases.
\***************************************************************************/
/* This function exists to allow freestanding systems to initialize
 * printing, this can be useful in FAUX_FREESTANDING test builds. */
void eembed_system_print_init(void);

#if (EEMBED_HOSTED || FAUX_FREESTANDING)

#ifndef EEMBED_FUNC_MAIN
#define EEMBED_FUNC_MAIN(pfunc) \
int main(void) \
{ \
	eembed_system_print_init(); \
	return pfunc() ? 1 : 0; \
}
#endif

#endif /* (EEMBED_HOSTED || FAUX_FREESTANDING) */

#ifndef EEMBED_FUNC_MAIN
#define EEMBED_FUNC_MAIN(pfunc)	/* skip */
#endif

Eembed_end_C_functions
#undef Eembed_end_C_functions
#endif /* EEMBED_H */
