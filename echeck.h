/* echeck.h */
#ifndef ECHECK_H
#define ECHECK_H

#include "echeck_impl.h"

/*check char*/
#define check_char_ml(actual, expected, msg, log)\
	check_char_efl(actual, expected, msg, log, __FILE__, __LINE__)

#define check_char_m(actual, expected, msg)\
	check_char_efl(actual, expected, msg, stderr, __FILE__, __LINE__)

#define check_char_l(actual, expected, log)\
	check_char_efl(actual, expected, NULL, log, __FILE__, __LINE__)

#define check_char(actual, expected)\
	check_char_efl(actual, expected, NULL, stderr, __FILE__, __LINE__)

/* check int */
#define check_int_ml(actual, expected,  msg, log)\
	check_int_efl(actual, expected, msg, log, __FILE__, __LINE__)

#define check_int_m(actual, expected,  msg)\
	check_int_efl(actual, expected, msg, stderr, __FILE__, __LINE__)

#define check_int_l(actual, expected, log)\
	check_int_efl(actual, expected, NULL, log, __FILE__, __LINE__)

#define check_int(actual, expected)\
	check_int_efl(actual, expected, NULL, stderr, __FILE__, __LINE__)

/* check long */
#define check_long_ml(actual, expected,  msg, log)\
	check_long_efl(actual, expected, msg, log, __FILE__, __LINE__)

#define check_long_m(actual, expected,  msg)\
	check_long_efl(actual, expected, msg, stderr, __FILE__, __LINE__)

#define check_long_l(actual, expected, log)\
	check_long_efl(actual, expected, NULL, log, __FILE__, __LINE__)

#define check_long(actual, expected)\
	check_long_efl(actual, expected, NULL, stderr, __FILE__, __LINE__)

/* check str */
#define check_str_ml(actual, expected,  msg, log)\
	check_str_efl(actual, expected, msg, log, __FILE__, __LINE__)

#define check_str_m(actual, expected,  msg)\
	check_str_efl(actual, expected, msg, stderr, __FILE__, __LINE__)

#define check_str_l(actual, expected, log)\
	check_str_efl(actual, expected, NULL, log, __FILE__, __LINE__)

#define check_str(actual, expected)\
	check_str_efl(actual, expected, NULL, stderr, __FILE__, __LINE__)

/* check ptr */
#define check_ptr_ml(actual, expected,  msg, log)\
	check_ptr_efl(actual, expected, msg, log, __FILE__, __LINE__)

#define check_ptr_m(actual, expected,  msg)\
	check_ptr_efl(actual, expected, msg, stderr, __FILE__, __LINE__)

#define check_ptr_l(actual, expected, log)\
	check_ptr_efl(actual, expected, NULL, log, __FILE__, __LINE__)

#define check_ptr(actual, expected)\
	check_ptr_efl(actual, expected, NULL, stderr, __FILE__, __LINE__)

/* check unsigned int */
#define check_unsigned_int_ml(actual, expected,  msg, log)\
	check_unsigned_int_efl(actual, expected, msg, log, __FILE__, __LINE__)

#define check_unsigned_int_m(actual, expected,  msg)\
	check_unsigned_int_efl(actual, expected, msg, stderr, __FILE__,\
	__LINE__)

#define check_unsigned_int_l(actual, expected, log)\
	check_unsigned_int_efl(actual, expected, NULL, log, __FILE__, __LINE__)

#define check_unsigned_int(actual, expected)\
	check_unsigned_int_efl(actual, expected, NULL, stderr, __FILE__,\
	__LINE__)

/* check unsigned long */
#define check_unsigned_long_ml(actual, expected,  msg, log)\
	check_unsigned_long_efl(actual, expected, msg, log, __FILE__, __LINE__)

#define check_unsigned_long_m(actual, expected,  msg)\
	check_unsigned_long_efl(actual, expected, msg, stderr, __FILE__,\
	__LINE__)

#define check_unsigned_long_l(actual, expected, log)\
	check_unsigned_long_efl(actual, expected, NULL, log, __FILE__,\
	__LINE__)

#define check_unsigned_long(actual, expected)\
	check_unsigned_long_efl(actual, expected, NULL, stderr, __FILE__,\
	__LINE__)

/* check size_t */
#define check_size_t_ml(actual, expected,  msg, log)\
	check_size_t_efl(actual, expected, msg, log, __FILE__, __LINE__)

#define check_size_t_m(actual, expected,  msg)\
	check_size_t_efl(actual, expected, msg, stderr, __FILE__, __LINE__)

#define check_size_t_l(actual, expected, log)\
	check_size_t_efl(actual, expected, NULL, log, __FILE__, __LINE__)

#define check_size_t(actual, expected)\
	check_size_t_efl(actual, expected, NULL, stderr, __FILE__, __LINE__)

/* check byte_array */
#define check_byte_array_ml(actual, actual_len, expected, expected_len, msg,\
	log)\
	check_byte_array_efl(actual, actual_len, expected, expected_len, msg,\
	log, __FILE__, __LINE__)

#define check_byte_array_m(actual, actual_len, expected, expected_len,  msg)\
	check_byte_array_efl(actual, actual_len, expected, expected_len, msg,\
	stderr, __FILE__, __LINE__)

#define check_byte_array_l(actual, actual_len, expected, expected_len, log)\
	check_byte_array_efl(actual, actual_len, expected, expected_len, NULL,\
	log, __FILE__, __LINE__)

#define check_byte_array(actual, actual_len, expected, expected_len)\
	check_byte_array_efl(actual, actual_len, expected, expected_len, NULL,\
	stderr, __FILE__, __LINE__)

#endif /* ECHECK_H */
