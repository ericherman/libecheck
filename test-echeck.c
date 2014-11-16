#include <stdio.h>
#include <string.h>

#include "echeck.h"

int err_contains(const char *filename, char *expected[], size_t str_count)
{
	FILE *capture;
	char buffer[4096];
	size_t i, len;
	unsigned int msg_found;

	capture = fopen(filename, "r");
	msg_found = 0;
	if (capture) {
		len = 0;
		while (fgets(buffer + len, (sizeof buffer) - len, capture)) {
			len = strlen(buffer);
			if (len >= (sizeof buffer)) {
				len = (sizeof buffer);
				buffer[len - 1] = '\0';
			}
		}
		if (len) {
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

int check_check_char(const char *filename)
{
	FILE *orig, *capture;
	char *strs[2];
	int failures = 0;

	strs[0] = "a";
	strs[1] = "b";

	failures += check_char('a', 'a');

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_char('a', 'b')) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_char\n", failures);
	}
	return failures;
}

int check_check_char_m(const char *filename)
{
	FILE *orig, *capture;
	char *strs[3];
	int failures = 0;

	strs[0] = "a";
	strs[1] = "b";
	strs[2] = "contextual info";

	failures += check_char_m('a', 'a', "contextual info");

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_char_m('a', 'b', "contextual info")) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_char_m\n",
			failures);
	}
	return failures;
}

int check_check_int(const char *filename)
{
	FILE *orig, *capture;
	char *strs[2];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "-5";

	failures += check_int(5, 5);

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_int(5, -5)) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_int\n", failures);
	}
	return failures;
}

int check_check_int_m(const char *filename)
{
	FILE *orig, *capture;
	char *strs[3];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "-5";
	strs[2] = "contextual info";

	failures += check_int_m(5, 5, "contextual info");

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_int_m(5, -5, "contextual info")) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_int_m\n", failures);
	}
	return failures;
}

int check_check_unsigned_int(const char *filename)
{
	FILE *orig, *capture;
	char *strs[2];
	int failures = 0;

	strs[0] = "1";
	strs[1] = "4294967295";

	failures += check_unsigned_int(5, 5);

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_unsigned_int(1, -1)) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_unsigned_int\n",
			failures);
	}
	return failures;
}

int check_check_unsigned_int_m(const char *filename)
{
	FILE *orig, *capture;
	char *strs[3];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "6";
	strs[2] = "contextual info";

	failures += check_unsigned_int_m(5, 5, "contextual info");

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_unsigned_int_m(5, 6, "contextual info")) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_unsigned_int_m\n",
			failures);
	}
	return failures;
}

int check_check_long(const char *filename)
{
	FILE *orig, *capture;
	char *strs[2];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "-5";

	failures += check_long(5L, 5L);

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_long(5L, -5L)) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_long\n", failures);
	}
	return failures;
}

int check_check_long_m(const char *filename)
{
	FILE *orig, *capture;
	char *strs[3];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "-5";
	strs[2] = "contextual info";

	failures += check_long_m(5L, 5L, "contextual info");

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_long_m(5L, -5L, "contextual info")) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_long_m\n",
			failures);
	}
	return failures;
}

int check_check_unsigned_long(const char *filename)
{
	FILE *orig, *capture;
	char *strs[2];
	int failures = 0;

	strs[0] = "1";
	strs[1] = "4294967295";

	failures += check_unsigned_long(5L, 5L);

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_unsigned_long(1L, -1L)) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_unsigned_long\n",
			failures);
	}
	return failures;
}

int check_check_unsigned_long_m(const char *filename)
{
	FILE *orig, *capture;
	char *strs[3];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "6";
	strs[2] = "contextual info";

	failures += check_unsigned_long_m(5L, 5L, "contextual info");

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_unsigned_long_m(5L, 6L, "contextual info")) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_unsigned_long_m\n",
			failures);
	}
	return failures;
}

int check_check_byte_array(const char *filename)
{
	FILE *orig, *capture;
	char *strs[2];
	int failures = 0;

	unsigned char bytes_a[2] = { 0x00, 0xFF };
	unsigned char bytes_b[2] = { 0x00, 0xFF };
	unsigned char bytes_c[2] = { 0x13, 0x10 };

	strs[0] = "";
	strs[1] = "";

	failures += check_byte_array(bytes_a, 2, bytes_b, 2);

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_byte_array(bytes_b, 2, bytes_c, 2)) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_byte_array\n",
			failures);
	}
	return failures;
}

int check_check_byte_array_m(const char *filename)
{
	FILE *orig, *capture;
	char *strs[3];
	int failures = 0;

	unsigned char bytes_a[2] = { 0x00, 0xFF };
	unsigned char bytes_b[2] = { 0x00, 0xFF };
	unsigned char bytes_c[2] = { 0x13, 0x10 };

	strs[0] = "";
	strs[1] = "";
	strs[2] = "contextual info";

	failures +=
	    check_byte_array_m(bytes_a, 2, bytes_b, 2, "contextual info");

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_byte_array_m(bytes_b, 2, bytes_c, 2, "contextual info")) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_byte_array_m\n",
			failures);
	}
	return failures;
}

int check_check_ptr(const char *filename)
{
	FILE *orig, *capture;
	char *strs[2];
	int failures = 0;

	strs[0] = "Expected";	/* this is cheating */
	strs[1] = "but was";

	failures += check_ptr(strs[0], strs[0]);

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_ptr(strs[0], strs[1])) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_ptr\n", failures);
	}
	return failures;
}

int check_check_ptr_m(const char *filename)
{
	FILE *orig, *capture;
	char *strs[3];
	int failures = 0;

	strs[0] = "contextual info";
	strs[1] = "foo";

	failures += check_ptr_m(strs[0], strs[0], "contextual info");

	capture = fopen(filename, "w");
	orig = set_echeck_stderr(capture);
	if (0 == check_ptr_m(strs[0], strs[1], "contextual info")) {
		failures++;
	}
	set_echeck_stderr(orig);
	fclose(capture);

	failures += err_contains(filename, strs, 1);
	if (failures) {
		fprintf(stderr, "%d failures in check_check_ptr_m\n", failures);
	}
	return failures;
}

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
	failures += check_check_char(filename);
	failures += check_check_char_m(filename);

	failures += check_check_int(filename);
	failures += check_check_int_m(filename);
	failures += check_check_unsigned_int(filename);
	failures += check_check_unsigned_int_m(filename);

	failures += check_check_long(filename);
	failures += check_check_long_m(filename);
	failures += check_check_unsigned_long(filename);
	failures += check_check_unsigned_long_m(filename);

	failures += check_check_ptr(filename);
	failures += check_check_ptr_m(filename);
	failures += check_check_byte_array(filename);
	failures += check_check_byte_array_m(filename);

	if (failures) {
		fprintf(stderr, "%d failures in total\n", failures);
	}
	return failures;
}
