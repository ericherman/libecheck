/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2016, 2017, 2018, 2019 Eric Herman <eric@freesa.org> */

#ifndef TEST_ECHECK_PRIVATE_UTILS_H
#define TEST_ECHECK_PRIVATE_UTILS_H

#include "echeck.h"

extern struct eembed_log echeck_test_buf_log;

struct eembed_log *echeck_test_log_capture(void);
void echeck_test_log_release(struct eembed_log *orig);

int echeck_test_buf_contains(const char *buf, const char *expected);
int echeck_test_err_log_contains(const char *expected[], size_t expected_len);

void echeck_test_debug_print_failures(unsigned failures, const char *where);

#if FAUX_FREESTANDING
void eembed_faux_freestanding_system_print(const char *str);
#define eembed_test_init() \
	eembed_system_print = eembed_faux_freestanding_system_print
#else
#define eembed_test_init() EEMBED_NOP()
#endif

#ifndef ECHECK_TEST_MAIN
#if (!EEMBED_HOSTED && !FAUX_FREESTANDING)
#define ECHECK_TEST_MAIN(pfunc, filename)	/* skip */
#else
#define ECHECK_TEST_MAIN(pfunc, filename) \
int main(void) \
{ \
	unsigned failures = 0; \
	eembed_test_init(); \
	failures += pfunc(); \
	if (failures) { \
		echeck_test_debug_print_failures(failures, filename); \
	} \
	return check_status(failures); \
}
#endif
#endif /* ECHECK_TEST_MAIN */

#endif /* TEST_ECHECK_PRIVATE_UTILS_H */
