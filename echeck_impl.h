/* echeck_imp.h */
#ifndef ECHECK_IMPL_H
#define ECHECK_IMPL_H

#include <stdlib.h>		/* for size_t */
#include <stdio.h>		/* for FILE */

int check_char_efl(char actual, char expected, const char *msg,
		   FILE *err, const char *file, int line);

int check_int_efl(int actual, int expected, const char *msg, FILE *err,
		  const char *file, int line);

int check_long_efl(long actual, long expected, const char *msg,
		   FILE *err, const char *file, int line);

int check_str_efl(const char *actual, const char *expected, const char *msg,
		  FILE *err, const char *file, int line);

int check_ptr_efl(const void *actual, const void *expected, const char *msg,
		  FILE *err, const char *file, int line);

int check_unsigned_int_efl(unsigned int actual, unsigned int expected,
			   const char *msg, FILE *err, const char *file,
			   int line);

int check_unsigned_long_efl(unsigned long actual, unsigned long expected,
			    const char *msg, FILE *err, const char *file,
			    int line);

int check_size_t_efl(size_t actual, size_t expected, const char *msg,
		     FILE *err, const char *file, int line);

int check_byte_array_efl(unsigned char *actual, size_t actual_len,
			 unsigned char *expected, size_t expected_len,
			 const char *msg, FILE *err, const char *file,
			 int line);

#endif /* ECHECK_IMPL_H */
