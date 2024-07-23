#ifndef MAP_H_
#define MAP_H_

#include <stdio.h>

/* definition of tree node */
struct tree_node {
    void *key;               /* the key in this node   */
    void *value;             /* the value in this node */
    struct tree_node *left;  /* pointer to the left subtree  */
    struct tree_node *right; /* pointer to the right subtree */
};

/* definition of the map structure */
struct map {
    int (*cmp)(void *, void *); /* function pointer to compare keys */
    struct tree_node *root;     /* pointer to the root of the BST   */
};

/* map_create: creates a new map.
 *
 * cmp: comparison function. Should return a negative value if the first
 *      argument is less than the second, zero if they are equal, and
 *      a positive value if the first argument is greater than the second.
 * return: Pointer to newly created map. NULL if it fails to allocate memory.
 */
struct map *map_create(int (*cmp)(void *, void *));

/* map_free: frees a map.
 *
 * m: map to be freed.
 */
void map_free(struct map *m);

/* map_get: gets the value of a given key in the map.
 *
 * m: pointer to the map
 * key: pointer to the key
 * return: pointer to the value of the given key. NULL if the key does not
 *         exist in the map.
 */
void *map_get(struct map *m, void *key);

/* map_insert: inserts a key-value pair into the map. If an equal key already
 * exists in the map, the existing value will be replaced with the new one.

 * m: pointer to the map
 * key: pointer to the key. `key` cannot be NULL.
 * value: pointer to the value. `value` cannot be NULL.
 * return: the replaced value if the key already exists in the map;
 *         NULL otherwise
 */
void *map_insert(struct map *m, void *key, void *value);

/* map_remove: removes a key-value pair from the map.
 *
 * m: pointer to the map
 * key: pointer to the key to remove
 * return: pointer to the value of the removed key. NULL if the key does not
 * exist in the map.
 */
void *map_remove(struct map *m, void *key);

/* map_print: prints the map to the given file pointer.
 *
 * m: pointer to the map
 * fp: file pointer to print to
 */
void map_print(struct map *m, FILE *fp);

/* map_print_internal: prints the internal structure of the map to the given
 * file pointer for debugging.
 *
 * m: pointer to the map
 * fp: file pointer to print to
 */
void map_print_internal(struct map *m, FILE *fp);

/* map_walk: walks through the map and applies the visit function to each
 * key-value pair.
 *
 * m: pointer to the map
 * visit: a function pointer that takes a key, value and data and performs some
 *        operation
 * data: pointer to additional data that visit might need, data will be passed
 *       back to `visit`
 */
void map_walk(struct map *m,
                void (*visit)(void *key, void *value, void *data),
                void *data);

#endif
