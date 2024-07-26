#ifndef DICT_H_
#define DICT_H_

#include <stdint.h>

/* An abstract dictionary whose definition is hidden */
struct dict;

/* Create a dictionary using hash tables.
 *
 * The function should be called with the same arguments as `table_create`.
 * */
struct dict *dict_create_table(int hint_size, int (*cmp)(void *, void *),
        uint64_t (*hash)(void *key));

/* Create a dictionary using binary search trees.
 *
 * The function should be called with the same arguments as `map_create`.
 * */
struct dict *dict_create_map(int (*cmp)(void *, void *));

/* Frees the dictionary */
void dict_free(struct dict *dict);

/* Look up the value of a given key */
void *dict_get(struct dict *dict, void *key);

/* Insert a key-value pair into the table */
void *dict_insert(struct dict *dict, void *key, void *value);

/* Applies the visit function to each key-value pairs, in the ascending order
 * of keys. */
void dict_walk(struct dict *dict,
        void (*visit)(void *key, void *value, void *data),
        void *data);

/* Applies the visit function to each key-value pairs in no guaranteed order */
void dict_walk_unordered(struct dict *dict,
        void (*visit)(void *key, void *value, void *data),
        void *data);
#endif
