#include <stdio.h>
#include <string.h>

#include "echeck.h"
#include "test-echeck-private-utils.h"

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

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_int");

	failures += test_check_int(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return failures;
}
