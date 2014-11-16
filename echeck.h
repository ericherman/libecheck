/* echeck.h */
#ifndef _ECHECK_H_
#define _ECHECK_H_

void check_chars(char actual, char expected, const char *msg);
void check_char(char actual, char expected);

void check_ints(int actual, int expected, const char *msg);
void check_int(int actual, int expected);

void check_strs(const char *actual, const char *expected, const char *msg);
void check_str(const char *actual, const char *expected);

void check_ptrs(const void *actual, const void *expected, const char *msg);
void check_ptr(const void *actual, const void *expected);

void check_unsigned_ints(unsigned int actual, unsigned int expected,
			 const char *msg);
void check_unsigned_int(unsigned int actual, unsigned int expected);

void check_byte_arrays(unsigned char *actual, unsigned int actual_len,
		       unsigned char *expected, unsigned int expected_len,
		       const char *msg);
void check_byte_array(unsigned char *actual, unsigned int actual_len,
		      unsigned char *expected, unsigned int expected_len);

#endif /* _ECHECK_H_ */
