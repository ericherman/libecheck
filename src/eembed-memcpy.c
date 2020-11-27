/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "eembed.h"
#include "eembed-hosted.h"

#if EEMBED_HOSTED

#include <string.h>
void *(*eembed_memcpy)(void *dest, const void *src, size_t n) = memcpy;
void *(*eembed_memmove)(void *dest, const void *src, size_t n) = memmove;

#else

void *eembed_diy_memmove(void *d, const void *s, size_t n)
{
	const unsigned char *src = (const unsigned char *)s;
	unsigned char *dest = (unsigned char *)d;
	size_t i;

	if (src < dest && dest < src + n) {
		for (i = n; i; i--) {
			dest[i - 1] = src[i - 1];
		}
	} else {
		for (i = 0; i < n; ++i) {
			dest[i] = src[i];
		}
	}
	return d;
}

void *(*eembed_memcpy)(void *dest, const void *src, size_t n) =
    eembed_diy_memmove;
void *(*eembed_memmove)(void *dest, const void *src, size_t n) =
    eembed_diy_memmove;

#endif
