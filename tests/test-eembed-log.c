#include "eembed.h"
#include "eembed-hosted.h"

unsigned int test_eembed_log(void)
{
	size_t buf_len = 250;
	char buf[250];
	struct eembed_str_buf bctx;
	struct eembed_log llog;
	struct eembed_log *log = NULL;
	int diff = 0;
	int failures = 0;
	const char *expected = NULL;

	buf[0] = '\0';
	log = eembed_char_buf_log_init(&llog, &bctx, buf, buf_len);
	if (!log) {
		++failures;
		return failures;
	}

	log->append_s(log, "foo ");
	log->append_z(log, 4);
	log->append_s(log, " bar");
	log->append_eol(log);
	expected = "foo 4 bar\n";

	diff = eembed_strncmp(expected, buf, buf_len);
	if (diff) {
		eembed_err_log->append_s(eembed_err_log, "Expedcted '");
		eembed_err_log->append_s(eembed_err_log, expected);
		eembed_err_log->append_s(eembed_err_log, "' but was '");
		eembed_err_log->append_s(eembed_err_log, buf);
		eembed_err_log->append_s(eembed_err_log, "'");
		eembed_err_log->append_eol(eembed_err_log);
		eembed_err_log->append_s(eembed_err_log, "\tdiff of ");
		eembed_err_log->append_l(eembed_err_log, diff);
		eembed_err_log->append_eol(eembed_err_log);
		++failures;
	}

	return failures;
}

#if FAUX_FREESTANDING
#include <stdio.h>
void eembed_faux_freestanding_system_print(const char *str)
{
	printf(str);
}

void eembed_system_system_println(void)
{
	printf("\n");
}
#endif

#if (EEMBED_HOSTED || FAUX_FREESTANDING)
int main(void)
{
	unsigned int failures = 0;

#if FAUX_FREESTANDING
	eembed_system_print = eembed_faux_freestanding_system_print;
	eembed_system_println = eembed_system_system_println;
#endif

	failures += test_eembed_log();

	return (failures) ? 1 : 0;
}
#endif
