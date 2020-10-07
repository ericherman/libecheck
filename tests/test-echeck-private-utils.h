/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#ifndef TEST_ECHECK_PRIVATE_UTILS_H
#define TEST_ECHECK_PRIVATE_UTILS_H

#include "echeck.h"

#ifndef ECHECK_HOSTED
#ifdef __STDC_HOSTED__
#define ECHECK_HOSTED __STDC_HOSTED__
#else
#define ECHECK_HOSTED 1
#endif
#endif

#include <stdint.h>

extern void (*echeck_test_debug_prints)(const char *s);
extern void (*echeck_test_debug_printv)(const void *v);
extern void (*echeck_test_debug_printz)(size_t z);
extern void (*echeck_test_debug_printeol)(void);

extern struct echeck_log echeck_test_buf_log;

struct echeck_log *echeck_test_log_capture(void);
void echeck_test_log_release(struct echeck_log *orig);

int echeck_test_buf_contains(const char *buf, const char *expected);
int echeck_test_err_log_contains(const char *expected[], size_t expected_len);

void echeck_test_debug_print_failures(unsigned failures, const char *where);

size_t echeck_diy_strnlen(const char *str, size_t buf_size);
char *echeck_test_u64_to_hex(char *buf, size_t len, uint64_t z);
char *echeck_test_ul_to_str(char *buf, size_t len, unsigned long ul);
char *echeck_test_bogus_float_to_str(char *buf, size_t len, double f);

#endif /* TEST_ECHECK_PRIVATE_UTILS_H */
