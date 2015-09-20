/* echeck_imp.h */
#ifndef ECHECK_IMPL_H
#define ECHECK_IMPL_H

#include <stdlib.h>		/* for size_t */
#include <stdio.h>		/* for FILE */

int echeck_char_m(FILE *err, const char *file, int line, char actual,
		  char expected, const char *msg);

int echeck_int_m(FILE *err, const char *file, int line, int actual,
		 int expected, const char *msg);

int echeck_long_m(FILE *err, const char *file, int line, long actual,
		  long expected, const char *msg);

int echeck_str_m(FILE *err, const char *file, int line, const char *actual,
		 const char *expected, const char *msg);

int echeck_ptr_m(FILE *err, const char *file, int line, const void *actual,
		 const void *expected, const char *msg);

int echeck_unsigned_int_m(FILE *err, const char *file, int line,
			  unsigned int actual, unsigned int expected,
			  const char *msg);

int echeck_unsigned_long_m(FILE *err, const char *file, int line,
			   unsigned long actual, unsigned long expected,
			   const char *msg);

int echeck_size_t_m(FILE *err, const char *file, int line, size_t actual,
		    size_t expected, const char *msg);

int echeck_byte_array_m(FILE *err, const char *file, int line,
			unsigned char *actual, size_t actual_len,
			unsigned char *expected, size_t expected_len,
			const char *msg);

#endif /* ECHECK_IMPL_H */
