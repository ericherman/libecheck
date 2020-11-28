/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* oom-injecting-malloc.h */
/* Copyright (C) 2016, 2019, 2020 Eric Herman <eric@freesa.org> */

#ifndef ECHECK_ERR_INJECTING_MALLOC_H
#define ECHECK_ERR_INJECTING_MALLOC_H 1

#ifdef __cplusplus
#define Echeck_err_injecting_begin_C_functions extern "C" {
#define Echeck_err_injecting_end_C_functions }
#else
#define Echeck_err_injecting_begin_C_functions
#define Echeck_err_injecting_end_C_functions
#endif

Echeck_err_injecting_begin_C_functions
#undef Echeck_err_injecting_begin_C_functions
#include "eembed.h"
    struct echeck_err_injecting_context;

int echeck_err_injecting_allocator_init(struct eembed_allocator *with_errs,
					struct eembed_allocator *real,
					struct echeck_err_injecting_context *c,
					struct eembed_log *log);

struct echeck_err_injecting_context {
	unsigned long allocs;
	unsigned long alloc_bytes;
	unsigned long frees;
	unsigned long free_bytes;
	unsigned long fails;
	unsigned long max_used;
	unsigned long attempts;
	unsigned long attempts_to_fail_bitmask;

	struct eembed_allocator *real;
	struct eembed_log *log;
};

Echeck_err_injecting_end_C_functions
#undef Echeck_err_injecting_end_C_functions
#endif /* ECHECK_ERR_INJECTING_MALLOC_H */
