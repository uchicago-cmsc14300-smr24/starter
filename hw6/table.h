#ifndef TABLE_H_
#define TABLE_H_

#include <stdint.h>
#include <stdio.h>

/* the internal node of a table whose definition is hidden */
struct table;

/* table_create: create a new table
 *
 * hint_size: the expected size of this table
 * eq: equality function. Should return zero if two keys are the same and
 *     non-zero otherwise.
 * hash: calculate the hash of a given key
 * return: pointer to newly created map.
 */
struct table *table_create(int hint_size,
                int (*cmp)(void *, void *),
                uint64_t (*hash)(void *key));

/* table_free: frees a table
 *
 * t: table to be freed.
 */
void table_free(struct table *t);

/* table_get: gets the value of a given key in the table
 *
 * t: pointer to the table
 * key: pointer to the key
 * return: pointer to the value of the given key. NULL if the key does not exist
 *         in the table
 */
void *table_get(struct table *t, void *key);

/* table_insert: inserts a key-value pair into the table. If an equal key
 * already exists in the table, the existing value will be replaced with the
 * new one.

 * t: pointer to the table
 * key: pointer to the key. `key` cannot be NULL.
 * value: pointer to the value. `value` cannot be NULL.
 * return: the replaced value if the key already exists in the table;
 *         NULL otherwise
 */
void *table_insert(struct table *t, void *key, void *value);

/* table_remove: removes a key-value pair from the table.
 *
 * t: pointer to the table
 * key: pointer to the key to remove
 * return: pointer to the value of the removed key. NULL if the key does not
 * exist in the table.
 */
void *table_remove(struct table *t, void *key);

/* table_print: prints the table to the given file pointer.
 *
 * t: pointer to the table
 * fp: file pointer to print to
 */
void table_print(struct table *t, FILE *fp);

/* table_length: get the number of key-value pairs in the table.
 *
 * t: pointer to the table
 */
int table_length(struct table *t);

/* table_walk: applies the visit function to each key-value pair in ascending
 * order of the keys.
 *
 * t: pointer to the table
 * visit: a function pointer that takes a key, value and data and performs some
 *        operation
 * data: pointer to additional data that visit might need, data will be passed
 *       back to `visit`
 */
void table_walk(struct table *t,
                  void (*visit)(void *key, void *value, void *data),
                  void *data);

/* table_walk_unordered: applies the visit function to each key-value pair in
 * no particular order.
 *
 * t: pointer to the table
 * visit: a function pointer that takes a key, value and data and performs some
 *        operation
 * data: pointer to additional data that visit might need, data will be passed
 *       back to `visit`
 */
void table_walk_unordered(struct table *t,
                void (*visit)(void *key, void *value, void *data), void *data);

#endif
