/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <stddef.h>

#include "eembed.h"
#include "eembed-hosted.h"

#if EEMBED_HOSTED

#include <string.h>
void *(*eembed_memset)(void *dest, int c, size_t n) = memset;

#else

void *eembed_diy_memset(void *dest, int val, size_t n)
{
	unsigned char *d;
	unsigned char v;
	if (!n || !dest) {
		return dest;
	}

	d = (unsigned char *)dest;
	v = (unsigned char)val;
	while (n--) {
		d[n] = v;
	}

	return dest;
}

void *(*eembed_memset)(void *dest, int c, size_t n) = eembed_diy_memset;

#endif
