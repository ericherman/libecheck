/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed-strcat.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "eembed.h"
#include "eembed-hosted.h"

#if EEMBED_HOSTED
#include <string.h>

char *(*eembed_strcat)(char *dest, const char *src) = strcat;
char *(*eembed_strncat)(char *dest, const char *src, size_t n) = strncat;

#else

char *eembed_diy_strcat(char *dest, const char *src)
{
	return eembed_strncat(dest, src, SIZE_MAX);
}

char *(*eembed_strcat)(char *dest, const char *src) = eembed_diy_strcat;

char *eembed_diy_strncat(char *dest, const char *src, size_t n)
{
	size_t dest_len = 0;
	size_t i = 0;

	if (!dest) {
		return NULL;
	}

	dest_len = eembed_strlen(dest);
	if (src) {
		while (i < n && src[i] != '\0') {
			dest[dest_len + i] = src[i];
			++i;
		}
	}
	dest[dest_len + i] = '\0';

	return dest;
}

char *(*eembed_strncat)(char *dest, const char *src, size_t n) =
    eembed_diy_strncat;

#endif
