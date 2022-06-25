/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2012, 2017, 2020 Eric Herman <eric@freesa.org> */

#include "eembed.h"

unsigned test_eembed_chunk_realloc(void)
{
	const size_t bytes_len = 256 * sizeof(size_t);
	unsigned char bytes[256 * sizeof(size_t)];
	struct eembed_allocator *ea = NULL;
	const size_t pointers_len = 8;
	char *pointers[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	const size_t test_object_size = bytes_len / (pointers_len * 2);
	const size_t test_object_size_2 = 1 + ((test_object_size * 3) / 4);
	size_t i = 0;

	ea = eembed_bytes_allocator(bytes, bytes_len);
	eembed_crash_if_false(ea);

	for (i = 0; i < pointers_len; i++) {
		if (i == 0) {
			pointers[i] =
			    (char *)ea->realloc(ea, NULL, test_object_size);
			eembed_memset(pointers[i], 0x00, test_object_size);
		} else {
			pointers[i] =
			    (char *)ea->calloc(ea, 1, test_object_size);
		}
		eembed_crash_if_false(pointers[i] != NULL);
		eembed_memset(pointers[i], '0' + i, test_object_size);
		pointers[i][6] = '\0';	/* truncate string */
	}
	for (i = 1; i < pointers_len; i += 2) {
		ea->free(ea, pointers[i]);
	}
	for (i = 1; i < pointers_len; ++i) {
		if (i % 2) {
			pointers[i] =
			    (char *)ea->realloc(ea, pointers[i],
						test_object_size_2);
		} else {
			pointers[i] =
			    (char *)ea->reallocarray(ea, pointers[i], 1,
						     test_object_size_2);
		}
		eembed_crash_if_false(pointers[i] != NULL);
		eembed_memset(pointers[i], '0' + i, test_object_size_2);
		pointers[i][4] = '\0';	/* truncate string */
	}

	for (i = 0; i < pointers_len; i++) {
		pointers[i] = (char *)ea->realloc(ea, pointers[i], 0);
		eembed_crash_if_false(pointers[i] == NULL);
		ea->free(ea, pointers[i]);
	}

	return 0;
}

EEMBED_FUNC_MAIN(test_eembed_chunk_realloc)
