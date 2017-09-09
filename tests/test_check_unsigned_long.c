/*
libecheck: "E(asy)Check" boiler-plate to make simple testing easier
Copyright (C) 2016, 2017 Eric Herman <eric@freesa.org>

This work is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This work is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

	https://www.gnu.org/licenses/lgpl-3.0.txt
	https://www.gnu.org/licenses/gpl-3.0.txt
*/
#include <stdio.h>
#include <string.h>

#include "../src/echeck.h"
#include "test-echeck-private-utils.h"

int test_check_unsigned_long(const char *filename)
{
	FILE *log;
	const char *strs[2];
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

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_unsigned_long");

	failures += test_check_unsigned_long(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
