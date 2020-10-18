/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2019 Eric Herman <eric@freesa.org> */

#include <Arduino.h>
#include <HardwareSerial.h>

#include "echeck-arduino.h"

struct echeck_log serial_log;

void serial_log_prints(struct echeck_log *log, const char *s)
{
	(void)log;
	Serial.print(s);
}

void serial_log_printul(struct echeck_log *log, unsigned long ul)
{
	(void)log;
	Serial.print(ul);
}

void serial_log_printz(struct echeck_log *log, size_t z)
{
	(void)log;
	Serial.print(z);
}

void serial_log_printl(struct echeck_log *log, long l)
{
	(void)log;
	Serial.print(l);
}

void serial_log_printf(struct echeck_log *log, double f)
{
	(void)log;
	Serial.print(f);
}

void serial_log_printvp(struct echeck_log *log, const void *v)
{
	(void)log;
	Serial.print((size_t)v);
}

void serial_log_printeol(struct echeck_log *log)
{
	(void)log;
	Serial.println();
}

void echeck_arduino_serial_log_init(void)
{
	serial_log.context = NULL;
	serial_log.append_s = serial_log_prints;
	serial_log.append_ul = serial_log_printul;
	serial_log.append_z = serial_log_printz;
	serial_log.append_l = serial_log_printl;
	serial_log.append_f = serial_log_printf;
	serial_log.append_vp = serial_log_printvp;
	serial_log.append_eol = serial_log_printeol;
	echeck_default_log = &serial_log;
}
