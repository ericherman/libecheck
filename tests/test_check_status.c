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

int test_check_status(const char *filename)
{
	int i, failures;
	FILE *log;
	char actual, expected;
	char buf[40];
	const char *expected_strs[1];

	failures = 0;

	for (i = -128; i < 128; ++i) {
		sprintf(buf, "in:%d expected:%c", i, (char)i);
		actual = check_status(i);
		failures += check_int_m(actual, i, buf);
		if (i == 0) {
			failures +=
			    check_int_m(actual, EXIT_SUCCESS, "EXIT_SUCCESS");
		}
	}

	expected_strs[0] = buf;

	expected = 127;
	for (i = expected; i < 260; ++i) {
		log = fopen(filename, "w");
		actual = fcheck_status(log, i);
		fclose(log);
		sprintf(buf, "%d", i);
		if (i != expected) {
			failures += err_contains(filename, expected_strs, 1);
		}
		failures += check_int_m(actual, expected, buf);
	}

	expected = -128;
	for (i = expected; i < -260; --i) {
		log = fopen(filename, "w");
		actual = fcheck_status(log, i);
		fclose(log);
		sprintf(buf, "%d", i);
		if (i != expected) {
			failures += err_contains(filename, expected_strs, 1);
		}
		failures += check_int_m(actual, expected, buf);
	}

	if (failures) {
		fprintf(stderr, "%d failures in test_check_status\n", failures);
	}
	return failures;
}

int main(int argc, char *argv[])
{
	const char *dir;
	char log[ECHECK_PATH_MAX + 1];
	int failures = 0;

	dir = (argc > 1) ? argv[1] : NULL;
	set_log(log, dir, "test_check_status");

	failures += test_check_status(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}

	return ((failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
