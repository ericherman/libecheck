/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed-assert.h */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#ifndef EEMBED_ASSERT_H
#define EEMBED_ASSERT_H

#include "eembed.h"

#ifndef EEMBED_ASSERT_NOP
#define EEMBED_ASSERT_NOP() do { ((void)0); } while (0)
#endif

#ifndef eembed_assert
#if EEMBED_HOSTED
#include <assert.h>
#define eembed_assert(expression) assert(expression)
#else
#ifdef NDEBUG
#define eembed_assert(expression) EEMBED_ASSERT_NOP()
#else
#define eembed_assert(expression) \
	do { \
		if (expression) { \
			EEMBED_ASSERT_NOP(); \
		} else { \
			char _eembed_itoa_buf[25]; \
			eembed_ulong_to_str(_eembed_itoa_buf, 25, __LINE__); \
			eembed_system_print(__FILE__); \
			eembed_system_print(":"); \
			eembed_system_print(_eembed_itoa_buf); \
			eembed_system_print(": ASSERTION assert("); \
			eembed_system_print(#expression); \
			eembed_system_print(") FAILED"); \
			eembed_system_println(); \
			eembed_assert_crash(); \
		} \
	} while (0)
#endif
#endif
#endif

#endif /* EEMBED_ASSERT_H */
