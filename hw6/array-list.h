#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <stdio.h>
#include <stdbool.h>

struct alist {
        int length;
        int capacity;
        void **elems;
};

/* create an empty array list */
struct alist *alist_create(void);

/* recycle the array list */
void alist_free(struct alist *l);

/* make a new copy of the list */
struct alist *alist_copy(const struct alist *l);

/* add the elements to the back of the array list */
static inline void alist_append(struct alist *l, void *elem);

/* add the elements to the front of the array list */
void alist_prepend(struct alist *l, void *elem);

/* return the length of the list */
static inline int alist_len(struct alist *l);

/* return the capacity of the list */
int alist_capacity(struct alist *l);

/* insert the elements between (i-1)-th and i-th element; the inserted
 * element has index i
 */
void alist_insert_at(struct alist *l, int i, void *elem);

/* remove the elements at i, returning the removed element */
void *alist_remove_at(struct alist *l, int i);

/* print the entire list */
void alist_print(struct alist *l, FILE *file);

/* return non zero if the list is empty */
bool alist_is_empty(struct alist *l);

/* return a reference to the i-th element */
void **alist_at(struct alist *l, int i);

/* definition inlining */
#include <assert.h>
#include <stdlib.h>

static inline void alist_append(struct alist *l, void *elem)
{
        assert(l != NULL && "l is null");

        if (l->length >= l->capacity) {
                l->capacity *= 2;
                l->elems = realloc(l->elems, l->capacity * sizeof(*l->elems));
        }

        l->elems[l->length] = elem;
        l->length += 1;
}

static inline int alist_len(struct alist *l)
{
        assert(l != NULL && "l is null");

        return l->length;
}


#endif
