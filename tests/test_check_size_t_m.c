/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include <stdio.h>
#include <string.h>

#include "../src/echeck.h"
#include "test-echeck-private-utils.h"

int test_check_size_t_m(const char *filename)
{
	FILE *log;
	const char *strs[3];
	int failures = 0;
	size_t five = 5;
	size_t six = 6;

	strs[0] = "5";
	strs[1] = "6";
	strs[2] = "contextual info";

	failures += check_size_t_m(five, five, "contextual info");

	log = fopen(filename, "w");
	if (0 == fcheck_size_t_m(log, five, six, "contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_size_t_m\n",
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
	set_log(log, dir, "test_check_size_t_m");

	failures += test_check_size_t_m(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
