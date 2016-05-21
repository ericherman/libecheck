#include <stdio.h>
#include <string.h>

#include "echeck.h"
#include "test-echeck-private-utils.h"

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

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_unsigned_int_m");

	failures += test_check_unsigned_int_m(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return failures;
}
