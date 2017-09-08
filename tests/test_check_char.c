#include <stdio.h>
#include <string.h>

#include "../src/echeck.h"
#include "test-echeck-private-utils.h"

int test_check_char(const char *filename)
{
	FILE *log;
	const char *strs[2];
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

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_char");

	failures += test_check_char(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
