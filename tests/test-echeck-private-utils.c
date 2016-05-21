#include <stdio.h>
#include <string.h>

#include "test-echeck-private-utils.h"

char *set_log(char *log, const char *logdir, const char *filename)
{
	if (logdir) {
		sprintf(log, "%s%c%s.log", logdir, ECHECK_PATH_SEP, filename);
	} else {
		sprintf(log, "%s.log", filename);
	}
	return log;
}

int err_contains(const char *filename, char *expected[], size_t expected_len)
{
	FILE *log;
	char buffer[4096];
	size_t i, len;
	unsigned int msg_found;

	log = fopen(filename, "r");
	msg_found = 0;
	if (log) {
		len = 0;
		while (fgets(buffer + len, (sizeof buffer) - len, log)) {
			len = strlen(buffer);
			if (len >= (sizeof buffer)) {
				len = (sizeof buffer);
				buffer[len - 1] = '\0';
			}
		}
		if (len) {
			for (i = 0; i < expected_len; i++) {
				if (strstr(buffer, expected[i])) {
					msg_found++;
				} else {
					fprintf(stderr, "'%s' not in: %s\n",
						expected[i], buffer);
				}
			}
		}
		fclose(log);
	}
	return expected_len - msg_found;
}
