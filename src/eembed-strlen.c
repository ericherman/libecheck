/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed.c */
/* libeembed: "E(asy)Embed": easier writing and testing of embedded libraries */
/* Copyright (C) 2020 Eric Herman <eric@freesa.org> */

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include "eembed.h"
#include "eembed-hosted.h"

#if (!((_POSIX_C_SOURCE >= 200809L) && (EEMBED_HOSTED)))
#define Eembed_use_diy_strnlen 1
size_t eembed_diy_strnlen(const char *str, size_t buf_size)
{
	size_t i;

	for (i = 0; i < buf_size; ++i) {
		if (str[i] == '\0') {
			return i;
		}
	}

	return buf_size;
}
#else
#define Eembed_use_diy_strnlen 0
#endif

#if EEMBED_HOSTED

#include <string.h>
size_t (*eembed_strlen)(const char *s) = strlen;
#if Eembed_use_diy_strnlen
size_t (*eembed_strnlen)(const char *s, size_t maxlen) = eembed_diy_strnlen;
#else
size_t (*eembed_strnlen)(const char *s, size_t maxlen) = strnlen;
#endif

#else

size_t eembed_diy_strlen(const char *s)
{
	return eembed_strnlen(s, SIZE_MAX);
}

size_t (*eembed_strlen)(const char *s) = eembed_diy_strlen;

size_t (*eembed_strnlen)(const char *s, size_t maxlen) = eembed_diy_strnlen;

#endif
