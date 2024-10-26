#ifndef __ZHMU_TEST_H___
#define __ZHMU_TEST_H___

void test_report_expect_failure(const char* file, int line, const char* expr);
int test_get_num_of_expect_failures();

/* EXPECT(x) should yield in a condition 'x' which is true */
#define EXPECT(x) \
	!(x) ? test_report_expect_failure(__FILE__, __LINE__, #x) : (void)0

#endif /* __ZHMU_TEST_H___ */
