#include "hash.h"

#include <string.h>

int string_cmp(void *key1, void *key2)
{
    return strcmp(key1, key2);
}

uint64_t string_hash(void *key)
{
    /* djb2 hashing */
    char *str = key;
    uint64_t hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}
