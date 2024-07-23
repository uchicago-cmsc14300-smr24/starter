#include "dict.h"

#include "map.h"
#include "table.h"

#include <assert.h>
#include <stdlib.h>

/* An enum indicating which implementation the dictionary uses */
enum dict_type {
    TABLE,
    MAP,
};

/* A union of two data */
union table_or_map {
    struct table *tbl;
    struct map *map;
};

/* Representation of an abstract dictionary, which is a tagged union of a table
 * and a map */
struct dict {
    enum dict_type type;
    union table_or_map data;
};

struct dict *dict_create_table(int hint_size, int (*cmp)(void *, void *),
        uint64_t (*hash)(void *key))
{
    (void) hint_size;
    (void) cmp;
    (void) hash;

    assert(0 && "TODO");
    return NULL;
}

struct dict *dict_create_map(int (*cmp)(void *, void *))
{
    (void) cmp;

    assert(0 && "TODO");
    return NULL;
}

void dict_free(struct dict *dict)
{
    (void) dict;

    assert(0 && "TODO");
    return NULL;
}

void *dict_get(struct dict *dict, void *key)
{
    (void) dict;

    assert(0 && "TODO");
    return NULL;
}

void *dict_insert(struct dict *dict, void *key, void *value)
{
    (void) dict;
    (void) key;
    (void) value;

    assert(0 && "TODO");
    return NULL;
}

void dict_walk(struct dict *dict,
        void (*visit)(void *key, void *value, void *data),
        void *data)
{
    if (dict->type == TABLE) {
        table_walk(dict->data.tbl, visit, data);
    } else {
        map_walk(dict->data.map, visit, data);
    }
}
