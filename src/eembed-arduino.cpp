/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2019-2025 Eric Herman <eric@freesa.org> */

#include "eembed.h"

unsigned long eembed_die_delay = 5 * 1000;

void (*call_null_pointer_to_crash_and_reboot)(void) = NULL;
void die(void)
{
	Serial.println("EXIT FAILURE");
	Serial.print("(reboot in ");
	Serial.print(eembed_die_delay);
	Serial.println(" ms)");
	delay(eembed_die_delay);
	Serial.println();
	call_null_pointer_to_crash_and_reboot();
}

struct eembed_log serial_log;

void serial_log_print_c(struct eembed_log *log, char c)
{
	(void)log;
	Serial.print(c);
}

void serial_log_print_s(struct eembed_log *log, const char *s)
{
	(void)log;
	Serial.print(s);
}

void serial_log_print_ul(struct eembed_log *log, uint64_t ul)
{
	(void)log;
	Serial.print((unsigned long)ul);
}

void serial_log_print_l(struct eembed_log *log, int64_t l)
{
	(void)log;
	Serial.print((long)l);
}

void serial_log_print_f(struct eembed_log *log, long double f)
{
	(void)log;
	Serial.print((double)f);
}

void serial_log_print_vp(struct eembed_log *log, const void *v)
{
	(void)log;
	Serial.print((size_t)v);
}

void serial_log_print_eol(struct eembed_log *log)
{
	(void)log;
	Serial.println();
}

void eembed_arduino_serial_log_init(void)
{
	serial_log.context = NULL;
	serial_log.append_c = serial_log_print_c;
	serial_log.append_s = serial_log_print_s;
	serial_log.append_ul = serial_log_print_ul;
	serial_log.append_l = serial_log_print_l;
	serial_log.append_f = serial_log_print_f;
	serial_log.append_vp = serial_log_print_vp;
	serial_log.append_eol = serial_log_print_eol;
	eembed_err_log = &serial_log;
	eembed_out_log = &serial_log;

	eembed_assert_crash = die;
}
