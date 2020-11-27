/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed-strstr.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "eembed.h"
#include "eembed-hosted.h"

#if EEMBED_HOSTED
#include <string.h>
char *(*eembed_strstr)(const char *haystack, const char *needle) = strstr;
#else
char *eembed_diy_strstr(const char *haystack, const char *needle)
{
	size_t i, j, found, len;

	if (!haystack) {
		return NULL;
	}

	len = 0;
	if (!needle || !needle[0]) {
		return (char *)haystack;
	}

	for (i = 0; haystack[i]; ++i) {
		found = 1;
		for (j = 0; found && j < len; ++j) {
			if (haystack[i + j] != needle[j]) {
				found = 0;
			}
		}
		if (found) {
			return (char *)(haystack + i);
		}
	}
	return NULL;
}

char *(*eembed_strstr)(const char *haystack, const char *needle) =
    eembed_diy_strstr;
#endif
