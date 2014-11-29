#include "echeck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int ECHECK_GLOBAL_EXIT_ON_FAIL = 0;
FILE *ECHECK_STDERR;

unsigned int set_echeck_global_exit_on_fail(unsigned int new_val)
{
	unsigned int previous = ECHECK_GLOBAL_EXIT_ON_FAIL;
	ECHECK_GLOBAL_EXIT_ON_FAIL = new_val;
	return previous;
}

FILE *set_echeck_stderr(FILE * new_err)
{
	FILE *previous = ECHECK_STDERR;
	ECHECK_STDERR = new_err;
	return previous;
}

FILE *echeck_err(void)
{
	return (ECHECK_STDERR == NULL) ? stderr : ECHECK_STDERR;
}

int check_char_m(char actual, char expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(echeck_err(), "FAIL: %s Expected '%c' but was '%c'\n",
		msg, expected, actual);
	if (ECHECK_GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_char(char actual, char expected)
{
	return check_char_m(actual, expected, "");
}

int check_unsigned_long_m(unsigned long actual, unsigned long expected,
			  const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(echeck_err(), "FAIL: %s Expected %lu but was %lu\n",
		msg, expected, actual);
	if (ECHECK_GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_unsigned_long(unsigned long actual, unsigned long expected)
{
	return check_unsigned_long_m(actual, expected, "");
}

int check_unsigned_int_m(unsigned int actual, unsigned int expected,
			 const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(echeck_err(), "FAIL: %s Expected %u but was %u\n",
		msg, expected, actual);
	if (ECHECK_GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_unsigned_int(unsigned int actual, unsigned int expected)
{
	return check_unsigned_int_m(actual, expected, "");
}

int check_int_m(int actual, int expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(echeck_err(), "FAIL: %s Expected %d but was %d\n",
		msg, expected, actual);
	if (ECHECK_GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_int(int actual, int expected)
{
	return check_int_m(actual, expected, "");
}

int check_long_m(long actual, long expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(echeck_err(), "FAIL: %s Expected %ld but was %ld\n",
		msg, expected, actual);
	if (ECHECK_GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_size_t_m(size_t actual, size_t expected, const char *msg)
{
	return check_unsigned_long_m((unsigned long)actual,
				     (unsigned long)expected, msg);
}

int check_size_t(size_t actual, size_t expected)
{
	return check_size_t_m(actual, expected, "");
}

int check_long(long actual, long expected)
{
	return check_long_m(actual, expected, "");
}

int check_str_m(const char *actual, const char *expected, const char *msg)
{
	if (strcmp(expected, actual) == 0) {
		return 0;
	}
	fprintf(echeck_err(), "FAIL: %s Expected '%s' but was '%s'\n",
		msg, expected, actual);
	if (ECHECK_GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_str(const char *actual, const char *expected)
{
	return check_str_m(actual, expected, "");
}

int check_byte_array_m(unsigned char *actual, size_t actual_len,
		       unsigned char *expected, size_t expected_len,
		       const char *name)
{

	unsigned int i;

	if (actual_len != expected_len) {
		fprintf(echeck_err(),
			"actual/expected length mis-match %d != %d\n",
			actual_len, expected_len);
		goto fail;
	}
	for (i = 0; i < actual_len; i++) {
		if (actual[i] != expected[i]) {
			fprintf(echeck_err(), "buffers differ at %d\n", i);
			goto fail;
		}
	}
	return 0;

      fail:
	fprintf(echeck_err(), "FAIL: %s\n", name);
	for (i = 0; i < actual_len; i++) {
		fprintf(echeck_err(), "actual[%d]=%02x\n", i, actual[i]);
	}
	for (i = 0; i < expected_len; i++) {
		fprintf(echeck_err(), "expected[%d]=%02x\n", i, expected[i]);
	}
	if (ECHECK_GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_byte_array(unsigned char *expected, size_t expected_len,
		     unsigned char *actual, size_t actual_len)
{

	return check_byte_array_m(actual, actual_len, expected, expected_len,
				  "");
}

int check_ptr_m(const void *actual, const void *expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(echeck_err(), "FAIL: %s Expected '%p' but was '%p'\n",
		msg, expected, actual);
	if (ECHECK_GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_ptr(const void *actual, const void *expected)
{
	return check_ptr_m(actual, expected, "");
}
