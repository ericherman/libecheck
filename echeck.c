#include "echeck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_char_m(char actual, char expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(stderr, "FAIL: %s Expected '%c' but was '%c'\n",
		msg, expected, actual);
	if (GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_char(char actual, char expected)
{
	return check_char_m(actual, expected, "");
}

int check_unsigned_int_m(unsigned int actual, unsigned int expected,
			 const char *msg)
{

	if (expected == actual) {
		return 0;
	}
	fprintf(stderr, "FAIL: %s Expected %d but was %d\n",
		msg, expected, actual);
	if (GLOBAL_EXIT_ON_FAIL) {
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
	fprintf(stderr, "FAIL: %s Expected %d but was %d\n",
		msg, expected, actual);
	if (GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_int(int actual, int expected)
{
	return check_unsigned_int_m(actual, expected, "");
}

int check_str_m(const char *actual, const char *expected, const char *msg)
{
	if (strcmp(expected, actual) == 0) {
		return 0;
	}
	fprintf(stderr, "FAIL: %s Expected '%s' but was '%s'\n",
		msg, expected, actual);
	if (GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_str(const char *actual, const char *expected)
{
	return check_str_m(actual, expected, "");
}

int check_byte_array_m(unsigned char *actual, unsigned int actual_len,
		       unsigned char *expected, unsigned int expected_len,
		       const char *name)
{

	unsigned int i;

	if (actual_len != expected_len) {
		fprintf(stderr, "actual/expected length mis-match %d != %d\n",
			actual_len, expected_len);
		goto fail;
	}
	for (i = 0; i < actual_len; i++) {
		if (actual[i] != expected[i]) {
			fprintf(stderr, "buffers differ at %d\n", i);
			goto fail;
		}
	}
	return 0;

      fail:
	fprintf(stderr, "FAIL: %s\n", name);
	for (i = 0; i < actual_len; i++) {
		fprintf(stderr, "actual[%d]=%02x\n", i, actual[i]);
	}
	for (i = 0; i < expected_len; i++) {
		fprintf(stderr, "expected[%d]=%02x\n", i, expected[i]);
	}
	if (GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_byte_array(unsigned char *expected, unsigned int expected_len,
		     unsigned char *actual, unsigned int actual_len)
{

	return check_byte_array_m(actual, actual_len, expected, expected_len,
				  "");
}

int check_ptr_m(const void *actual, const void *expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(stderr, "FAIL: %s Expected '%p' but was '%p'\n",
		msg, expected, actual);
	if (GLOBAL_EXIT_ON_FAIL) {
		exit(1);
	}
	return 1;
}

int check_ptr(const void *actual, const void *expected)
{
	return check_ptr_m(actual, expected, "");
}
