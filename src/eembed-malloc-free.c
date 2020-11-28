/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eemebd-malloc.c - malloc/free wrappers */
/* Copyright (C) 2016, 2019, 2020 Eric Herman <eric@freesa.org> */

#include "eembed.h"
#include "eembed-hosted.h"
#include <stddef.h>

#if (EEMBED_HOSTED && (_DEFAULT_SOURCE || _GNU_SOURCE))
#define EEMBED_HAVE_HOSTED_REALLOC_ARRAY 1
#else
#define EEMBED_HAVE_HOSTED_REALLOC_ARRAY 0
void *eembed_diy_reallocarray(void *ptr, size_t nmemb, size_t size)
{
	return eembed_realloc(ptr, (nmemb * size));
}

void *(*eembed_reallocarray)(void *ptr, size_t nmemb, size_t size) =
    eembed_diy_reallocarray;
#endif

#if EEMBED_HOSTED

#include <stdlib.h>

void *(*eembed_malloc)(size_t size) = malloc;
void *(*eembed_realloc)(void *ptr, size_t size) = realloc;
void *(*eembed_calloc)(size_t nmemb, size_t size) = calloc;
void (*eembed_free)(void *ptr) = free;
#if EEMBED_HAVE_HOSTED_REALLOC_ARRAY
void *(*eembed_reallocarray)(void *ptr, size_t nmemb, size_t size) =
    reallocarray;
#endif

#else

void *eembed_noop_malloc(size_t size)
{
	(void)size;
	return NULL;
}

void *(*eembed_malloc)(size_t size) = eembed_noop_malloc;

void *eembed_noop_realloc(void *ptr, size_t size)
{
	(void)ptr;
	(void)size;
	return NULL;
}

void *(*eembed_realloc)(void *ptr, size_t size) = eembed_noop_realloc;

void *eembed_noop_calloc(size_t nmemb, size_t size)
{
	(void)nmemb;
	(void)size;
	return NULL;
}

void *(*eembed_calloc)(size_t nmemb, size_t size) = eembed_noop_calloc;

void eembed_noop_free(void *ptr)
{
	(void)ptr;
}

void (*eembed_free)(void *ptr) = eembed_noop_free;
#endif
