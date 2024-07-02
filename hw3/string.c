/* string.c
 *
 * Convert between the plain-text format and the binary format for student
 * records.
 *
 * usage: ./string [-r,--test-readline] [-c,--test-concat] <file>
 *    -c,--test-readline  Reads each line in file and immediately prints it to
 *                        stdout using your readline function.
 *    -r,--test-concat    Reads each line in file, concatenate every 10 lines
 *                        into one string, and prints the string.
 *    <file>              a filename. If the filename is "-", then the program
 *                        will read from the standard input.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

/*
 * Read a single line from file.
 *
 * This function reads each character until either a newline character '\n'
 * or EOF, assigns a heap-allocated string to *line_p, and return the length of
 * the string.
 */
int readline(FILE* file, char** line_p);

/*
 * Concatenate an array of strings.
 *
 * This function takes an array of strings and concatenates them into a single
 * string. The resulting string is heap-allocated and must be freed by the
 * caller. `n_strings` is the number of strings in the array and `strings` is an
 * array of C strings, each of which is null-terminated.
 */
char* concat(int n_strings, char** strings);


/******************************************************************************/
/*                          YOUR IMPLEMENTATION HERE                          */
/******************************************************************************/

int readline(FILE* file, char** line_p)
{
    assert(file != NULL && line_p != NULL);

    /* YOUR CODE HERE */

    return 0; // change this to an appropriate value
}

char* concat(int n_strings, char** strings)
{
    assert(n_strings > 0 && strings != NULL);

    /* YOUR CODE HERE */

    return NULL; // change this to an appropriate value
}

/******************************************************************************/
/*                                TESTING CODE                                */
/******************************************************************************/
void test_readline(FILE *file)
{
    int len = 0;
    char* line = NULL;
    int line_no = 1;

    while ((len = readline(file, &line)) != 0) {
        /* Prints line number, the length of the line, and the line itself */
        printf("%5d:%5d:%s", line_no++, len, line);
        free(line);
    }
}

void test_concat(FILE *file)
{
    char *lines[10];
    int n_read = 0, group_no = 0, len;

    while ((len = readline(file, &lines[n_read++])) != 0) {
        if (n_read == 10) {
            char *str = concat(10, lines);
            printf("GROUP %5d ********************************************\n",
                    group_no);
            fputs(str, stdout);
            printf("END   %5d ********************************************\n\n",
                    group_no);
            n_read = 0;
            group_no++;

            free(str);
            for (int i = 0; i < 10; i++) {
                free(lines[i]);
            }
        }
    }

    n_read = n_read - 1; // offsetting the last n_read++

    if (n_read > 0) {
        char *str = concat(n_read, lines);
        printf("LAST *************************************************\n");
        fputs(str, stdout);
        printf("END  *************************************************\n");

        free(str);
        for (int i = 0; i < n_read; i++) {
            free(lines[i]);
        }
    }
}


/******************************************************************************/
/*                                    MAIN                                    */
/******************************************************************************/

void usage(const char* name);

int main(int argc, char *argv[])
{
    bool test_r = false, test_c = false;
    FILE* input = NULL;

    /* Parse command-line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0
                || strcmp(argv[i], "--test-readline") == 0) {
            test_r = true;
        } else if (strcmp(argv[i], "-c") == 0
                || strcmp(argv[i], "--test-concat") == 0) {
            test_c = true;
        } else {
            if (input != NULL) {
                printf("More than one file is given.\n");
                usage(argv[0]);
            }
            if (strcmp(argv[i], "-") == 0) {
                input = stdin;
            } else {
                input = fopen(argv[i], "r");
                if (input == NULL) {
                    printf("Cannot open the file '%s'\n", argv[i]);
                    usage(argv[0]);
                }
            }
        }
    }

    if (test_c) {
        /* test_c takes precedence since concat contains readline. */
        test_concat(input);
    } else if (test_r) {
        test_readline(input);
    } else {
        printf("Must specify a test\n");
        usage(argv[0]);
    }

    fclose(input);
    return EXIT_SUCCESS;
}

void usage(const char* name)
{
    printf("usage: %s [-r,--test-readline] [-c,--test-concat] <file>\n", name);
    printf("\t-r,--test-readline\tReads each line in file and immediately"
            " prints it to stdout using your readline function.\n");
    printf("\t-c,--test-concat\tReads each line in file, concatenate every 10 "
            "lines into one string, and prints the string.\n");
    printf("\t<file>\ta filename. If the filename is \"-\","
            " then the program will read from the standard input.\n");

    exit(EXIT_FAILURE);
}
