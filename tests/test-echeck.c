#include <stdio.h>
#include <string.h>

#include "echeck.h"

/* TODO: do something portable alternative here */
#define ECHECK_PATH_MAX 4096
#define ECHECK_SEP '/'

static char *set_log(char *log, const char *logdir, const char *filename)
{
	sprintf(log, "%s%c%s.log", logdir, ECHECK_SEP, filename);
	return log;
}

int err_contains(const char *filename, char *expected[], size_t str_count)
{
	FILE *log;
	char buffer[4096];
	size_t i, len;
	unsigned int msg_found;

	log = fopen(filename, "r");
	msg_found = 0;
	if (log) {
		len = 0;
		while (fgets(buffer + len, (sizeof buffer) - len, log)) {
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
		fclose(log);
	}
	return str_count - msg_found;
}

int test_check_str(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;

	strs[0] = "same";
	strs[1] = "different";

	failures += check_str("same", "same");

	log = fopen(filename, "w");
	if (0 == fcheck_str(log, "same", "different")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_str\n", failures);
	}
	return failures;
}

int test_check_str_m(const char *filename)
{
	FILE *log;
	char *strs[3];
	int failures = 0;

	strs[0] = "weirdly";
	strs[1] = "different";
	strs[2] = "contextual info";

	failures += check_str_m("same", "same", "contextual info");

	log = fopen(filename, "w");
	if (0 == fcheck_str_m(log, "weirdly", "different", "contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_str_m\n", failures);
	}
	return failures;
}

int test_check_char(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;

	strs[0] = "a";
	strs[1] = "b";

	failures += check_char('a', 'a');

	log = fopen(filename, "w");
	if (0 == fcheck_char(log, 'a', 'b')) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_char\n", failures);
	}
	return failures;
}

int test_check_char_m(const char *filename)
{
	FILE *log;
	char *strs[3];
	int failures = 0;

	strs[0] = "a";
	strs[1] = "b";
	strs[2] = "contextual info";

	failures += check_char_m('a', 'a', "contextual info");

	log = fopen(filename, "w");
	if (0 == fcheck_char_m(log, 'a', 'b', "contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_char_m\n", failures);
	}
	return failures;
}

int test_check_int(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "-5";

	failures += check_int(5, 5);

	log = fopen(filename, "w");
	if (0 == fcheck_int(log, 5, -5)) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_int\n", failures);
	}
	return failures;
}

int test_check_int_m(const char *filename)
{
	FILE *log;
	char *strs[3];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "-5";
	strs[2] = "contextual info";

	failures += check_int_m(5, 5, "contextual info");

	log = fopen(filename, "w");
	if (0 == fcheck_int_m(log, 5, -5, "contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_int_m\n", failures);
	}
	return failures;
}

int test_check_unsigned_int(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;

	strs[0] = "1";
	strs[1] = "4294967295";

	failures += check_unsigned_int(5, 5);

	log = fopen(filename, "w");
	if (0 == fcheck_unsigned_int(log, 1, -1)) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_unsigned_int\n",
			failures);
	}
	return failures;
}

int test_check_unsigned_int_m(const char *filename)
{
	FILE *log;
	char *strs[3];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "6";
	strs[2] = "contextual info";

	failures += check_unsigned_int_m(5, 5, "contextual info");

	log = fopen(filename, "w");
	if (0 == fcheck_unsigned_int_m(log, 5, 6, "contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_unsigned_int_m\n",
			failures);
	}
	return failures;
}

int test_check_long(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "-5";

	failures += check_long(5L, 5L);

	log = fopen(filename, "w");
	if (0 == fcheck_long(log, 5L, -5L)) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_long\n", failures);
	}
	return failures;
}

int test_check_long_m(const char *filename)
{
	FILE *log;
	char *strs[3];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "-5";
	strs[2] = "contextual info";

	failures += check_long_m(5L, 5L, "contextual info");

	log = fopen(filename, "w");
	if (0 == fcheck_long_m(log, 5L, -5L, "contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_long_m\n", failures);
	}
	return failures;
}

int test_check_unsigned_long(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;

	strs[0] = "1";
	strs[1] = sizeof(size_t) == 8 ? "18446744073709551615" : "4294967295";

	failures += check_unsigned_long(5L, 5L);

	log = fopen(filename, "w");
	if (0 == fcheck_unsigned_long(log, 1L, -1L)) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_unsigned_long\n",
			failures);
	}
	return failures;
}

