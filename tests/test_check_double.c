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

int test_check_double(const char *filename)
{
	FILE *log;
	const char *strs[4];
	int failures = 0;

	strs[0] = "8.8";
	strs[1] = "-8.8";
	strs[2] = "9.001";
	strs[3] = "9.002";

	failures += check_double(1.0, 1.0, 0.00001);
	failures += check_double(2.00002, 2.0, 0.001);
	failures += check_double(3.0003, 3.0004, 0.01);
	failures += check_double(-4.0001, -4.0002, 0.01);
	failures += check_double(-5.0002, -5.0001, 0.01);
	failures += check_double(0.00006, 0.0, 0.001);
	failures += check_double(0.0, 0.00007, 0.001);

	log = fopen(filename, "w");
	if (0 == fcheck_double(log, 8.8, -8.8, 0.00001)) {
		failures++;
	}
	if (0 == fcheck_double(log, 9.001, 9.002, 0.00001)) {
		failures++;
	}
	fclose(log);
	failures += err_contains(filename, strs, 2);

	failures += check_double(-0.0001, 0.0001, 0.1);
	failures += check_double(0.0002, -0.0002, 0.1);

	failures += check_double_scaled_epsilon(0.0002, 0.0002);

	if (failures) {
		fprintf(stderr, "%d failures in test_check_double\n", failures);
	}
	return failures;
}

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_double");

	failures += test_check_double(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
