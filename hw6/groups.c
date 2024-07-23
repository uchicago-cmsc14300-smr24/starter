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

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    printf("This file will be posted after the late deadline.\n");
    return EXIT_SUCCESS;
}
