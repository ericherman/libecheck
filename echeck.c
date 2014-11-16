#include "echeck.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_chars(char actual, char expected, const char *msg)
{
	if (expected == actual) {
		return;
	}
	fprintf(stderr, "FAIL: %s Expected '%c' but was '%c'\n",
		msg, expected, actual);
	exit(1);
}

void check_char(char actual, char expected)
{
	check_chars(actual, expected, "");
}

void check_unsigned_ints(unsigned int actual, unsigned int expected,
			 const char *msg)
{

	if (expected == actual) {
		return;
	}
	fprintf(stderr, "FAIL: %s Expected %d but was %d\n",
		msg, expected, actual);
	exit(1);
}

void check_unsigned_int(unsigned int actual, unsigned int expected)
{
	check_unsigned_ints(actual, expected, "");
}

void check_ints(int actual, int expected, const char *msg)
{
	if (expected == actual) {
		return;
	}
	fprintf(stderr, "FAIL: %s Expected %d but was %d\n",
		msg, expected, actual);
	exit(1);
}

void check_int(int actual, int expected)
{
	check_unsigned_ints(actual, expected, "");
}

void check_strs(const char *actual, const char *expected, const char *msg)
{
	if (strcmp(expected, actual) == 0) {
		return;
	}
	fprintf(stderr, "FAIL: %s Expected '%s' but was '%s'\n",
		msg, expected, actual);
	exit(1);
}

void check_str(const char *actual, const char *expected)
{
	check_strs(actual, expected, "");
}

void check_byte_arrays(unsigned char *actual, unsigned int actual_len,
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
	return;

fail:
	fprintf(stderr, "FAIL: %s\n", name);
	for (i = 0; i < actual_len; i++) {
		fprintf(stderr, "actual[%d]=%02x\n", i, actual[i]);
	}
	for (i = 0; i < expected_len; i++) {
		fprintf(stderr, "expected[%d]=%02x\n", i, expected[i]);
	}
	exit(1);
}

void check_byte_array(unsigned char *expected, unsigned int expected_len,
		      unsigned char *actual, unsigned int actual_len)
{

	check_byte_arrays(actual, actual_len, expected, expected_len, "");
}

void check_ptrs(const void *actual, const void *expected, const char *msg)
{
	if (expected == actual) {
		return;
	}
	fprintf(stderr, "FAIL: %s Expected '%p' but was '%p'\n",
		msg, expected, actual);
	exit(1);
}

void check_ptr(const void *actual, const void *expected)
{
	check_ptrs(actual, expected, "");
}
