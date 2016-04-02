/* echeck.h */
#ifndef ECHECK_H
#define ECHECK_H

#include <stdlib.h>		/* for size_t */
#include <stdio.h>		/* for FILE */

#ifdef __GNUC__
#define ECHECK_FUNC __PRETTY_FUNCTION__
#else
#if (__STDC_VERSION__ >= 199901L)
#define ECHECK_FUNC __func__
#else
#define ECHECK_FUNC NULL
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ */

/*check char*/
int echeck_char_m(FILE *err, const char *func, const char *file, int line,
		  char actual, char expected, const char *msg);

#define fcheck_char_m(log, actual, expected, msg)\
	echeck_char_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_char_m(actual, expected, msg)\
	echeck_char_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define fcheck_char(log, actual, expected)\
	echeck_char_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

#define check_char(actual, expected)\
	echeck_char_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

/* check int */
int echeck_int_m(FILE *err, const char *func, const char *file, int line,
		 int actual, int expected, const char *msg);

#define fcheck_int_m(log, actual, expected, msg)\
	echeck_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_int_m(actual, expected, msg)\
	echeck_int_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define fcheck_int(log, actual, expected)\
	echeck_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

#define check_int(actual, expected)\
	echeck_int_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

/* check long */
int echeck_long_m(FILE *err, const char *func, const char *file, int line,
		  long actual, long expected, const char *msg);

#define fcheck_long_m(log, actual, expected, msg)\
	echeck_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_long_m(actual, expected, msg)\
	echeck_long_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define fcheck_long(log, actual, expected)\
	echeck_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

#define check_long(actual, expected)\
	echeck_long_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

/* check str */
int echeck_str_m(FILE *err, const char *func, const char *file, int line,
		 const char *actual, const char *expected, const char *msg);

#define fcheck_str_m(log, actual, expected, msg)\
	echeck_str_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_str_m(actual, expected, msg)\
	echeck_str_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define fcheck_str(log, actual, expected)\
	echeck_str_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

#define check_str(actual, expected)\
	echeck_str_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

/* check ptr */
int echeck_ptr_m(FILE *err, const char *func, const char *file, int line,
		 const void *actual, const void *expected, const char *msg);

#define fcheck_ptr_m(log, actual, expected, msg)\
	echeck_ptr_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_ptr_m(actual, expected, msg)\
	echeck_ptr_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define fcheck_ptr(log, actual, expected)\
	echeck_ptr_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

#define check_ptr(actual, expected)\
	echeck_ptr_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

/* check unsigned int */
int echeck_unsigned_int_m(FILE *err, const char *func, const char *file,
			  int line, unsigned int actual, unsigned int expected,
			  const char *msg);

#define fcheck_unsigned_int_m(log, actual, expected, msg)\
	echeck_unsigned_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_unsigned_int_m(actual, expected, msg)\
	echeck_unsigned_int_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define fcheck_unsigned_int(log, actual, expected)\
	echeck_unsigned_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

#define check_unsigned_int(actual, expected)\
	echeck_unsigned_int_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

/* check unsigned long */
int echeck_unsigned_long_m(FILE *err, const char *func, const char *file,
			   int line, unsigned long actual,
			   unsigned long expected, const char *msg);

#define fcheck_unsigned_long_m(log, actual, expected, msg)\
	echeck_unsigned_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_unsigned_long_m(actual, expected, msg)\
	echeck_unsigned_long_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define fcheck_unsigned_long(log, actual, expected)\
	echeck_unsigned_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

#define check_unsigned_long(actual, expected)\
	echeck_unsigned_long_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

/* check size_t */
int echeck_size_t_m(FILE *err, const char *func, const char *file, int line,
		    size_t actual, size_t expected, const char *msg);

#define fcheck_size_t_m(log, actual, expected, msg)\
	echeck_size_t_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_size_t_m(actual, expected, msg)\
	echeck_size_t_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define fcheck_size_t(log, actual, expected)\
	echeck_size_t_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

#define check_size_t(actual, expected)\
	echeck_size_t_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, NULL)

/* check byte_array */
int echeck_byte_array_m(FILE *err, const char *func, const char *file, int line,
			unsigned char *actual, size_t actual_len,
			unsigned char *expected, size_t expected_len,
			const char *msg);

#define fcheck_byte_array_m(log, actual, actual_len, expected, expected_len,\
	msg)\
	echeck_byte_array_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, msg)

#define check_byte_array_m(actual, actual_len, expected, expected_len, msg)\
	echeck_byte_array_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, msg)

#define fcheck_byte_array(log, actual, actual_len, expected, expected_len)\
	echeck_byte_array_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, NULL)

#define check_byte_array(actual, actual_len, expected, expected_len)\
	echeck_byte_array_m(stderr, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, NULL)

#endif /* ECHECK_H */
