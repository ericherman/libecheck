/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed.h */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020-2024 Eric Herman <eric@freesa.org> */

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
#ifndef FAUX_FREESTANDING
#define FAUX_FREESTANDING 0
#endif
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
#ifdef __SSIZE_T_TYPE
    typedef __SSIZE_T_TYPE ssize_t;
#else
/* Define ssize_t as intptr_t if it is not defined */
    typedef intptr_t ssize_t;
#define __SSIZE_T_TYPE intptr_t
#endif

/* Define the macros to indicate ssize_t is defined */
#define _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED_
#define __ssize_t_defined

#endif /* #ifndef __ssize_t_defined */
#endif /* #ifndef _SSIZE_T_DEFINED_ */
#endif /* #ifndef _SSIZE_T_DEFINED */
/* End of ssize_t defined */

#ifdef ARDUINO

#ifndef eembed_out
#define eembed_out Serial
#endif

#ifndef eembed_err
#define eembed_err eembed_out
#endif

#ifndef print_s
#define print_s(s) eembed_out.print(s)
#endif

#ifndef print_err_s
#define print_err_s(s) eembed_err.print(s)
#endif

#ifndef print_i
#define print_i(i) eembed_out.print(i)
#endif

#ifndef print_err_i
#define print_err_i(i) eembed_err.print(i)
#endif

#ifndef print_l
#define print_l(l) eembed_out.print(l)
#endif

#ifndef print_err_l
#define print_err_l(l) eembed_err.print(l)
#endif

#ifndef print_ll
#define print_ll(ll) eembed_out.print(ll)
#endif

#ifndef print_err_ll
#define print_err_ll(ll) eembed_err.print(ll)
#endif

#ifndef print_u
#define print_u(u) eembed_out.print(u)
#endif

#ifndef print_err_u
#define print_err_u(u) eembed_err.print(u)
#endif

#ifndef print_ul
#define print_ul(ul) eembed_out.print(ul)
#endif

#ifndef print_err_ul
#define print_err_ul(ul) eembed_err.print(ul)
#endif

#ifndef print_ull
#define print_ull(ull) eembed_out.print(ull)
#endif

#ifndef print_err_ull
#define print_err_ull(ull) eembed_err.print(ull)
#endif

#ifndef print_z
#define print_z(z) eembed_out.print(z)
#endif

#ifndef print_err_z
#define print_err_z(z) eembed_err.print(z)
#endif

#ifndef print_c
#define print_c(c) eembed_out.print(c)
#endif

#ifndef print_err_c
#define print_err_c(c) eembed_err.print(c)
#endif

#ifndef print_f
#define print_f(f) eembed_out.print(f)
#endif

#ifndef print_err_f
#define print_err_f(f) eembed_err.print(f)
#endif

#ifndef print_lf
#define print_lf(lf) eembed_out.print(lf)
#endif

#ifndef print_err_lf
#define print_err_lf(lf) eembed_err.print(lf)
#endif

#ifndef print_fd
#define print_fd(f,d) eembed_out.print(f, d)
#endif

#ifndef print_err_fd
#define print_err_fd(f,d) eembed_err.print(f, d)
#endif

#ifndef print_lfd
#define print_lfd(lf,d) eembed_out.print(lf, d)
#endif

#ifndef print_err_lfd
#define print_err_lfd(lf,d) eembed_err.print(lf, d)
#endif

#ifndef print_x
#define print_x(x) eembed_out.print(x, HEX)
#endif

#ifndef print_err_x
#define print_err_x(x) eembed_err.print(x, HEX)
#endif

#ifndef print_lx
#define print_lx(lx) eembed_out.print(lx, HEX)
#endif

#ifndef print_err_lx
#define print_err_lx(lx) eembed_err.print(lx, HEX)
#endif

#ifndef print_u8_bin
#define print_u8_bin(u8) eembed_out.print(u8, BIN)
#endif

#ifndef print_err_u8_bin
#define print_err_u8_bin(u8) eembed_err.print(u8, BIN)
#endif

#ifndef print_u16_bin
#define print_u16_bin(u16) eembed_out.print(u16, BIN)
#endif

#ifndef print_err_u16_bin
#define print_err_u16_bin(u16) eembed_err.print(u16, BIN)
#endif

#ifndef print_u32_bin
#define print_u32_bin(u32) eembed_out.print(u32, BIN)
#endif

#ifndef print_err_u32_bin
#define print_err_u32_bin(u32) eembed_err.print(u32, BIN)
#endif

#ifndef print_u64_bin
#define print_u64_bin(u64) eembed_out.print(u64, BIN)
#endif

