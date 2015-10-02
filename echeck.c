#include "echeck_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int echeck_char_m(FILE *err, const char *file, int line, char actual,
		  char expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err,
		"FAIL: %s%sExpected '%c' but was '%c' [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int echeck_unsigned_long_m(FILE *err, const char *file, int line,
			   unsigned long actual, unsigned long expected,
			   const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err, "FAIL: %s%sExpected %lu but was %lu [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int echeck_unsigned_int_m(FILE *err, const char *file, int line,
			  unsigned int actual, unsigned int expected,
			  const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err, "FAIL: %s%sExpected %u but was %u [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int echeck_int_m(FILE *err, const char *file, int line, int actual,
		 int expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err, "FAIL: %s%sExpected %d but was %d [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int echeck_long_m(FILE *err, const char *file, int line, long actual,
		  long expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err, "FAIL: %s%sExpected %ld but was %ld [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int echeck_size_t_m(FILE *err, const char *file, int line, size_t actual,
		    size_t expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err, "FAIL: %s%sExpected %lu but was %lu [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ",
		(unsigned long)expected, (unsigned long)actual, file, line);
	return 1;
}

int echeck_str_m(FILE *err, const char *file, int line, const char *actual,
		 const char *expected, const char *msg)
{
	if (strcmp(expected, actual) == 0) {
		return 0;
	}
	fprintf(err,
		"FAIL: %s%sExpected '%s' but was '%s' [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int echeck_byte_array_m(FILE *err, const char *file, int line,
			unsigned char *actual, size_t actual_len,
			unsigned char *expected, size_t expected_len,
			const char *msg)
{

	size_t i;
	const char *fmt;

	fmt = "actual/expected length mis-match %d != %d [File: %s Line: %d]\n";

	if (actual_len != expected_len) {
		fprintf(err, fmt, actual_len, expected_len, file, line);
		goto fail;
	}
	for (i = 0; i < actual_len; i++) {
		if (actual[i] != expected[i]) {
			fprintf(err, "buffers differ at %d\n", i);
			goto fail;
		}
	}
	return 0;

fail:
	fprintf(err, "FAIL: %s%s[File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", file, line);
	for (i = 0; i < actual_len; i++) {
		fprintf(err, "actual[%d]=%02x\n", i, actual[i]);
	}
	for (i = 0; i < expected_len; i++) {
		fprintf(err, "expected[%d]=%02x\n", i, expected[i]);
	}
	return 1;
}

int echeck_ptr_m(FILE *err, const char *file, int line, const void *actual,
		 const void *expected, const char *msg)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err,
		"FAIL: %s%sExpected '%p' but was '%p' [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}
