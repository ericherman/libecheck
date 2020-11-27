/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed-strcpy.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "eembed.h"
#include "eembed-hosted.h"

#if EEMBED_HOSTED

#include <string.h>
char *(*eembed_strcpy)(char *dest, const char *src) = strcpy;
char *(*eembed_strncpy)(char *dest, const char *src, size_t n) = strncpy;

#else

char *eembded_diy_strcpy(char *dest, const char *src)
{
	return eembed_strncpy(dest, src, SIZE_MAX);
}

char *(*eembed_strcpy)(char *dest, const char *src) = eembded_diy_strcpy;

char *eembed_diy_strncpy(char *dest, const char *src, size_t n)
{
	size_t i = 0;

	if (!dest) {
		return NULL;
	}

	if (src) {
		while (i < n && src[i] != '\0') {
			dest[i] = src[i];
			++i;
		}
	}
	if (i < n) {
		dest[i] = '\0';
	}

	return dest;
}

char *(*eembed_strncpy)(char *dest, const char *src, size_t n) =
    eembed_diy_strncpy;

#endif
