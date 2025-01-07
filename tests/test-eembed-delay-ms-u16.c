/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-delay-ms.c */
/* Copyright (C) 2024 Eric Herman <eric@freesa.org> */

#include "eembed.h"

void test_eembed_delay_interval(uint16_t ms)
{
	unsigned long initial_uptime = 0;
	unsigned long current_uptime = 0;
	unsigned long minimum_uptime = 0;

	initial_uptime = uptime_ms();

	delay_ms_u16(ms);

	current_uptime = uptime_ms();
	minimum_uptime = initial_uptime + ms;

	eembed_crash_if_false(current_uptime >= minimum_uptime);
}

unsigned test_eembed_delay_ms_u16(void)
{
	test_eembed_delay_interval(0);
	test_eembed_delay_interval(31);
	test_eembed_delay_interval(1000);

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_delay_ms_u16)
