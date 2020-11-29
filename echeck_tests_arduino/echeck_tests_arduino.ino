/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* echeck_tests_arduino.ino: running the tests on an arduino */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <Arduino.h>
#include <HardwareSerial.h>

#include "eembed-arduino.h"

#include "test-echeck-private-utils.h"

/* prototypes for the test functions, are not needed in the hosted case
 * becase these are normally called by individual programs in parallel, but
 * we need these exported because this firmware will run the tests from each
 * test compilation unit */
unsigned test_eembed_log(void);
unsigned test_eembed_long_to_str(void);
unsigned test_eembed_ulong_to_str(void);
unsigned test_eembed_ulong_to_hex(void);
unsigned test_eembed_memcmp(void);

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
unsigned test_check_ptr_not_null(void);
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

void setup(void)
{
	eembed_arduino_serial_log_init();

	Serial.begin(9600);

	delay(50);

	Serial.println();
	Serial.println();
	Serial.println("==================================================");
	Serial.print("__FILE__ == ");
	Serial.println(__FILE__);
	Serial.print("__LINE__ == ");
	Serial.println(__LINE__);
	Serial.print("__func__ == ");
	Serial.println(__func__);
	Serial.println();
	Serial.print("ARDUINO == ");
	Serial.println(ARDUINO);
	Serial.println();
	Serial.print("__STDC_HOSTED__ == ");
	Serial.println(__STDC_HOSTED__);
	Serial.println();
	Serial.print("EEMBED_HOSTED == ");
	Serial.println(EEMBED_HOSTED);
	Serial.println();
	Serial.println("Begin");

	loop_count = 0;
}

unsigned run_test(unsigned (*pfunc)(void), const char *name)
{
	unsigned failures = 0;
	Serial.print(name);
	Serial.print(" ... ");
	failures += pfunc();
	Serial.println(failures ? "FAIL!" : "ok");
	return failures;
}

#define Run_test(func) run_test(func, #func)

void loop(void)
{
	++loop_count;
	eembed_assert(loop_count < 5);

	Serial.println("==================================================");
	Serial.print("Starting test run #");
	Serial.println(loop_count);
	Serial.println("==================================================");

	unsigned failures = 0;

	failures += Run_test(test_eembed_log);
	failures += Run_test(test_eembed_long_to_str);
	failures += Run_test(test_eembed_ulong_to_str);
	failures += Run_test(test_eembed_ulong_to_hex);
	failures += Run_test(test_eembed_memcmp);

	Serial.println();
	failures += Run_test(test_check_byte_array);
	failures += Run_test(test_check_byte_array_m);
	failures += Run_test(test_check_char);
	failures += Run_test(test_check_char_m);
	failures += Run_test(test_check_double);
	failures += Run_test(test_check_int);
	failures += Run_test(test_check_int_m);
	failures += Run_test(test_check_long);
	failures += Run_test(test_check_long_m);
	failures += Run_test(test_check_ptr);
	failures += Run_test(test_check_ptr_m);
	failures += Run_test(test_check_ptr_not_null);
	failures += Run_test(test_check_size_t);
	failures += Run_test(test_check_size_t_m);
	failures += Run_test(test_check_status);
	failures += Run_test(test_check_str);
	failures += Run_test(test_check_str_m);
	failures += Run_test(test_check_str_null_ptr);
	failures += Run_test(test_check_unsigned_int);
	failures += Run_test(test_check_unsigned_int_m);
	failures += Run_test(test_check_unsigned_long);
	failures += Run_test(test_check_unsigned_long_m);

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
