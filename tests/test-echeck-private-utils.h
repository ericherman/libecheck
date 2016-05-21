#ifndef TEST_ECHECK_PRIVATE_UTILS_H
#define TEST_ECHECK_PRIVATE_UTILS_H

/* TODO: do something portable alternative here */
#define ECHECK_PATH_MAX 4096
#define ECHECK_PATH_SEP '/'

char *set_log(char *log, const char *logdir, const char *filename);

int err_contains(const char *filename, char *expected[], size_t expected_len);

#endif /* TEST_ECHECK_PRIVATE_UTILS_H */
