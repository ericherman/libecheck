#include "echeck.h"

unsigned int test_eembed_log(void)
{
	const size_t buf_len = 32;
	unsigned char buf1[32];
	unsigned char buf2[32];
	unsigned char buf3[32];

	size_t hexbuf_len = (2 + (2 * 32) + 1);
	char hexbuf[(2 + (2 * 32) + 1)];

	const size_t msg_len = 80 + (3 * 64);
	char msg[80 + (3 * 64)];

	unsigned failures = 0;
	int err = 0;
	size_t i = 0;
	size_t diff12 = 0;
	size_t diff13 = 0;
	size_t diff23 = 0;

	eembed_memset(msg, 0x00, msg_len);
	eembed_memset(buf1, 0x00, buf_len);
	eembed_memset(buf2, 0x00, buf_len);
	eembed_memset(buf3, 0x00, buf_len);

	eembed_strcat(msg, "buffers:\n");

	err = eembed_random_bytes(buf1, buf_len);
	eembed_bytes_to_hex(hexbuf, hexbuf_len, buf1, buf_len);
	failures += check_int_m(err, 0, hexbuf);
	eembed_strcat(msg, "buf1: ");
	eembed_strcat(msg, hexbuf);
	eembed_strcat(msg, "\n");

	err = eembed_random_bytes(buf2, buf_len);
	eembed_bytes_to_hex(hexbuf, hexbuf_len, buf2, buf_len);
	failures += check_int_m(err, 0, hexbuf);
	eembed_strcat(msg, "buf2: ");
	eembed_strcat(msg, hexbuf);
	eembed_strcat(msg, "\n");

	err = eembed_random_bytes(buf3, buf_len);
	eembed_bytes_to_hex(hexbuf, hexbuf_len, buf3, buf_len);
	failures += check_int_m(err, 0, hexbuf);
	eembed_strcat(msg, "buf3: ");
	eembed_strcat(msg, hexbuf);
	eembed_strcat(msg, "\n");

	for (i = 0; i < buf_len; ++i) {
		if (buf1[i] != buf2[i]) {
			++diff12;
		}
		if (buf1[i] != buf3[i]) {
			++diff13;
		}
		if (buf2[i] != buf3[i]) {
			++diff23;
		}
	}

	failures += check_int_m((diff12 > 0 ? 1 : 0), 1, msg);
	failures += check_int_m((diff13 > 0 ? 1 : 0), 1, msg);
	failures += check_int_m((diff23 > 0 ? 1 : 0), 1, msg);

	return failures;
}

ECHECK_TEST_MAIN(test_eembed_log)
