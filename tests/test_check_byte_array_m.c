/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#include <stdio.h>
#include <string.h>

#include "../src/echeck.h"
#include "test-echeck-private-utils.h"

int test_check_byte_array_m(const char *filename)
{
	FILE *log;
	const char *strs[3];
	int failures = 0;

	unsigned char bytes_a[2] = { 0x00, 0xFF };
	unsigned char bytes_b[2] = { 0x00, 0xFF };
	unsigned char bytes_c[2] = { 0x13, 0x10 };

	strs[0] = "";
	strs[1] = "";
	strs[2] = "contextual info";

	failures +=
	    check_byte_array_m(bytes_a, 2, bytes_b, 2, "contextual info");

	log = fopen(filename, "w");
	if (0 ==
	    fcheck_byte_array_m(log, bytes_b, 2, bytes_c, 2,
				"contextual info")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 3);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_byte_array_m\n",
			failures);
	}
	return failures;
}

int test_check_byte_array_m_2(const char *filename)
{
	FILE *log;
	const char *strs[2];
	int failures = 0;

	unsigned char bytes_a[3] = { 0x13, 0x10, 0x00 };
	unsigned char bytes_b[4] = { 0x13, 0x10, 0x00, 0x7F };

	strs[0] = "length mis-match";
	strs[1] = "contextual";

	log = fopen(filename, "w");
	if (0 == fcheck_byte_array_m(log, bytes_a, 3, bytes_b, 4, "contextual")) {
		failures++;
	}
	fclose(log);

	failures += err_contains(filename, strs, 2);
	if (failures) {
		fprintf(stderr, "%d failures in test_check_byte_array_m\n",
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
	set_log(log, dir, "test_check_byte_array_m");

	failures += test_check_byte_array_m(log);
	failures += test_check_byte_array_m_2(log);

	if (failures) {
		fprintf(stderr, "%d failures in %s\n", failures, __FILE__);
	}
	return check_status(failures);
}
