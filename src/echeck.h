/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* echeck.h */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#ifndef ECHECK_H
#define ECHECK_H

#ifdef __cplusplus
#define Echeck_begin_C_functions extern "C" {
#define Echeck_end_C_functions }
#else
#define Echeck_begin_C_functions
#define Echeck_end_C_functions
#endif

#include <stdlib.h>		/* for size_t */
#include <float.h>		/* for DBL_EPSILON */

#ifdef _GNU_SOURCE
#define ECHECK_FUNC __PRETTY_FUNCTION__
#else
#if (__STDC_VERSION__ >= 199901L)
#define ECHECK_FUNC __func__
#else
#define ECHECK_FUNC NULL
#endif /* _GNU_SOURCE */
#endif /* __STDC_VERSION__ */

struct echeck_log {
	void *context;
	void (*append_s)(struct echeck_log *log, const char *str);
	void (*append_ul)(struct echeck_log *log, unsigned long ul);
	void (*append_z)(struct echeck_log *log, size_t z);
	void (*append_l)(struct echeck_log *log, long l);
	void (*append_f)(struct echeck_log *log, double f);
	void (*append_vp)(struct echeck_log *log, const void *ptr);
	void (*append_eol)(struct echeck_log *log);
};

Echeck_begin_C_functions
/* echeck_default_log may be NULL */
extern struct echeck_log *echeck_default_log;

/*check char*/
int echeck_char_m(struct echeck_log *err, const char *func, const char *file,
		  int line, char actual, char expected, const char *msg);

#define lcheck_char_m(log, actual, expected, msg)\
	echeck_char_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_char_m(actual, expected, msg)\
	echeck_char_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_char(log, actual, expected)\
	echeck_char_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_char(actual, expected)\
	echeck_char_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check long */
int echeck_long_m(struct echeck_log *err, const char *func, const char *file,
		  int line, long actual, long expected, const char *msg);

#define lcheck_long_m(log, actual, expected, msg)\
	echeck_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_long_m(actual, expected, msg)\
	echeck_long_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_long(log, actual, expected)\
	echeck_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_long(actual, expected)\
	echeck_long_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check int */
#define echeck_int_m(err, func, file, line, actual, expected, msg)\
	echeck_long_m(err, func, file, line, actual, expected, msg)

#define lcheck_int_m(log, actual, expected, msg)\
	echeck_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_int_m(actual, expected, msg)\
	echeck_int_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_int(log, actual, expected)\
	echeck_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_int(actual, expected)\
	echeck_int_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check str */
int echeck_str_m(struct echeck_log *err, const char *func, const char *file,
		 int line, const char *actual, const char *expected,
		 const char *msg);

#define lcheck_str_m(log, actual, expected, msg)\
	echeck_str_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_str_m(actual, expected, msg)\
	echeck_str_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_str(log, actual, expected)\
	echeck_str_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_str(actual, expected)\
	echeck_str_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check ptr */
int echeck_ptr_m(struct echeck_log *err, const char *func, const char *file,
		 int line, const void *actual, const void *expected,
		 const char *msg);

#define lcheck_ptr_m(log, actual, expected, msg)\
	echeck_ptr_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_ptr_m(actual, expected, msg)\
	echeck_ptr_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_ptr(log, actual, expected)\
	echeck_ptr_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_ptr(actual, expected)\
	echeck_ptr_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check unsigned long */
int echeck_unsigned_long_m(struct echeck_log *err, const char *func,
			   const char *file, int line, unsigned long actual,
			   unsigned long expected, const char *msg);

#define lcheck_unsigned_long_m(log, actual, expected, msg)\
	echeck_unsigned_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_unsigned_long_m(actual, expected, msg)\
	echeck_unsigned_long_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_unsigned_long(log, actual, expected)\
	echeck_unsigned_long_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_unsigned_long(actual, expected)\
	echeck_unsigned_long_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check unsigned int */
#define echeck_unsigned_int_m(err, func, file, line, actual, expected, msg)\
	echeck_unsigned_long_m(err, func, file, line, actual, expected, msg)

