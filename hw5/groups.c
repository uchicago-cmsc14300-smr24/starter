#include "map.h"
#include "array-list.h"
#include "linked-list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* a line in the file */
struct record {
    char hometown[32];
    char fullname[64];
};

/******************************************************************************/
/*                              Helper Functions                              */
/******************************************************************************/

/* Read the next student from file:
 * If the file has no more student records, the record that rec_p points
 * to is untouched and the function returns false; otherwise, this function
 * writes the record to *rec_p, and returns true. If the record stored in the
 * file is invalid, this function aborts the program with an error message.
 */
bool read_record(FILE *file, struct record *rec_p);

/* Strcmp wrapper that has the right type to pass to create */
int string_cmp(void *, void *);

/******************************************************************************/
/*                         Your Function Declarations                         */
/******************************************************************************/



/******************************************************************************/
/*                                    Main                                    */
/******************************************************************************/

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("usage: %s <file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file;
    if (strcmp(argv[1], "-") == 0) {
        file = stdin;
    } else {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("cannot open '%s'\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    /* USE THE FILE HERE */

    fclose(file);

    return EXIT_SUCCESS;
}

/******************************************************************************/
/*                         Your Function Definitions                          */
/******************************************************************************/





/******************************************************************************/
/*                      Helper Function Implementations                       */
/******************************************************************************/

bool read_record(FILE *file, struct record *rec_p)
{
    assert(file != NULL);

    if (feof(file)) {
        /* if the file has nothing more to read, return false */
        return false;
    }

    /* declare and 0 initialize a rec to read into */
    struct record rec = { 0 };

    int n_read = fscanf(file, "%31[^\t]\t%63[^\n]\n",
            rec.hometown, rec.fullname);

    if (n_read != 2) {
        fprintf(stderr, "Invalid data file -- incomplete record\n");
        exit(EXIT_FAILURE);
    }

    *rec_p = rec;
    return true;
}

int string_cmp(void *s1, void *s2)
{
    return strcmp(s1, s2);
}

