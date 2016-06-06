#include <stdio.h>
#include <string.h>

#include "../src/echeck.h"
#include "test-echeck-private-utils.h"

int test_check_unsigned_int(const char *filename)
{
	FILE *log;
	char *strs[2];
	int failures = 0;
	char buf[40];

	strs[0] = "1";
	sprintf(buf, "%lu", (unsigned long)-1);
	strs[1] = buf;

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

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_unsigned_int");

	failures += test_check_unsigned_int(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
