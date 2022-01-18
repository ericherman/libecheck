/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

int test_out_of_memory_inner(uint32_t malloc_fail_bitmask)
{
	const size_t bigger_than_ram = SIZE_MAX / 8;
	struct eembed_allocator with_errs;
	struct echeck_err_injecting_context mctx;
	struct eembed_allocator *real = NULL;
	struct eembed_allocator *orig = eembed_global_allocator;
	size_t i = 0;
	size_t j = 0;
	char *message = NULL;
	char *messages[40];
	const size_t bytes_len = 500 * sizeof(size_t);
	unsigned char bytes[500 * sizeof(size_t)];
	const size_t test_object_size = 183;

	unsigned failures = 0;

	eembed_memset(messages, 0x00, 40 * sizeof(char *));

	if (eembed_global_allocator == NULL) {
		real = eembed_bytes_allocator(bytes, bytes_len);
	} else {
		real = eembed_global_allocator;
	}

	failures +=
	    echeck_err_injecting_allocator_init(&with_errs, real, &mctx,
						eembed_err_log);
	if (failures) {
		return 1;
	}
	eembed_global_allocator = &with_errs;

	mctx.attempts_to_fail_bitmask = malloc_fail_bitmask;

	for (i = 0; i < 10; ++i) {
		switch (i % 3) {
		case 1:
			message =
			    (char *)eembed_realloc(NULL, test_object_size);
			break;
		case 2:
			message =
			    (char *)eembed_reallocarray(NULL, 1,
							test_object_size);
			break;
		default:
			message = (char *)eembed_malloc(test_object_size);
		}

		if (message) {
			eembed_memset(message, ('A' + i), test_object_size);
			message[10] = '\0';
			messages[i] = message;
		}
	}
	for (i = 10; i > 2; i -= 2) {
		j = i - 1;
		message = messages[j];
		eembed_free(message);
		messages[j] = NULL;
	}

	message = (char *)eembed_realloc(messages[0], 2);
	if (message) {
		messages[0] = message;
	}
	message = (char *)eembed_reallocarray(messages[0], 1, test_object_size);
	if (message) {
		messages[0] = message;
	}

	for (i = 10; i; --i) {
		j = i - 1;
		message = messages[i - 1];
		eembed_free(message);
		messages[j] = NULL;
	}

	message = (char *)eembed_malloc(bigger_than_ram);
	failures += check_ptr(message, NULL);

	failures += ((mctx.frees == mctx.allocs) ? 0 : 1);
	failures += ((mctx.free_bytes == mctx.alloc_bytes) ? 0 : 1);

	eembed_global_allocator = orig;

	return failures;
}

void whine_if_context_data_corruption(struct echeck_err_injecting_context *ctx);
unsigned check_whine(void)
{
	unsigned int failures = 0;
	const size_t buf_size = 250;
	char buf[250];
	struct eembed_str_buf sbuf;
	struct eembed_log slog;
	struct echeck_err_injecting_context mctx;
	const char *found = NULL;

	eembed_memset(buf, 0x00, buf_size);
	eembed_memset(&mctx, 0x00, sizeof(struct echeck_err_injecting_context));
	mctx.log = eembed_char_buf_log_init(&slog, &sbuf, buf, buf_size);

	mctx.allocs = 2;
	mctx.alloc_bytes = 100;
	mctx.frees = 2;
	mctx.free_bytes = 200;
	mctx.max_used = 100;
	mctx.attempts = 1;

	whine_if_context_data_corruption(&mctx);

	found = eembed_strstr(buf, "free_bytes");
	failures += check_ptr_not_null_m(found, buf);

	found = eembed_strstr(buf, "alloc_bytes");
	failures += check_ptr_not_null_m(found, buf);

	found = eembed_strstr(buf, "100");
	failures += check_ptr_not_null_m(found, buf);

	found = eembed_strstr(buf, "200");
	failures += check_ptr_not_null_m(found, buf);

	return failures;
}

int test_out_of_memory(void)
{
	int failures = 0;
	uint32_t i = 0;
	uint32_t one = 1;
	uint32_t mask32 = 0;

	for (i = 0; i < 30; ++i) {
		mask32 = i;
		failures += test_out_of_memory_inner(mask32);
		mask32 = (one << i);
		failures += test_out_of_memory_inner(mask32);
		mask32 = ((one << i) | (one << (i + one)));
		failures += test_out_of_memory_inner(mask32);
	}

	failures += check_whine();

	return failures;
}

ECHECK_TEST_MAIN(test_out_of_memory)
