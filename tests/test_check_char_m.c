#include <stdio.h>
#include <string.h>

#include "../src/echeck.h"
#include "test-echeck-private-utils.h"

int test_check_char_m(const char *filename)
{
	FILE *log;
	const char *strs[3];
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

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_char_m");

	failures += test_check_char_m(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
