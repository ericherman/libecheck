/* SPDX-License-Identifier: LGPL-2.1-or-later */
/* eembed_alloc_arduino_demo.ino : showing eembed-alloc in arduino */
/* Copyright (C) 2020-2025 Eric Herman */
/* https://github.com/ericherman/eembed-alloc */
/* https://www.gnu.org/licenses/old-licenses/lgpl-2.1.txt */

#include <Arduino.h>
#include <HardwareSerial.h>

#include "eembed-arduino.h"

// Allocate the buffer used by eembed_alloc
const size_t global_buffer_len = 512;
unsigned char global_buffer[global_buffer_len];

// the buffers[] array will be where we stash allocated memory
const size_t buffers_len = 10;
char *buffers[buffers_len];

// we will allocate buffers of no more than 70 characters in order to
// display nicely on an 80 column screen. However 70 * 10 is obvioulsy
// larger than 512, thus we can not possibly allocate all of the
// buffers at max length; we should expect some allocation requests
// to be denied by eembed_malloc()
const size_t max_str_len = 70;

uint16_t loop_delay_ms = (2 * 1000);
uint32_t loops_completed;

void setup(void)
{
	Serial.begin(9600);
	// wait to ensure stable Serial initialziaiton
	delay(50);

	eembed_arduino_serial_log_init();

	// struct initialization includes using some of the buffer
	// space for internal data structures; the usable space will
	// be slightly less than this
	struct eembed_allocator *ea =
	    eembed_bytes_allocator(global_buffer, global_buffer_len);
	if (ea) {
		eembed_global_allocator = ea;
	}

	loops_completed = 0;

	Serial.println();
	Serial.println("==================================================");
	Serial.println();
	Serial.println("Begin");
	Serial.println();
	Serial.println("==================================================");
}

void loop(void)
{
	size_t idx = loops_completed % buffers_len;
	Serial.println();
	Serial.println();
	Serial.println("==================================================");
	Serial.print(" Starting run number ");
	Serial.print(loops_completed);
	Serial.print(" using b[");
	Serial.print(idx);
	Serial.println("]");

	// if there is a pre-existing string at this index,
	// free the old string
	if (buffers[idx]) {
		size_t in_use = eembed_strlen(buffers[idx]) + 1;
		Serial.print("freeing ");
		Serial.print(in_use);
		Serial.print(" bytes ...");

		eembed_free(buffers[idx]);
		buffers[idx] = NULL;

		Serial.println(" done.");
	}

	Serial.println("==================================================");
	if (eembed_global_allocator && eembed_global_allocator->context) {
		if (0) {
			eembed_bytes_allocator_dump(eembed_err_log,
						    eembed_global_allocator);
		}
		if (1) {
			Serial.println();
			Serial.println("A: allocator overhead");
			Serial.println("O: chunk overhead");
			Serial.println(".: free memory");
			Serial.println("_: used memory");
			Serial.println("memory layout:");
			int stringify_contents = 1;
			size_t width = 64;
			eembed_bytes_allocator_visual(eembed_err_log,
						      eembed_global_allocator,
						      stringify_contents,
						      width);
		}
		Serial.println
		    ("==================================================");
	}
	// choose an arbitrary size ... after several loops, we can
	// expect some pretty fragmented memory
	size_t buflen = (millis() % max_str_len);
	if (buflen < 2) {
		buflen = 2;
	}
	Serial.print("requesting allocation of ");
	Serial.print(buflen);
	Serial.println(" bytes ....");

	// Make space for a new buffer by calling eembed_malloc()
	// however, since the total global buffer size is less than
	// the total size of all the strings if they are maxium
	// length, the call to malloc may fail:
	char *buf = (char *)eembed_malloc(buflen);
	if (!buf) {
		Serial.println();
		Serial.print("\tCOULD NOT ALLOCATE ");
		Serial.print(buflen);
		Serial.println(" BYTES!");
	} else {
		Serial.print("\tallocated ");
		Serial.print(buflen);
		Serial.println(" bytes.");

		// we will fill our allocated buffers with a string
		// of a single character repeating, this will help
		// give us some confidence that the data is not
		// getting over-written by other allocations
		char c = char_for(loops_completed);
		for (size_t i = 0; i < buflen; ++i) {
			buf[i] = c;
		}
		buf[buflen - 1] = '\0';
		buffers[idx] = buf;
	}

	size_t bytes_used = 0;
	size_t buffers_used = 0;
	Serial.println();
	Serial.println("Currently allocated buffers: ");
	// print all the strings
	for (size_t i = 0; i < buffers_len; ++i) {
		Serial.print(" b[");
		Serial.print(i);
		Serial.print("] = ");
		if (buffers[i]) {
			bytes_used += (1 + eembed_strlen(buffers[i]));
			Serial.println(buffers[i]);
			++buffers_used;
		} else {
			Serial.println("(null)");
		}
	}

	Serial.println();

	Serial.print(bytes_used);
	Serial.print(" bytes allocated across ");
	Serial.print(buffers_used);
	Serial.println(" active allocations.");

	Serial.println();
	for (size_t i = 0; i < 50; ++i) {
		Serial.print("=");
		delay(loop_delay_ms / 50);
	}
	Serial.println();

	Serial.println("some pseudo-random bytes:");
	for (size_t i = 0; i < 10; ++i) {
		const size_t bin_buf_size = 16;
		unsigned char bin_buf[bin_buf_size];
		const size_t hex_buf_size = (2 + (2 * bin_buf_size) + 1);
		char hex_buf[hex_buf_size];
		eembed_random_bytes(bin_buf, bin_buf_size);
		eembed_bytes_to_hex(hex_buf, hex_buf_size, bin_buf,
				    bin_buf_size);
		Serial.println(hex_buf);
	}
	Serial.println();

	Serial.println();
	for (size_t i = 0; i < 50; ++i) {
		Serial.print("=");
		delay(loop_delay_ms / 50);
	}
	Serial.println();

	++loops_completed;
}

char char_for(size_t n)
{
	return 'a' + (n % 26);
}
