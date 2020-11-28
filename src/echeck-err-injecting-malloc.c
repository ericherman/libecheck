/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* echeck-err-injecting-malloc.c */
/* Copyright (C) 2016, 2019, 2020 Eric Herman <eric@freesa.org> */

#include "echeck-err-injecting-malloc.h"
#include "eembed-assert.h"

void *echeck_err_injecting_malloc(struct eembed_allocator *ea, size_t size)
{
	struct eembed_log *log = NULL;
	struct eembed_allocator *real;
	struct echeck_err_injecting_context *ctx = NULL;
	unsigned char *tracking_buffer = NULL;
	void *ptr = NULL;
	size_t wide = 0;
	size_t used = 0;

	ctx = (struct echeck_err_injecting_context *)ea->context;
	if (0x01 & (ctx->attempts_to_fail_bitmask >> ctx->attempts++)) {
		return NULL;
	}
	real = ctx->real;
	wide = sizeof(size_t) + size;
	tracking_buffer = (unsigned char *)real->malloc(real, wide);
	if (!tracking_buffer) {
		++ctx->fails;
		return NULL;
	}

	eembed_memcpy(tracking_buffer, &size, sizeof(size_t));
	++ctx->allocs;
	ctx->alloc_bytes += size;
	if (ctx->free_bytes > ctx->alloc_bytes) {
		log = ctx->log;
		log->append_s(log, __FILE__);
		log->append_s(log, ":");
		log->append_ul(log, __LINE__);
		log->append_s(log, " BAD MOJO: ");
		log->append_s(log, " free_bytes > alloc_bytes?! (");
		log->append_ul(log, ctx->free_bytes);
		log->append_s(log, " > ");
		log->append_ul(log, ctx->alloc_bytes);
		log->append_s(log, ")");
		log->append_eol(log);
	}
	used = ctx->alloc_bytes - ctx->free_bytes;
	if (used > ctx->max_used) {
		ctx->max_used = used;
	}
	ptr = (void *)(tracking_buffer + sizeof(size_t));
	return ptr;
}

void *echeck_err_injecting_calloc(struct eembed_allocator *ea, size_t nmemb,
				  size_t size)
{
	void *ptr = ea->malloc(ea, nmemb * size);
	if (ptr) {
		eembed_memset(ptr, 0x00, nmemb * size);
	}
	return ptr;
}

void *echeck_err_injecting_realloc(struct eembed_allocator *ea, void *ptr,
				   size_t newsize)
{
	unsigned char *tracking_buffer = NULL;
	size_t size = 0;
	void *ptr2 = NULL;

	if (ptr == NULL) {
		return ea->malloc(ea, newsize);
	}

	tracking_buffer = ((unsigned char *)ptr) - sizeof(size_t);
	eembed_memcpy(&size, tracking_buffer, sizeof(size_t));

	if (newsize == size) {
		return ptr;
	}

	ptr2 = ea->calloc(ea, 1, newsize);
	if (!ptr2) {
		return ptr2;
	}

	eembed_memcpy(ptr2, ptr, newsize < size ? newsize : size);

	ea->free(ea, ptr);

	return ptr2;
}

void *echeck_err_injecting_reallocarray(struct eembed_allocator *ea, void *ptr,
					size_t nmemb, size_t size)
{
	return ea->realloc(ea, ptr, (nmemb * size));
}

void echeck_err_injecting_free(struct eembed_allocator *ea, void *ptr)
{
	struct eembed_log *log = NULL;
	struct eembed_allocator *real;
	struct echeck_err_injecting_context *ctx = NULL;
	unsigned char *tracking_buffer = NULL;
	size_t size = 0;

	if (ptr == NULL) {
		++ctx->fails;
		return;
	}

	ctx = (struct echeck_err_injecting_context *)ea->context;
	tracking_buffer = ((unsigned char *)ptr) - sizeof(size_t);
	eembed_memcpy(&size, tracking_buffer, sizeof(size_t));

	real = ctx->real;
	real->free(real, tracking_buffer);

	ctx->free_bytes += size;
	++ctx->frees;
	if (ctx->free_bytes > ctx->alloc_bytes) {
		log = ctx->log;
		log->append_s(log, __FILE__);
		log->append_s(log, ":");
		log->append_ul(log, __LINE__);
		log->append_s(log, " BAD MOJO: ");
		log->append_s(log, " free_bytes > alloc_bytes?! (");
		log->append_ul(log, ctx->free_bytes);
		log->append_s(log, " > ");
		log->append_ul(log, ctx->alloc_bytes);
		log->append_s(log, ")");
		log->append_eol(log);
	}
}

int echeck_err_injecting_allocator_init(struct eembed_allocator *with_errs,
					struct eembed_allocator *real,
					struct echeck_err_injecting_context *c,
					struct eembed_log *log)
{
	eembed_assert(with_errs);
	eembed_assert(real);
	eembed_assert(c);
	eembed_assert(log);

	if (!with_errs || !real || !c || !log) {
		return 1;
	}

	eembed_memset(c, 0x00, sizeof(struct echeck_err_injecting_context));
	c->real = real;
	c->log = log;

	with_errs->malloc = echeck_err_injecting_malloc;
	with_errs->calloc = echeck_err_injecting_calloc;
	with_errs->realloc = echeck_err_injecting_realloc;
	with_errs->reallocarray = echeck_err_injecting_reallocarray;
	with_errs->free = echeck_err_injecting_free;

	return 0;
}
