/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#ifndef TEST_ECHECK_PRIVATE_UTILS_H
#define TEST_ECHECK_PRIVATE_UTILS_H

/* TODO: do something portable alternative here */
#define ECHECK_PATH_MAX 4096
#define ECHECK_PATH_SEP '/'

char *set_log(char *log, const char *logdir, const char *filename);

int err_contains(const char *filename, const char *expected[],
		 size_t expected_len);

#endif /* TEST_ECHECK_PRIVATE_UTILS_H */
