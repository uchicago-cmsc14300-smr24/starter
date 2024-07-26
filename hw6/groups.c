#include "dict.h"
#include "hash.h"
#include "array-list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdnoreturn.h>

struct record {
    char hometown[32];
    char fullname[64];
};

/* Prints a usage message and quits. noreturn tells the compiler that this
 * function does not return.
 */
noreturn void usage(const char *name);

/* Read the next student from file:
 * If the file has no more student records, the record that rec_p points
 * to is untouched and the function returns false; otherwise, this function
 * writes the record to *rec_p, and returns true. If the record stored in the
 * file is invalid, this function aborts the program with an error message.
 */
bool read_record(FILE *file, struct record *rec_p);

/* Read each line of the file and populate the dictionary */
void index_file(FILE *file, struct dict *dict);

/* Add an entry to the table.
 * If the hometown already exists in the table, append fullname to the list;
 * otherwise, insert a singleton list with fullname.
 */
void add_entry(struct dict *dict, char *hometone, char *fullname);

/* A visitor function that prints a group */
void print_group(void *key, void *value, void *data);

/* A visitor function that frees all entries in the dictionary */
void free_group(void *key, void *value, void *data);

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage(argv[0]);
    }

    struct dict *dict;
    if (strcmp(argv[1], "-t") == 0) {
        dict = dict_create_table(1024, string_cmp, string_hash);
    } else if (strcmp(argv[1], "-m") == 0) {
        dict = dict_create_map(string_cmp);
    } else {
        usage(argv[0]);
    }

    FILE *file;
    if (strcmp(argv[2], "-") == 0) {
        file = stdin;
    } else {
        file = fopen(argv[2], "r");
        if (file == NULL) {
            printf("cannot open '%s'\n", argv[2]);
            usage(argv[0]);
        }
    }

    index_file(file, dict);
    fclose(file);

    dict_walk(dict, print_group, NULL);
    dict_walk(dict, free_group, NULL);

    dict_free(dict);
    return EXIT_SUCCESS;
}

noreturn void usage(const char *name)
{
    fprintf(stderr, "usage: %s [-t|-m] <file>\n", name);
    fprintf(stderr, "\t-t\tUse hash tables as the dictionary\n");
    fprintf(stderr, "\t-m\tUse binary search tree as the dictionary\n");
    exit(EXIT_FAILURE);
}

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

void index_file(FILE *file, struct dict *dict)
{
    struct record rec;

    while (read_record(file, &rec)) {
        char *hometown = strdup(rec.hometown);
        char *fullname = strdup(rec.fullname);

        add_entry(dict, hometown, fullname);
    }
}

void add_entry(struct dict *m, char *hometown, char *name)
{
    struct alist *list = dict_get(m, hometown);

    if (list == NULL) {
        list = alist_create();
        dict_insert(m, hometown, list);
    } else {
        free(hometown);
    }

    alist_append(list, name);
}

void print_group(void *key, void *value, void *data)
{
    (void) data;

    char *hometown = key;
    struct alist *list = value;

    int len = alist_len(list);
    if (len < 2) {
        return;
    }

    printf("%s:\n", hometown);
    for (int i = 0; i < len; i++) {
        char *name = *alist_at(list, i);
        printf("%s\n", name);
    }
    putchar('\n');
}

void free_group(void *key, void *value, void *data)
{
    (void) data;

    struct alist *list = value;

    int len = alist_len(list);
    for (int i = 0; i < len; i++) {
        char *name = *alist_at(list, i);
        free(name);
    }
    alist_free(list);
    free(key);
}

