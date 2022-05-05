/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2012, 2017, 2020 Eric Herman <eric@freesa.org> */

#include "echeck.h"

unsigned test_eembed_chunk_realloc(void)
{
	const size_t bytes_len = 256 * sizeof(size_t);
	unsigned char bytes[256 * sizeof(size_t)];
	struct eembed_allocator *ea = NULL;
	const size_t pointers_len = 8;
	char *pointers[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	const size_t test_object_size = bytes_len / (pointers_len * 2);
	const size_t test_object_size_2 = 1 + ((test_object_size * 3) / 4);
	const size_t buf_len = 80;
	char buf[80];
	size_t i = 0;
	unsigned failures = 0;

	ea = eembed_bytes_allocator(bytes, bytes_len);
	if (check_ptr_not_null(ea)) {
		return 1;
	}

	for (i = 0; i < pointers_len; i++) {
		if (i == 0) {
			pointers[i] =
			    (char *)ea->realloc(ea, NULL, test_object_size);
			eembed_memset(pointers[i], 0x00, test_object_size);
		} else {
			pointers[i] =
			    (char *)ea->calloc(ea, 1, test_object_size);
		}
		failures +=
		    check_ptr_not_null_m(pointers[i],
					 eembed_ulong_to_str(buf, buf_len, i));
		if (pointers[i]) {
			eembed_memset(pointers[i], '0' + i, test_object_size);
			pointers[i][6] = '\0';	/* truncate string */
		}
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
		failures +=
		    check_ptr_not_null_m(pointers[i],
					 eembed_ulong_to_str(buf, buf_len, i));
		if (pointers[i]) {
			eembed_memset(pointers[i], '0' + i, test_object_size_2);
			pointers[i][4] = '\0';	/* truncate string */
		}
	}

	for (i = 0; i < pointers_len; i++) {
		pointers[i] = (char *)ea->realloc(ea, pointers[i], 0);
		check_ptr_m(pointers[i], NULL,
			    eembed_ulong_to_str(buf, buf_len, i));
		ea->free(ea, pointers[i]);
	}

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_chunk_realloc)
