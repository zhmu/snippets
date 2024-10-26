#include "test.h"
#include <stdio.h>

static int num_expect_failures = 0;

void
test_report_expect_failure(const char* file, int line, const char* expr)
{
	fprintf(stderr, "expecture failed in %s:%d: %s\n", file, line, expr);
	num_expect_failures++;
}

int
test_get_num_of_expect_failures()
{
	return num_expect_failures;
}
