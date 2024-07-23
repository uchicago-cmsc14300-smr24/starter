/* implementation of the table module */

#include "table.h"

#include <assert.h>
#include <limits.h>
#include <stdlib.h>

/* a list of prime numbers for selecting the initial size */
static const int PRIMES[] = {
    73, 179, 283, 419, 811, 1663, 3259, 6481, 12893, 25667, 51263, INT_MAX,
};

/* representation of buckets */
struct bucket {
    void *key;
    void *value;
    struct bucket *next;
};

/* internal representation of a table */
struct table {
    int size; /* the number of buckets in this table */
    int length; /* the number of key-value pairs */
    int (*cmp)(void *, void *); /* comparison between two keys */
    uint64_t (*hash)(void *);   /* hash a key */
    struct bucket *buckets[];   /* a variable array member for buckets */
};


struct table *table_create(int hint,
        int (*cmp)(void *, void *),
        uint64_t (*hash)(void *key))
{
    assert(hint >= 0);
    assert(cmp != NULL && hash != NULL);

    /* Look for the first prime number in the list that is less than the hint
     * since to minimize collision, the size of the table should be a prime
     * number */
    int i;
    for (i = 1; PRIMES[i] < hint; i++);
    int size = PRIMES[i - 1];

    /* Allocates all fields of the structure plus extra space for the buckets */
    struct table *t = malloc(sizeof(*t) + size * sizeof(t->buckets[0]));
    t->size = size;
    t->length = 0;
    t->cmp = cmp;
    t->hash = hash;

    for (i = 0; i < size; i++) {
        t->buckets[i] = NULL;
    }

    return t;
}

/******************************************************************************/
/*                            Your Implementations                            */
/******************************************************************************/

void table_free(struct table *t)
{
    (void) t;
    assert(0 && "TODO");
}

void *table_get(struct table *t, void *key)
{
    assert(t != NULL && key != NULL);

    assert(0 && "TODO");
    return NULL;
}

void *table_insert(struct table *t, void *key, void *value)
{
    assert(t != NULL && key != NULL && value != NULL);

    assert(0 && "TODO");
    return NULL;
}

/******************************************************************************/
/*                          Provided Implementations                          */
/******************************************************************************/

void *table_remove(struct table *t, void *key)
{
    assert(t != NULL && key != NULL);

    int idx = t->hash(key) % t->size;

    for (struct bucket **b_p = &t->buckets[idx]; *b_p != NULL;
            b_p = &(*b_p)->next) {
        if (t->cmp(key, (*b_p)->key) == 0) {
            void *old_value = (*b_p)->value;
            struct bucket *next = (*b_p)->next;
            free(*b_p);
            *b_p = next;
            t->length--;
            return old_value;
        }
    }

    return NULL;
}

int table_length(struct table *t)
{
    return t->length;
}

static void print_kv(void *key, void *value, void *data)
{
    FILE *fp = data;
    fprintf(fp, "%p -> %p\n", key, value);
}

void table_print(struct table *t, FILE *fp)
{
    table_walk(t, print_kv, fp);
}

static void quick_sort(void **arr, int len, int (*cmp)(void *, void *));

static void **sorted_keys(struct table *t)
{
    int length = table_length(t);

    void **keys = malloc(length * sizeof(void *));
    int len = 0;
    for (int i = 0; i < t->size; i++) {
        for (struct bucket *b = t->buckets[i]; b != NULL; b = b->next) {
            keys[len++] = b->key;
        }
    }

    assert(length == len);
    quick_sort(keys, len, t->cmp);

    return keys;
}

void table_walk(struct table *t,
        void (*visit)(void *key, void *value, void *data),
        void *data)
{
    assert(t != NULL && visit != NULL);

    void **keys = sorted_keys(t);
    int length = table_length(t);

    for (int i = 0; i < length; i++) {
        void *key = keys[i];
        void *value = table_get(t, key);

        visit(key, value, data);
    }

    free(keys);
}

/******************************************************************************/
/*                        Implementation of quick sort                        */
/******************************************************************************/

static inline void swap(void **arr, int i, int j)
{
    void *tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

static int partition(void **arr, int start, int end, int (*cmp)(void *, void *))
{
    void *pivot = arr[start];
    int next = end - 1;

    for (int i = end - 1; i > start; i--) {
        if (cmp(arr[i], pivot) > 0) {
            swap(arr, next--, i);
        }
    }

    swap(arr, start, next);
    return next;
}

static void quick_sort_impl(void **arr, int start, int end,
        int (*cmp)(void *, void *))
{
    if (end - start <= 1) {
        return;
    }

    int p = partition(arr, start, end, cmp);

    quick_sort_impl(arr, start, p, cmp);
    quick_sort_impl(arr, p + 1, end, cmp);
}

static void quick_sort(void **arr, int len, int (*cmp)(void *, void *))
{
    quick_sort_impl(arr, 0, len, cmp);
}

