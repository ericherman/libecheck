/*
libecheck: "E(asy)Check" boiler-plate to make simple testing easier
Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org>

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
#ifndef TEST_ECHECK_PRIVATE_UTILS_H
#define TEST_ECHECK_PRIVATE_UTILS_H

/* TODO: do something portable alternative here */
#define ECHECK_PATH_MAX 4096
#define ECHECK_PATH_SEP '/'

char *set_log(char *log, const char *logdir, const char *filename);

int err_contains(const char *filename, const char *expected[],
		 size_t expected_len);

#endif /* TEST_ECHECK_PRIVATE_UTILS_H */
