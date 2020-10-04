/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#ifndef TEST_ECHECK_PRIVATE_UTILS_H
#define TEST_ECHECK_PRIVATE_UTILS_H

#include "echeck.h"

extern struct echeck_log echeck_test_buf_log;

struct echeck_log *echeck_test_log_capture(void);
void echeck_test_log_release(struct echeck_log *orig);

int err_contains(const char *expected[], size_t expected_len);

#endif /* TEST_ECHECK_PRIVATE_UTILS_H */