#define lcheck_unsigned_int_m(log, actual, expected, msg)\
	echeck_unsigned_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_unsigned_int_m(actual, expected, msg)\
	echeck_unsigned_int_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_unsigned_int(log, actual, expected)\
	echeck_unsigned_int_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_unsigned_int(actual, expected)\
	echeck_unsigned_int_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check size_t */
int echeck_size_t_m(struct echeck_log *err, const char *func, const char *file,
		    int line, size_t actual, size_t expected, const char *msg);

#define lcheck_size_t_m(log, actual, expected, msg)\
	echeck_size_t_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define check_size_t_m(actual, expected, msg)\
	echeck_size_t_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, msg)

#define lcheck_size_t(log, actual, expected)\
	echeck_size_t_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

#define check_size_t(actual, expected)\
	echeck_size_t_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, expected, #actual)

/* check byte_array */
int echeck_byte_array_m(struct echeck_log *err, const char *func,
			const char *file, int line, const unsigned char *actual,
			size_t actual_len, const unsigned char *expected,
			size_t expected_len, const char *msg);

#define lcheck_byte_array_m(log, actual, actual_len, expected, expected_len,\
	msg)\
	echeck_byte_array_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, msg)

#define check_byte_array_m(actual, actual_len, expected, expected_len, msg)\
	echeck_byte_array_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, msg)

#define lcheck_byte_array(log, actual, actual_len, expected, expected_len)\
	echeck_byte_array_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, #actual)

#define check_byte_array(actual, actual_len, expected, expected_len)\
	echeck_byte_array_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
		 actual, actual_len, expected, expected_len, #actual)

/* check double */
int echeck_double_m(struct echeck_log *err, const char *func, const char *file,
		    int line, double actual, double expected, double epsilon,
		    const char *msg);

#define lcheck_double_m(log, actual, expected, epsilon, msg)\
	echeck_double_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
			actual, expected, epsilon, msg)

#define check_double_m(actual, expected, epsilon, msg)\
	echeck_double_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
			actual, expected, epsilon, msg)

#define lcheck_double(log, actual, expected, epsilon)\
	echeck_double_m(log, ECHECK_FUNC, __FILE__, __LINE__,\
			actual, expected, epsilon, #actual)

#define check_double(actual, expected, epsilon)\
	echeck_double_m(NULL, ECHECK_FUNC, __FILE__, __LINE__,\
			actual, expected, epsilon, #actual)

#define echeck_double_scaled_epsilon_m(err, func, file, line, actual,\
				       expected, msg)\
	echeck_double_m(err, func, file, line, actual, expected,\
			((expected) * DBL_EPSILON), msg)

/*
 * WARNING: The "check_double_scaled_epsilon" family of functions
 * uses the "expected" value to scale DBL_EPSILON. This should not
 * be used if "expected" is an expression with side-effects.  */
#define lcheck_double_scaled_epsilon_m(log, actual, expected, msg)\
	lcheck_double_m(log, actual, expected,\
			((expected) * DBL_EPSILON), msg);

#define check_double_scaled_epsilon_m(actual, expected, msg)\
	check_double_m(actual, expected, ((expected) * DBL_EPSILON), msg);

#define lcheck_double_scaled_epsilon(log, actual, expected)\
	lcheck_double(log, actual, expected, ((expected) * DBL_EPSILON));

#define check_double_scaled_epsilon(actual, expected)\
	check_double(actual, expected, ((expected) * DBL_EPSILON));

/*check status*/
/* safe casting of non-zero int to avoid EXIT_SUCCESS */
char echeck_status_m(struct echeck_log *err, const char *func, const char *file,
		     int line, int val, const char *msg);

#define lcheck_status_m(log, val, msg)\
	echeck_status_m(log, ECHECK_FUNC, __FILE__, __LINE__, val, msg)

#define check_status_m(val, msg)\
	echeck_status_m(NULL, ECHECK_FUNC, __FILE__, __LINE__, val, msg)

#define lcheck_status(log, val)\
	echeck_status_m(log, ECHECK_FUNC, __FILE__, __LINE__, val, NULL)

#define check_status(val)\
	echeck_status_m(NULL, ECHECK_FUNC, __FILE__, __LINE__, val, NULL)

Echeck_end_C_functions
#undef Echeck_begin_C_functions
#undef Echeck_end_C_functions
#endif /* ECHECK_H */
