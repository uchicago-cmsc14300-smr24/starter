/*
 * Implementation of a simple testing infrastructure.
 *
 * Author: Byron Zhong (byronzhong@cs.uchicago.edu)
 * Description: This file implements the "test.h" interface, providing the main
 * function for the unit tests to run and implementing the checking facilities.
 */
#include "tests.h"

#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* indicate whether the jmp_buf is set */
static bool initialized = false;

/* the place to jump to in case of an error */
static jmp_buf error_jmp;

enum reason {
    ASSERT = 1,
    SEGFAULT,
    ABORT,
};

struct config {
    bool keep_going;
    bool capture_signals;
};

/* helper function to run all provided tests */
static int run_tests(struct config *c, struct unittest tests[], int n_tests);

/* callback function when SIGSEGV is raised */
static void segv_handler(int);

/* callback function when SIGABRT is raised */
static void abrt_handler(int);

/******************************************************************************/
/*                                 test main                                  */
/******************************************************************************/

int test_main(int argc, char *argv[], struct unittest tests[], int n_tests)
{
    initialized = true;

    struct config c = {
        .keep_going = false,
        .capture_signals = false,
    };

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--keep-going") == 0) {
            c.keep_going = true;
        } else if (strcmp(argv[i], "--capture") == 0) {
            c.capture_signals = true;
        } else if (strcmp(argv[i], "-h") == 0) {
            fprintf(stderr, "Usage: %s [options]\nOptions:\n", argv[0]);
            fprintf(stderr,
                    "\t--keep-going\tKeep going after a test failure.\n");
            fprintf(stderr,
                    "\t--capture\tCapture segmentation fault and abort signal. "
                    "This is useful in combination of --keep-going since it "
                    "enables the rest of tests to run even after a crash.\n");
            fprintf(stderr, "\t-t        \tPrint this message.\n");
            return EXIT_FAILURE;
        } else {
            fprintf(stderr, "Unrecognized flag '%s', use '-h' to see the list "
                            "of options\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    if (c.capture_signals) {
        signal(SIGSEGV, segv_handler);
        signal(SIGABRT, abrt_handler);
    }

    return run_tests(&c, tests, n_tests);
}

static int run_tests(struct config *c, struct unittest tests[], int n_tests)
{
    int n_test_failed = 0;
    int n_test_run = 0;

    sigset_t all_signals;
    sigfillset(&all_signals);

    for (int i = 0; i < n_tests; i++) {
        bool succeed = false;
        n_test_run++;

        switch (setjmp(error_jmp)) {
            case 0:
                printf("Running tests: %-32s", tests[i].name);
                sigprocmask(SIG_UNBLOCK, &all_signals, NULL);
                tests[i].run();
                printf("\t\tPASS\n");
                succeed = true;
                break;
            case ASSERT:
                /* error messages are printed in the assertions */
                break;
            case ABORT:
                printf("\t\tFAIL\n\tTest aborted\n");
                break;
            case SEGFAULT:
                printf("\t\tFAIL\n\tTest segfault'ed\n");
                break;
            default:
                fprintf(stderr,
                        "INTERNAL ERROR: unknown longjmp code\n");
                abort();
        };

        if (!succeed) {
            n_test_failed++;
            if (!c->keep_going) {
                break;
            }
        }
    }

    printf("%d tests are registered, %d tests ran, "
            "%d succeeded, %d failed.\n",
            n_tests,
            n_test_run,
            n_test_run - n_test_failed,
            n_test_failed);

    if (n_test_failed > 0) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}

static void segv_handler(int signum)
{
    (void) signum;
    longjmp(error_jmp, SEGFAULT);
}

static void abrt_handler(int signum)
{
    (void) signum;
    longjmp(error_jmp, ABORT);
}

/******************************************************************************/
/*                            Exported Functions                              */
/******************************************************************************/


FILE *mkfile(const char *str)
{
    return mkfilen(str, strlen(str));
}

FILE *mkfilen(const char *str, size_t n)
{
    FILE *file = tmpfile();
    if (file == NULL) {
        fprintf(stderr, "Unable to run tests because a tmpfile cannot "
                "be created in the current directory");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < n; i++) {
        fputc(str[i], file);
    }

    fflush(file);
    rewind(file);

    return file;
}

char **mk_random_strs(int n)
{
    char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890_";
    const int n_chars = sizeof(charset) - 1;

    char **strs = calloc(n, sizeof(*strs));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n_chars; j++) {
            int k = rand() % n_chars;

            char c = charset[j];
            charset[j] = charset[k];
            charset[k] = c;
        }
        strs[i] = strdup(charset);
    }

    return strs;
}

void test_fail(const char *format, ...)
{
    if (!initialized) {
        fprintf(stderr,
                "INTERNAL ERROR: tests are run outside of test_main\n");
        abort();
    }

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    longjmp(error_jmp, ASSERT);
}

void test_expect_eq(unsigned long exp, unsigned long actual,
        const char *file, int line)
{
    if (exp == actual) {
        return;
    }

    test_fail("\t\tFAIL\n\t%s:%d: expecting %ld but received %ld\n",
            file, line, exp, actual);
}

void test_expect_null(const void *s, const char *file, int line)
{
    if (s == NULL) {
        return;
    }

    test_fail("\t\tFAIL\n\t%s:%d: expecting NULL but received %p\n",
            file, line, s);
}

void test_expect_non_null(const void *s, const char *file, int line)
{
    if (s != NULL) {
        return;
    }

    test_fail("\t\tFAIL\n\t%s:%d: expecting non-NULL but received NULL\n",
            file, line);
    longjmp(error_jmp, ASSERT);
}

void test_expect_mem(const void *s1, const void *s2, size_t n,
        const char *file, int line)
{
    if (memcmp(s1, s2, n) == 0) {
        return;
    }

    printf("\t\tFAIL\n\t%s:%d: %p and %p differ\n", file, line, s1, s2);

    if (n >= 64) {
        printf("Difference omitted.\n");
    } else {
        for (size_t i = 0; i < n; i++) {
            char b1 = *((char *)s1 + i);
            char b2 = *((char *)s2 + i);

            printf("%p+%02lu: %3d, 0x%02x, '%c'\n",
                    s1, i, b1, b1, b1);
            printf("%p+%02lu: %3d, 0x%02x, '%c'\n",
                    s2, i, b2, b2, b2);
        }
    }

    longjmp(error_jmp, ASSERT);
}

void test_expect_str(const char *s1, const char *s2, const char *file, int line)
{
    if (strcmp(s1, s2) == 0) {
        return;
    }

    test_fail("\t\tFAIL\n\t%s:%d: \"%s\" and \"%s\" differ\n", file, line,
            s1, s2);
}

