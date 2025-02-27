/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* echeck_tests_arduino.ino: running the tests on an arduino */
/* libecheck: "E(asy)Check" boiler-plate to make simple testing easier */
/* Copyright (C) 2020-2025 Eric Herman <eric@freesa.org> */

#include <Arduino.h>
#include <HardwareSerial.h>

#include "eembed.h"

/* prototypes for the test functions, are not needed in the hosted case
 * becase these are normally called by individual programs in parallel, but
 * we need these exported because this firmware will run the tests from each
 * test compilation unit */
unsigned test_eembed_log(void);
unsigned test_eembed_assert(void);
unsigned test_eembed_print(void);
unsigned test_eembed_delay_ms_u16(void);
unsigned test_eembed_long_to_str(void);
unsigned test_eembed_ulong_to_str(void);
unsigned test_eembed_ulong_to_hex(void);
unsigned test_eembed_float_to_str(void);
unsigned test_eembed_str_to_num(void);
unsigned test_eembed_memcmp(void);
unsigned test_eembed_memcpy(void);
unsigned test_eembed_memmove(void);
unsigned test_eembed_memset(void);
unsigned test_eembed_strcat(void);
unsigned test_eembed_strcpy(void);
unsigned test_eembed_strlen(void);
unsigned test_eembed_strstr(void);
unsigned test_eembed_malloc_free(void);
unsigned test_eembed_chunk_alloc(void);
unsigned test_eembed_chunk_realloc(void);
unsigned test_eembed_random_bytes(void);
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
	Serial.println();
	Serial.println("==================================================");
	Serial.println("Starting test run");
	Serial.println("==================================================");

	unsigned failures = 0;

	failures += Run_test(test_eembed_log);
	failures += Run_test(test_eembed_assert);
	failures += Run_test(test_eembed_print);
	failures += Run_test(test_eembed_delay_ms_u16);
	failures += Run_test(test_eembed_long_to_str);
	failures += Run_test(test_eembed_ulong_to_str);
	failures += Run_test(test_eembed_ulong_to_hex);
	failures += Run_test(test_eembed_float_to_str);
	failures += Run_test(test_eembed_str_to_num);
	failures += Run_test(test_eembed_memcmp);
	failures += Run_test(test_eembed_memcpy);
	failures += Run_test(test_eembed_memmove);
	failures += Run_test(test_eembed_memset);
	failures += Run_test(test_eembed_strcat);
	failures += Run_test(test_eembed_strcpy);
	failures += Run_test(test_eembed_strlen);
	failures += Run_test(test_eembed_strstr);
	failures += Run_test(test_eembed_malloc_free);
	failures += Run_test(test_eembed_chunk_alloc);
	failures += Run_test(test_eembed_chunk_realloc);
	failures += Run_test(test_eembed_random_bytes);

	Serial.println("==================================================");
	Serial.println("Ending test run");
	Serial.println("==================================================");

	if (failures == 1) {
		Serial.println("There was 1 failure.");
		Serial.println("FAIL");
	} else if (failures != 0) {
		Serial.print("There were ");
		Serial.print(failures);
		Serial.println(" failures.");
		Serial.println("FAIL");
	} else {
		eembed_assert(1);
		Serial.println("SUCCESS!");
	}

	Serial.println("==================================================");

	Serial.println();
	uint16_t loop_delay_ms = 3 * 1000;
	size_t dots = 50;
	for (size_t i = 0; i < dots; ++i) {
		Serial.print(".");
		uint16_t delay_ms = loop_delay_ms / dots;
		if (failures) {
			delay_ms *= 10;
		}
		delay(delay_ms);
	}

	Serial.println();
	Serial.println();
	Serial.println("TESTING eembed_assert(false), EXPECT A CRASH!");
	for (size_t i = 5; i; --i) {
		Serial.println(i);
		delay(1000);
	}
	eembed_assert(1 == 0);

	Serial.println("Wait ... how did we get here?");
	Serial.println("The 'eembed_assert(1 == 0)' failed to crash?");
	Serial.println("THIS IS BAD!");
	delay(10 * 1000);
}
