/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* echeck_tests_arduino.ino: running the tests on an arduino */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <Arduino.h>
#include <HardwareSerial.h>

#include "echeck.h"

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

#include "test-echeck-private-utils.h"

void serial_prints(const char *s)
{
	Serial.print(s);
}

/* prototypes for the test functions, are not needed in the hosted case
 * becase these are normally called by individual programs in parallel, but
 * we need these exported because this firmware will run the tests from each
 * test compilation unit */
unsigned test_check_byte_array(void);
unsigned test_check_byte_array_m(void);
unsigned test_check_char(void);
unsigned test_check_char_m(void);
unsigned test_check_double(void);
unsigned test_check_int(void);
unsigned test_check_int_m(void);
unsigned test_check_long(void);
unsigned test_check_long_m(void);
unsigned test_check_ptr(void);
unsigned test_check_ptr_m(void);
unsigned test_check_size_t(void);
unsigned test_check_size_t_m(void);
unsigned test_check_status(void);
unsigned test_check_str(void);
unsigned test_check_str_m(void);
unsigned test_check_str_null_ptr(void);
unsigned test_check_unsigned_int(void);
unsigned test_check_unsigned_int_m(void);
unsigned test_check_unsigned_long(void);
unsigned test_check_unsigned_long_m(void);

/* setup/loop globals */
uint32_t loop_count;
uint16_t loop_delay_ms = (2 * 1000);
struct echeck_log serial_log;

void setup(void)
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

	echeck_test_debug_prints = serial_prints;

	Serial.begin(9600);

	delay(50);

	Serial.println();
	Serial.println();
	Serial.println("==================================================");
	Serial.println(__FILE__);
	Serial.println(__LINE__);
	Serial.println(__func__);
	Serial.println();
	Serial.print("ARDUINO == ");
	Serial.println(ARDUINO);
	Serial.println();
	Serial.print("__STDC_HOSTED__ == ");
	Serial.println(__STDC_HOSTED__);
	Serial.println();
	Serial.print("ECHECK_HOSTED == ");
	Serial.println(ECHECK_HOSTED);
	Serial.println();
	Serial.println("Begin");

	loop_count = 0;
}

void loop(void)
{
	++loop_count;
	Serial.println("==================================================");
	Serial.print("Starting test run #");
	Serial.println(loop_count);
	Serial.println("==================================================");

	unsigned failures = 0;

	Serial.println("test_check_byte_array");
	failures += test_check_byte_array();

	Serial.println("test_check_byte_array_m");
	failures += test_check_byte_array_m();

	Serial.println("test_check_char");
	failures += test_check_char();

	Serial.println("test_check_char_m");
	failures += test_check_char_m();

	Serial.println("test_check_double");
	failures += test_check_double();

	Serial.println("test_check_int");
	failures += test_check_int();

	Serial.println("test_check_int_m");
	failures += test_check_int_m();

	Serial.println("test_check_long");
	failures += test_check_long();

	Serial.println("test_check_long_m");
	failures += test_check_long_m();

	Serial.println("test_check_ptr");
	failures += test_check_ptr();

	Serial.println("test_check_ptr_m");
	failures += test_check_ptr_m();

	Serial.println("test_check_size_t");
	failures += test_check_size_t();

	Serial.println("test_check_size_t_m");
	failures += test_check_size_t_m();

	Serial.println("test_check_status");
	failures += test_check_status();

	Serial.println("test_check_str");
	failures += test_check_str();

	Serial.println("test_check_str_m");
	failures += test_check_str_m();

	Serial.println("test_check_str_null_ptr");
	failures += test_check_str_null_ptr();

	Serial.println("test_check_unsigned_int");
	failures += test_check_unsigned_int();

	Serial.println("test_check_unsigned_int_m");
	failures += test_check_unsigned_int_m();

	Serial.println("test_check_unsigned_long");
	failures += test_check_unsigned_long();

	Serial.println("test_check_unsigned_long_m");
	failures += test_check_unsigned_long_m();

	Serial.println();
	if (failures == 1) {
		Serial.println("There was 1 failure.");
		Serial.println("FAIL");
	} else if (failures != 0) {
		Serial.print("There were ");
		Serial.print(failures);
		Serial.println(" failures.");
		Serial.println("FAIL");
	} else {
		Serial.println("SUCCESS!");
	}
	Serial.println("==================================================");

	for (int i = 0; i < 50; ++i) {
		Serial.print(".");
		uint16_t delay_ms = loop_delay_ms / 50;
		if (failures) {
			delay_ms *= 10;
		}
		delay(delay_ms);
	}

	Serial.println();
}
