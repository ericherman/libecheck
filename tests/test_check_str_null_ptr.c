#include <stdio.h>
#include <string.h>

#include "../src/echeck.h"
#include "test-echeck-private-utils.h"

int test_check_str_null_ptr(const char *filename)
{
	FILE *log;
	int failures = 0;
	const char *strs[2];

	strs[0] = "not_null_second";
	strs[1] = "not_null_first";

	failures += check_str(NULL, NULL);

	log = fopen(filename, "w");
	if (0 == fcheck_str(log, NULL, strs[0])) {
		failures++;
	}
	if (0 == fcheck_str(log, strs[1], NULL)) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_str\n", failures);
	}
	return failures;
}

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_str_null_ptr");

	failures += test_check_str_null_ptr(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
