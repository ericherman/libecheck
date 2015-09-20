/* echeck.h */
#ifndef ECHECK_H
#define ECHECK_H

#include "echeck_impl.h"

/*check char*/
#define fcheck_char_m(log, actual, expected, msg)\
	echeck_char_m(log, __FILE__, __LINE__, actual, expected, msg)

#define check_char_m(actual, expected, msg)\
	echeck_char_m(stderr, __FILE__, __LINE__, actual, expected, msg)

#define fcheck_char(log, actual, expected)\
	echeck_char_m(log, __FILE__, __LINE__, actual, expected, NULL)

#define check_char(actual, expected)\
	echeck_char_m(stderr, __FILE__, __LINE__, actual, expected, NULL)

/* check int */
#define fcheck_int_m(log, actual, expected, msg)\
	echeck_int_m(log, __FILE__, __LINE__, actual, expected, msg)

#define check_int_m(actual, expected, msg)\
	echeck_int_m(stderr, __FILE__, __LINE__, actual, expected, msg)

#define fcheck_int(log, actual, expected)\
	echeck_int_m(log, __FILE__, __LINE__, actual, expected, NULL)

#define check_int(actual, expected)\
	echeck_int_m(stderr, __FILE__, __LINE__, actual, expected, NULL)

/* check long */
#define fcheck_long_m(log, actual, expected, msg)\
	echeck_long_m(log, __FILE__, __LINE__, actual, expected, msg)

#define check_long_m(actual, expected, msg)\
	echeck_long_m(stderr, __FILE__, __LINE__, actual, expected, msg)

#define fcheck_long(log, actual, expected)\
	echeck_long_m(log, __FILE__, __LINE__, actual, expected, NULL)

#define check_long(actual, expected)\
	echeck_long_m(stderr, __FILE__, __LINE__, actual, expected, NULL)

/* check str */
#define fcheck_str_m(log, actual, expected, msg)\
	echeck_str_m(log, __FILE__, __LINE__, actual, expected, msg)

#define check_str_m(actual, expected, msg)\
	echeck_str_m(stderr, __FILE__, __LINE__, actual, expected, msg)

#define fcheck_str(log, actual, expected)\
	echeck_str_m(log, __FILE__, __LINE__, actual, expected, NULL)

#define check_str(actual, expected)\
	echeck_str_m(stderr, __FILE__, __LINE__, actual, expected, NULL)

/* check ptr */
#define fcheck_ptr_m(log, actual, expected, msg)\
	echeck_ptr_m(log, __FILE__, __LINE__, actual, expected, msg)

#define check_ptr_m(actual, expected, msg)\
	echeck_ptr_m(stderr, __FILE__, __LINE__, actual, expected, msg)

#define fcheck_ptr(log, actual, expected)\
	echeck_ptr_m(log, __FILE__, __LINE__, actual, expected, NULL)

#define check_ptr(actual, expected)\
	echeck_ptr_m(stderr, __FILE__, __LINE__, actual, expected, NULL)

/* check unsigned int */
#define fcheck_unsigned_int_m(log, actual, expected, msg)\
	echeck_unsigned_int_m(log, __FILE__, __LINE__, actual, expected, msg)

#define check_unsigned_int_m(actual, expected, msg)\
	echeck_unsigned_int_m(stderr, __FILE__, __LINE__, actual, expected,\
	msg)

#define fcheck_unsigned_int(log, actual, expected)\
	echeck_unsigned_int_m(log, __FILE__, __LINE__, actual, expected, NULL)

#define check_unsigned_int(actual, expected)\
	echeck_unsigned_int_m(stderr, __FILE__, __LINE__, actual, expected,\
	NULL)

/* check unsigned long */
#define fcheck_unsigned_long_m(log, actual, expected, msg)\
	echeck_unsigned_long_m(log, __FILE__, __LINE__, actual, expected, msg)

#define check_unsigned_long_m(actual, expected, msg)\
	echeck_unsigned_long_m(stderr, __FILE__, __LINE__, actual, expected,\
	msg)

#define fcheck_unsigned_long(log, actual, expected)\
	echeck_unsigned_long_m(log, __FILE__, __LINE__, actual, expected,\
	NULL)

#define check_unsigned_long(actual, expected)\
	echeck_unsigned_long_m(stderr, __FILE__, __LINE__, actual, expected,\
	 NULL)

/* check size_t */
#define fcheck_size_t_m(log, actual, expected, msg)\
	echeck_size_t_m(log, __FILE__, __LINE__, actual, expected, msg)

#define check_size_t_m(actual, expected, msg)\
	echeck_size_t_m(stderr, __FILE__, __LINE__, actual, expected, msg)

#define fcheck_size_t(log, actual, expected)\
	echeck_size_t_m(log, __FILE__, __LINE__, actual, expected, NULL)

#define check_size_t(actual, expected)\
	echeck_size_t_m(stderr, __FILE__, __LINE__, actual, expected, NULL)

/* check byte_array */
#define fcheck_byte_array_m(log, actual, actual_len, expected, expected_len,\
	msg)\
	echeck_byte_array_m(log, __FILE__, __LINE__, actual, actual_len,\
	expected, expected_len, msg)

#define check_byte_array_m(actual, actual_len, expected, expected_len, msg)\
	echeck_byte_array_m(stderr, __FILE__, __LINE__, actual, actual_len,\
	expected, expected_len, msg)

#define fcheck_byte_array(log, actual, actual_len, expected, expected_len)\
	echeck_byte_array_m(log, __FILE__, __LINE__, actual, actual_len,\
	expected, expected_len, NULL)

#define check_byte_array(actual, actual_len, expected, expected_len)\
	echeck_byte_array_m(stderr, __FILE__, __LINE__, actual, actual_len,\
	expected, expected_len, NULL)

#endif /* ECHECK_H */
