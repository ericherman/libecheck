/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* test-eembed-mempy.c */
/* Copyright (C) 2017, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_eembed_chunk_alloc(void)
{
	const size_t bytes_len = 200 * sizeof(size_t);
	unsigned char bytes[200 * sizeof(size_t)];
	const size_t keys_len = 50;
	char *keys[50];
	struct eembed_allocator *ea = NULL;
	const size_t buf_len = 80;
	char buf[80];
	char *key = NULL;
	size_t i = 0;
	size_t len = 0;
	size_t allocs = 0;
	size_t allocs2 = 0;
	unsigned failures = 0;

	ea = eembed_bytes_allocator(bytes, bytes_len);
	if (check_ptr_not_null(ea)) {
		return 1;
	}

	for (i = 0, allocs = 0; i < keys_len; ++i) {
		eembed_ulong_to_str(buf, buf_len, allocs);
		len = eembed_strnlen(buf, buf_len);
		key = (char *)ea->malloc(ea, len);
		len = eembed_align(len) + 1;
		key = (char *)ea->realloc(ea, key, len);
		if (key) {
			eembed_strcpy(key, buf);
			++allocs;
		}
		keys[i] = key;
	}

	failures += check_int(allocs > 5 ? 1 : 0, 1);

	for (i = keys_len / 2; i > 2; i -= 2) {
		ea->free(ea, keys[i]);
		keys[i] = NULL;
	}
	for (i = keys_len / 3; i > (keys_len / 2); i -= 3) {
		ea->free(ea, keys[i]);
		keys[i] = NULL;
	}

	for (i = 0, allocs2 = 0; i < keys_len; ++i) {
		eembed_ulong_to_str(buf, buf_len, allocs2);
		len = 1 + eembed_strnlen(buf, buf_len);
		key = (char *)ea->realloc(ea, keys[i], len);
		if (key) {
			++allocs2;
			eembed_strcpy(key, buf);
		}
		keys[i] = key;
	}

	failures += check_int(allocs2 > 1 ? 1 : 0, 1);

	key = (char *)ea->realloc(ea, keys[0], SIZE_MAX / 2);
	failures += check_ptr(key, NULL);

	for (i = 3; i < keys_len; ++i) {
		if (i % 7) {
			ea->free(ea, keys[i]);
		}
	}

	len = 1 + (eembed_align(len) * 4);
	key = (char *)ea->realloc(ea, keys[0], len);
	failures +=
	    check_ptr_not_null_m(key, eembed_ulong_to_str(buf, buf_len, len));

	for (i = 0; i < keys_len; ++i) {
		ea->free(ea, keys[i]);
	}

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_chunk_alloc)
