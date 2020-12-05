/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_eembed_malloc_free(void)
{
	const size_t bytes_len = 500 * sizeof(size_t);
	unsigned char bytes[500 * sizeof(size_t)];
	struct eembed_allocator *orig = eembed_global_alloctor;
	struct eembed_allocator *ea = NULL;
	size_t i = 0;
	size_t j = 0;
	char *message = NULL;
	const size_t messages_len = 8;
	char *messages[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	const size_t test_object_size = bytes_len / (2 * messages_len);
	unsigned failures = 0;

	if (!EEMBED_HOSTED) {
		ea = eembed_bytes_allocator(bytes, bytes_len);
		if (check_ptr_not_null(ea)) {
			return 1;
		}
		eembed_global_alloctor = ea;
	}

	message = (char *)eembed_malloc(0);
	eembed_free(message);

	message = (char *)eembed_malloc(SIZE_MAX);
	failures += check_ptr(message, NULL);
	eembed_free(message);

	for (i = 0; i < messages_len; ++i) {
		switch (i % 4) {
		case 3:
			message = (char *)eembed_calloc(1, test_object_size);
			break;
		case 2:
			message =
			    (char *)eembed_realloc(NULL, test_object_size);
			break;
		case 1:
			message =
			    (char *)eembed_reallocarray(NULL, 1,
							test_object_size);
			break;
		default:
			message = (char *)eembed_malloc(test_object_size);
		}
		if (!message) {
			++failures;
		} else {
			if (i % 4 == 3) {
				for (j = 0; j < test_object_size; ++j) {
					check_int_m(message[j], 0, message);
				}
			}
			eembed_memset(message, ('A' + i), test_object_size);
			message[8] = '\0';	/* truncate for debug */
		}
		messages[i] = message;
	}
	for (i = 8; i > 2; i -= 2) {
		j = i - 1;
		message = messages[j];
		eembed_free(message);
		messages[j] = NULL;
	}
	for (i = 8; i; --i) {
		j = i - 1;
		message = messages[i - 1];
		eembed_free(message);
		messages[j] = NULL;
	}

	if (!EEMBED_HOSTED) {
		eembed_global_alloctor = orig;
	}

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_malloc_free)