int test_check_unsigned_long_m(const char *filename)
{
	FILE *log;
	char *strs[3];
	int failures = 0;

	strs[0] = "5";
	strs[1] = "6";
	strs[2] = "contextual info";

	failures += check_unsigned_long_m(5L, 5L, "contextual info");

	log = fopen(filename, "w");
	if (0 == fcheck_unsigned_long_m(log, 5L, 6L, "contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_unsigned_long_m\n",
			failures);
	}
	return failures;
}

int test_check_size_t(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;
	size_t one = 1;
	size_t big = -1;
	size_t five = 5;

	strs[0] = "1";
	strs[1] = sizeof(size_t) == 8 ? "18446744073709551615" : "4294967295";

	failures += check_size_t(five, five);

	log = fopen(filename, "w");
	if (0 == fcheck_size_t(log, one, big)) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_size_t\n", failures);
	}
	return failures;
}

int test_check_size_t_m(const char *filename)
{
	FILE *log;
	char *strs[3];
	int failures = 0;
	size_t five = 5;
	size_t six = 6;

	strs[0] = "5";
	strs[1] = "6";
	strs[2] = "contextual info";

	failures += check_size_t_m(five, five, "contextual info");

	log = fopen(filename, "w");
	if (0 == fcheck_size_t_m(log, five, six, "contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_size_t_m\n",
			failures);
	}
	return failures;
}

int test_check_byte_array(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;

	unsigned char bytes_a[2] = { 0x00, 0xFF };
	unsigned char bytes_b[2] = { 0x00, 0xFF };
	unsigned char bytes_c[2] = { 0x13, 0x10 };

	strs[0] = "";
	strs[1] = "";

	failures += check_byte_array(bytes_a, 2, bytes_b, 2);

	log = fopen(filename, "w");
	if (0 == fcheck_byte_array(log, bytes_b, 2, bytes_c, 2)) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_byte_array\n",
			failures);
	}
	return failures;
}

int test_check_byte_array_m(const char *filename)
{
	FILE *log;
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

	log = fopen(filename, "w");
	if (0 ==
	    fcheck_byte_array_m(log, bytes_b, 2, bytes_c, 2,
				"contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_byte_array_m\n",
			failures);
	}
	return failures;
}

int test_check_ptr(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;

	strs[0] = "Expected";	/* this is cheating */
	strs[1] = "but was";

	failures += check_ptr(strs[0], strs[0]);

	log = fopen(filename, "w");
	if (0 == fcheck_ptr(log, strs[0], strs[1])) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_ptr\n", failures);
	}
	return failures;
}

int test_check_ptr_m(const char *filename)
{
	FILE *log;
	char *strs[3];
	int failures = 0;

	strs[0] = "contextual info";
	strs[1] = "foo";

	failures += check_ptr_m(strs[0], strs[0], "contextual info");

	log = fopen(filename, "w");
	if (0 == fcheck_ptr_m(log, strs[0], strs[1], "contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 1);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_ptr_m\n", failures);
	}
	return failures;
}

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	if (argc > 1) {
		dir = argv[1];
	} else {
		dir = "/tmp";
	}

	set_log(log, dir, "test_check_str");
	failures += test_check_str(log);

	set_log(log, dir, "test_check_str_m");
	failures += test_check_str_m(log);

	set_log(log, dir, "test_check_char");
	failures += test_check_char(log);

	set_log(log, dir, "test_check_char_m");
	failures += test_check_char_m(log);

	set_log(log, dir, "test_check_int");
	failures += test_check_int(log);

	set_log(log, dir, "test_check_int_m");
	failures += test_check_int_m(log);

	set_log(log, dir, "test_check_unsigned_int");
	failures += test_check_unsigned_int(log);

	set_log(log, dir, "test_check_unsigned_int_m");
	failures += test_check_unsigned_int_m(log);

	set_log(log, dir, "test_check_long");
	failures += test_check_long(log);

	set_log(log, dir, "test_check_long_m");
	failures += test_check_long_m(log);

	set_log(log, dir, "test_check_unsigned_long");
	failures += test_check_unsigned_long(log);

	set_log(log, dir, "test_check_unsigned_long_m");
	failures += test_check_unsigned_long_m(log);

	set_log(log, dir, "test_check_size_t");
	failures += test_check_size_t(log);

	set_log(log, dir, "test_check_size_t_m");
	failures += test_check_size_t_m(log);

	set_log(log, dir, "test_check_ptr");
	failures += test_check_ptr(log);

	set_log(log, dir, "test_check_ptr_m");
	failures += test_check_ptr_m(log);

	set_log(log, dir, "test_check_byte_array");
	failures += test_check_byte_array(log);

	set_log(log, dir, "test_check_byte_array_m");
	failures += test_check_byte_array_m(log);

	if (failures) {
		fprintf(stderr, "%d failures in total\n", failures);
	}
	return failures;
}
