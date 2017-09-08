#include <stdio.h>
#include <string.h>

#include "../src/echeck.h"
#include "test-echeck-private-utils.h"

int test_check_ptr_m(const char *filename)
{
	FILE *log;
	const char *strs[3];
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

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_ptr_m");

	failures += test_check_ptr_m(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
