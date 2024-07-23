#ifndef TESTS_H_
#define TESTS_H_
/*
 * An extremely lightweight testing framework, inspired by Criterion.
 *
 * Author: Byron Zhong (byronzhong@cs.uchicago.edu)
 * Description: This testing framework provides some basic functions to
 * facilitate writing unit tests for your modules.
 *
 * Given an array of unit tests, the main function will run them in order, and
 * report the results of the each test. A test is a function having the
 * following signature:
 *         void run(void);
 * A test is considered successful if it returns normally. If any the following
 * expectation/assertion is not true, the function will abort at that point and
 * the test is considered a failure.
 */

#include <stdlib.h>
#include <stdio.h>

struct unittest {
    const char *name;  /* the name of this test */
    void (*run)(void); /* the function to run */
};

/* short-hand to use the same name as the function's name */
#define Test(func) { .name = #func, .run = func }

/* delegating the main function in the specific test */
int test_main(int argc, char *argv[], struct unittest tests[], int n_tests);

/* create a tmpfile containing the string str */
FILE *mkfile(const char *str);

/* same as the above, but only for the first n bytes in str */
FILE *mkfilen(const char *str, size_t n);

/* make n random heap-allocated strings */
char **mk_random_strs(int n);

/* abort a test */
void test_fail(const char *format, ...);

/* Pseudo-function interface
 *
 * Checking if a number is expected:
 *         void expect_eq(number exp, number actual);
 *
 * Checking if a pointer is null:
 *         void expect_null(void *s);
 *
 * Checking if a pointer is not null:
 *         void expect_non_null(void *s);
 *
 * Checking if two chunks of memory are equal:
 *         void expect_mem(void *s1, void *s2, size_t n);
 *
 * Checking if two strings are equal:
 *         void expect_str(void *s1, void *s2);
 *
 * Failing a test unconditionally:
 *         void expect_fail()
 */

/* YOU DON'T NEED TO LOOK BEYOND THIS POINT */

#define expect_eq(x, y)    test_expect_eq(x, y, __FILE__, __LINE__)
#define expect_null(s)     test_expect_null(s, __FILE__, __LINE__)
#define expect_non_null(s) test_expect_non_null(s, __FILE__, __LINE__)
#define expect_mem(s1, s2, n) test_expect_mem(s1, s2, n, __FILE__, __LINE__)
#define expect_str(s1, s2) test_expect_str(s1, s2, __FILE__, __LINE__)
#define expect_fail()      test_fail(                                  \
        "\t\tFAIL\n\t%s:%d: unconditional failure", __FILE__, __LINE__)

void test_expect_eq(unsigned long exp, unsigned long actual,
        const char *file, int line);

void test_expect_null(const void *s, const char *file, int line);

void test_expect_non_null(const void *s, const char *file, int line);

void test_expect_mem(const void *s1, const void *s2, size_t n,
        const char *file, int line);

void test_expect_str(const char *s1, const char *s2,
        const char *file, int line);

#endif
