/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* demo-eembed-static-assert.c */
/* Copyright (C) 2022 Eric Herman <eric@freesa.org> */

/* compile with:
   gcc        -Isrc src/eembed.c tests/demo-eembed-static-assert.c && ./a.out
   gcc -DFAIL -Isrc src/eembed.c tests/demo-eembed-static-assert.c
*/

#include "eembed.h"

struct foo {
	int i;
};

struct bar {
	unsigned int u;
};

struct baz {
	unsigned char c;
};

eembed_static_assert_sizes_match(struct foo, struct bar);
#if (FAIL)
eembed_static_assert_sizes_match(struct foo, struct baz);
#endif

void print_stuct_size(const char *msg, size_t struct_size)
{
	const size_t size = 40;
	char buf[40];
	struct eembed_log *out = eembed_out_log;

	eembed_ulong_to_str(buf, size, struct_size);

	out->append_s(out, msg);
	out->append_s(out, buf);
	out->append_eol(out);
}

int main(void)
{
	print_stuct_size("sizeof(struct foo): ", sizeof(struct foo));
	print_stuct_size("sizeof(struct bar): ", sizeof(struct bar));
	print_stuct_size("sizeof(struct baz): ", sizeof(struct baz));

	return 0;
}
