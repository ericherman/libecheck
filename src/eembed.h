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

int64_t eembed_str_to_long(const char *str, char **endptr);
uint64_t eembed_str_to_ulong(const char *str, char **endptr);
uint64_t eembed_hex_to_ulong(const char *str, char **endptr);
extern void (*eembed_assert_crash)(void);

/* a strncpy()-like function which always NULL-terminates.
 * returns 0 on success or non-zero if the value was truncated. */
int eembed_strcpy_safe(char *buf, size_t size, const char *str);

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

extern int (*eembed_random_bytes)(unsigned char *buf, size_t len);

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
