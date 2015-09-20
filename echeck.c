#include "echeck_impl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_char_efl(char actual, char expected, const char *msg, FILE *err,
		   const char *file, int line)
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

int check_unsigned_long_efl(unsigned long actual, unsigned long expected,
			    const char *msg, FILE *err, const char *file,
			    int line)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err, "FAIL: %s%sExpected %lu but was %lu [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int check_unsigned_int_efl(unsigned int actual, unsigned int expected,
			   const char *msg, FILE *err, const char *file,
			   int line)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err, "FAIL: %s%sExpected %u but was %u [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int check_int_efl(int actual, int expected, const char *msg, FILE *err,
		  const char *file, int line)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err, "FAIL: %s%sExpected %d but was %d [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int check_long_efl(long actual, long expected, const char *msg, FILE *err,
		   const char *file, int line)
{
	if (expected == actual) {
		return 0;
	}
	fprintf(err, "FAIL: %s%sExpected %ld but was %ld [File: %s Line: %d]\n",
		(msg == NULL) ? "" : msg, (msg == NULL) ? "" : " ", expected,
		actual, file, line);
	return 1;
}

int check_size_t_efl(size_t actual, size_t expected, const char *msg, FILE *err,
		     const char *file, int line)
{
	return check_unsigned_long_efl((unsigned long)actual,
				       (unsigned long)expected, msg, err, file,
				       line);
}

int check_str_efl(const char *actual, const char *expected, const char *msg,
		  FILE *err, const char *file, int line)
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

int check_byte_array_efl(unsigned char *actual, size_t actual_len,
			 unsigned char *expected, size_t expected_len,
			 const char *name, FILE *err, const char *file,
			 int line)
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
	fprintf(err, "FAIL: %s [File: %s Line: %d]\n", name, file, line);
	for (i = 0; i < actual_len; i++) {
		fprintf(err, "actual[%d]=%02x\n", i, actual[i]);
	}
	for (i = 0; i < expected_len; i++) {
		fprintf(err, "expected[%d]=%02x\n", i, expected[i]);
	}
	return 1;
}

int check_ptr_efl(const void *actual, const void *expected, const char *msg,
		  FILE *err, const char *file, int line)
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
