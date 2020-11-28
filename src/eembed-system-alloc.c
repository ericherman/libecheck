/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* eembed-system-alloc.c - malloc/free wrappers */
/* Copyright (C) 2016, 2019, 2020 Eric Herman <eric@freesa.org> */

#include "eembed.h"

void *eembed_system_malloc(struct eembed_allocator *ea, size_t size)
{
	(void)ea;
	return eembed_malloc(size);
}

void *eembed_system_realloc(struct eembed_allocator *ea, void *ptr, size_t size)
{
	(void)ea;
	return eembed_realloc(ptr, size);
}

void *eembed_system_calloc(struct eembed_allocator *ea, size_t nmemb,
			   size_t size)
{
	(void)ea;
	return eembed_calloc(nmemb, size);
}

void *eembed_system_reallocarray(struct eembed_allocator *ea, void *ptr,
				 size_t nmemb, size_t size)
{
	(void)ea;
	return eembed_realloc(ptr, nmemb * size);
}

void eembed_system_free(struct eembed_allocator *ea, void *ptr)
{
	(void)ea;
	eembed_free(ptr);
}

struct eembed_allocator eembed_system_alloctor = {
	NULL,
	eembed_system_malloc,
	eembed_system_calloc,
	eembed_system_realloc,
	eembed_system_reallocarray,
	eembed_system_free
};

struct eembed_allocator *eembed_global_alloctor = &eembed_system_alloctor;
