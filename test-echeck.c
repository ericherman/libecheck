#include <stdio.h>
#include <string.h>

#include "echeck.h"

int err_contains(const char *filename, char *expected[], size_t str_count)
{
	FILE *capture;
	char buffer[4096];
	size_t i;
	unsigned int msg_found;

	capture = fopen(filename, "r");
	if (capture) {
		msg_found = 0;
		if (fgets(buffer, sizeof buffer, capture)) {
			for (i = 0; i < str_count; i++) {
				if (strstr(buffer, expected[i])) {
					msg_found++;
				} else {
					fprintf(stderr, "'%s' not in: %s\n",
						expected[i], buffer);
				}
			}
		}
		fclose(capture);
	}
	return str_count - msg_found;
}

int check_check_str(const char *filename)
{
	FILE *orig, *capture;
	char *strs[2];
	int failures = 0;

	strs[0] = "same";
	strs[1] = "different";

	failures += check_str("same", "same");

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_str("same", "different")) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_str\n", failures);
	}
	return failures;
}

int check_check_str_m(const char *filename)
{
	FILE *orig, *capture;
	char *strs[3];
	int failures = 0;

	strs[0] = "weirdly";
	strs[1] = "different";
	strs[2] = "contextual info";

	failures += check_str_m("same", "same", "contextual info");

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_str_m("weirdly", "different", "contextual info")) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_str_m\n", failures);
	}
	return failures;
}

/*

int check_int_m(int actual, int expected, const char *msg);
int check_int(int actual, int expected);

int check_ptr_m(const void *actual, const void *expected, const char *msg);
int check_ptr(const void *actual, const void *expected);

int check_unsigned_int_m(unsigned int actual, unsigned int expected,
			 const char *msg);
int check_unsigned_int(unsigned int actual, unsigned int expected);

int check_byte_array_m(unsigned char *actual, size_t actual_len,
		       unsigned char *expected, size_t expected_len,
		       const char *msg);
int check_byte_array(unsigned char *actual, size_t actual_len,
		     unsigned char *expected, size_t expected_len);
*/
int main(int argc, char *argv[])
{
	const char *filename;
	int failures = 0;

	if (argc > 1) {
		filename = argv[1];
	} else {
		filename = "/tmp/stderr.log";
	}

	failures += check_check_str(filename);
	failures += check_check_str_m(filename);

	if (failures) {
		fprintf(stderr, "%d failures in total\n", failures);
	}
	return failures;
}