#ifndef print_err_u64_bin
#define print_err_u64_bin(u64) eembed_err.print(u64, BIN)
#endif

#ifndef print_u8_hex
#define print_u8_hex(u8) eembed_out.print(u8, HEX)
#endif

#ifndef print_err_u8_hex
#define print_err_u8_hex(u8) eembed_err.print(u8, HEX)
#endif

#ifndef print_vp_hex
#define print_vp_hex(p) eembed_out.print((uintptr_t)p, HEX)
#endif

#ifndef print_err_vp_hex
#define print_err_vp_hex(p) eembed_err.print((uintptr_t)p, HEX)
#endif

#ifndef print_eol
#define print_eol(void) eembed_out.println()
#endif

#ifndef print_err_eol
#define print_err_eol(void) eembed_err.println()
#endif

#endif /* #ifdef ARDUINO */

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

#endif /* #if (EEMBED_HOSTED && (!(FAUX_FREESTANDING))) */

/* forward declaring */
struct eembed_log;
extern struct eembed_log *eembed_null_log;

/* eembed_out_log and eembed_err_log may point to the same struct */
extern struct eembed_log *eembed_out_log;
extern struct eembed_log *eembed_err_log;

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

struct eembed_function_context {
	void *data;
	void *(*func)(void *data);
};

struct eembed_str_buf {
	char *buf;
	size_t size;
};

struct eembed_log *eembed_char_buf_log_init(struct eembed_log *log,
					    struct eembed_str_buf *ctx,
					    char *buf, size_t size);

char *eembed_long_to_str(char *buf, size_t size, int64_t l);
char *eembed_ulong_to_str(char *buf, size_t size, uint64_t ul);
char *eembed_float_to_str(char *buf, size_t size, long double f);

/* ((two hex digits per byte) + NULL terminator) */
#define eembed_bytes_to_hex_min_buf(num_bytes) ((2 * num_bytes) + 1)
char *eembed_ulong_to_hex(char *buf, size_t size, uint64_t z);
char *eembed_bytes_to_hex(char *buf, size_t buf_size, unsigned char *bytes,
			  size_t bytes_len);

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

extern void (*eembed_assert_crash)(void);

/* a strncpy()-like function which always NULL-terminates.
 * returns 0 on success or non-zero if the value was truncated. */
int eembed_strcpy_safe(char *buf, size_t size, const char *str);

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

int eembed_system_getrandom_bytes(unsigned char *buf, size_t buf_size);
#ifndef eembed_random_bytes
#define eembed_random_bytes eembed_system_getrandom_bytes
#endif

#endif /* #if (EEMBED_HOSTED && (!(FAUX_FREESTANDING))) */

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

int eembed_lcg_pseudo_random_bytes(unsigned char *buf, size_t size);

#ifndef eembed_random_bytes
#define eembed_random_bytes eembed_lcg_pseudo_random_bytes
#endif

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

struct eembed_allocator;
/* eembed_global_allocator may be the null_allocator, if not EEMBED_HOSTED */
extern struct eembed_allocator *eembed_global_allocator;
extern struct eembed_allocator *eembed_null_allocator;

/* default to "no op" versions if eembed_global_allocator is NULL */
void *eembed_malloc(size_t size);
void *eembed_calloc(size_t nmemb, size_t size);
void *eembed_realloc(void *ptr, size_t size);
void *eembed_reallocarray(void *ptr, size_t nmemb, size_t size);
void eembed_free(void *ptr);

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
void eembed_bytes_allocator_dump(struct eembed_log *log,
				 struct eembed_allocator *bytes_allocator);
void eembed_bytes_allocator_visual(struct eembed_log *log,
				   struct eembed_allocator *bytes_allocator,
				   int strinify_contents, size_t width);

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

#ifndef EEMBED_CHAR_BIT
#ifdef CHAR_BIT
#define EEMBED_CHAR_BIT CHAR_BIT
#else
#define EEMBED_CHAR_BIT 8
#endif
#endif

#ifndef EEMBED_WORD_LEN
#ifdef __WORDSIZE
#if (__WORDSIZE >= EEMBED_CHAR_BIT)
#define EEMBED_WORD_LEN (__WORDSIZE / EEMBED_CHAR_BIT)
#endif
#endif
#endif

#ifndef EEMBED_WORD_LEN
#define EEMBED_WORD_LEN (sizeof(size_t))
#endif

#define eembed_align_to(x, y) \
		(((x) + ((y) - 1)) \
		     & ~((y) - 1))

#define eembed_align(x) eembed_align_to(x, EEMBED_WORD_LEN)

#ifndef FAUX_FREESTANDING
#define FAUX_FREESTANDING 0
#endif

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
