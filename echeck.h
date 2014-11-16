/* echeck.h */
#ifndef _ECHECK_H_
#define _ECHECK_H_

#include <stdlib.h>		/* for size_t */
#include <stdio.h>		/* for FILE */

int check_char_m(char actual, char expected, const char *msg);
int check_char(char actual, char expected);

int check_int_m(int actual, int expected, const char *msg);
int check_int(int actual, int expected);

int check_long_m(long actual, long expected, const char *msg);
int check_long(long actual, long expected);

int check_str_m(const char *actual, const char *expected, const char *msg);
int check_str(const char *actual, const char *expected);

int check_ptr_m(const void *actual, const void *expected, const char *msg);
int check_ptr(const void *actual, const void *expected);

int check_unsigned_int_m(unsigned int actual, unsigned int expected,
			 const char *msg);
int check_unsigned_int(unsigned int actual, unsigned int expected);

int check_unsigned_long_m(unsigned long actual, unsigned long expected,
			  const char *msg);
int check_unsigned_long(unsigned long actual, unsigned long expected);

int check_byte_array_m(unsigned char *actual, size_t actual_len,
		       unsigned char *expected, size_t expected_len,
		       const char *msg);
int check_byte_array(unsigned char *actual, size_t actual_len,
		     unsigned char *expected, size_t expected_len);

/* these are for setting global variables */
unsigned int set_echeck_global_exit_on_fail(unsigned int new_val);
FILE *set_echeck_stderr(FILE * new_err);

#endif /* _ECHECK_H_ */
