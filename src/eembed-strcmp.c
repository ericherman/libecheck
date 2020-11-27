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

int (*eembed_strcmp)(const char *s1, const char *s2) = strcmp;
int (*eembed_strncmp)(const char *s1, const char *s2, size_t n) = strncmp;

#else

int echeck_diy_strcmp(const char *s1, const char *s2)
{
	return eembed_strncmp(s1, s2, SIZE_MAX);
}

int (*eembed_strcmp)(const char *s1, const char *s2) = echeck_diy_strcmp;

int echeck_diy_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i = 0;

	if (s1 == s2) {
		return 0;
	} else if (s1 == NULL) {
		return -1;
	} else if (s2 == NULL) {
		return 1;
	}

	for (i = 0; i < n && s1[i] && s2[i]; ++i) {
		if (s1[i] != s2[i]) {
			break;
		}
	}

	return s1[i] - s2[i];
}

int (*eembed_strncmp)(const char *s1, const char *s2, size_t n) =
    echeck_diy_strncmp;
#endif
