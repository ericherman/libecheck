/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* echeck_arduino_demo.ino: using EasyCheck in arduino firmware */
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

	Serial.begin(9600);

	delay(50);

	Serial.println();
	Serial.println();
	Serial.println("==================================================");
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
	unsigned expected_failures = 0;
	Serial.println("check_int");
	delay_print_separator(0, '-', 50);
	Serial.println("checking that two integers match");
	Serial.println("int val = 13;");
	int val = 13;
	Serial.println("int expected = 42;");
	int expected = 42;
	Serial.println("expected_failures += check_int(val, expected);");
	expected_failures += check_int(val, expected);
	Serial.println();
	delay_print_separator(loop_delay_ms, '-', 50);
	Serial.println("check_int again with a message string:");
	Serial.println
	    ("expected_failures += check_int_m(val, expected, \"msg 1\");");
	expected_failures += check_int_m(val, expected, "msg 1");
	delay_print_separator(loop_delay_ms, '-', 50);
	Serial.println("check_int is silent when the parameters match:");
	Serial.println("val = expected;");
	val = expected;
	Serial.println("failures += check_int_m(val, expected, \"msg 2\");");
	failures += check_int_m(val, expected, "msg 2");
	delay_print_separator(loop_delay_ms, '-', 50);
	Serial.println("All check_ functions return 0 on success, 1 on error");
	Serial.print("expected_failures == ");
	Serial.println(expected_failures);
	Serial.print("failures == ");
	Serial.println(failures);
	Serial.println();

	delay_print_separator(loop_delay_ms, '-', 50);

	Serial.println("check_str");
	delay_print_separator(0, '-', 50);
	Serial.println("const char *actual = NULL;");
	const char *actual = NULL;
	Serial.println("check_str_m(actual, \"expect me 1\", \"msg 3\")");
	check_str_m(actual, "expect me 1", "msg 3");
	delay_print_separator(loop_delay_ms, '-', 50);
	Serial.println();
	Serial.println("check_str(actual, \"expect me 2\")");
	check_str(actual, "expect me 2");
	delay_print_separator(loop_delay_ms, '-', 50);
	Serial.println();
	Serial.println("actual = \"bar\";");
	actual = "bar";
	Serial.println("check_str(actual, \"expect me 3\")");
	check_str("actual", "expect me 3");
	Serial.println();

	delay_print_separator(loop_delay_ms, '-', 50);

	Serial.println("check_char");
	delay_print_separator(0, '-', 50);
	Serial.println("char mychar = 'a';");
	char mychar = 'a';
	Serial.println("check_char_m(mychar, 'e', \"msg 4\");");
	expected_failures += check_char_m(mychar, 'e', "msg 4");
	Serial.println("check_char(mychar, 'e');");
	expected_failures += check_char(mychar, 'e');
	Serial.println();

	delay_print_separator(loop_delay_ms, '-', 50);

	Serial.println("check_byte_array");
	delay_print_separator(0, '-', 50);
	Serial.println("unsigned char bytes_a[] = { 0x01, 0x02, 0x03, };");
	Serial.println("size_t alen = 3;");
	Serial.println("unsigned char bytes_b[] = { 0x11, 0x02, 0x7F, };");
	Serial.println("size_t blen = 3;");
	Serial.println
	    ("check_byte_array_m(bytes_a, alen, bytes_b, blen, \"msg 5\");");
	unsigned char bytes_a[] = { 0x1F, 0x2B, 0x30, };
	size_t alen = 3;
	unsigned char bytes_b[] = { 0x1F, 0x2B, 0x7F, };
	size_t blen = 3;
	check_byte_array_m(bytes_a, alen, bytes_b, blen, "msg 5");
	Serial.println();

	delay_print_separator(loop_delay_ms, '-', 50);

	Serial.println("check_ptr");
	delay_print_separator(0, '-', 50);
	Serial.println("check_ptr_m(bytes_a, bytes_b, \"msg 6\");");
	check_ptr_m(bytes_a, bytes_b, "msg 6");
	Serial.println();

	Serial.println();
	delay_print_separator(loop_delay_ms, '.', 50);
	Serial.println();
}

void delay_print_separator(uint16_t delay_ms, char c, size_t chars)
{
	for (size_t i = 0; i < chars; ++i) {
		Serial.print(c);
		uint16_t delay_ms_fraction = delay_ms / chars;
		delay(delay_ms_fraction);
	}
	Serial.println();
}
