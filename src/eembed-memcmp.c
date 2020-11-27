/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed-memcmp.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "eembed.h"
#include "eembed-hosted.h"

#if EEMBED_HOSTED

#include <string.h>
int (*eembed_memcmp)(const void *s1, const void *s2, size_t n) = memcmp;

#else

int eembed_diy_memcmp(const void *a1, const void *a2, size_t n)
{
	size_t i;
	const unsigned char *s1;
	const unsigned char *s2;
	int d;

	if (a1 == a2 || n == 0) {
		return 0;
	}

	if (!a1 || !a2) {
		return a1 ? 1 : -1;
	}

	s1 = (unsigned char *)a1;
	s2 = (unsigned char *)a2;
	for (i = 0; i < n; ++i) {
		d = s1[i] - s2[i];
		if (d) {
			return d;
		}
	}
	return 0;
}

int (*eembed_memcmp)(const void *s1, const void *s2, size_t n) =
    eembed_diy_memcmp;
#endif
