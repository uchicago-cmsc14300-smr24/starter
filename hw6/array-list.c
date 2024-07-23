#include "array-list.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct alist *alist_create(void)
{
        struct alist *l = malloc(sizeof(*l));
        l->length = 0;
        l->capacity = 4;
        l->elems = malloc(l->capacity * sizeof(*l->elems));

        return l;
}

void alist_free(struct alist *l)
{
        assert(l != NULL);
        free(l->elems);
        free(l);
}

struct alist *alist_copy(const struct alist *other)
{
        assert(other != NULL);

        struct alist *l = malloc(sizeof(*l));
        l->length = other->length;
        l->capacity = other->capacity;
        l->elems = malloc(l->capacity * sizeof(*l->elems));

        memcpy(l->elems, other->elems, l->length * sizeof(*l->elems));

        return l;
}

void alist_prepend(struct alist *l, void *elem)
{
        alist_insert_at(l, 0, elem);
}

void alist_insert_at(struct alist *l, int i, void *elem)
{
        assert(l != NULL && "l is null");
        assert(0 <= i && i <= l->length);

        if (l->length >= l->capacity) {
                l->capacity *= 2;
                l->elems = realloc(l->elems, l->capacity * sizeof(*l->elems));
        }

        memmove(l->elems + i + 1, l->elems + i,
                        (l->length - i) * sizeof(*l->elems));
        l->elems[i] = elem;
        l->length += 1;
}

void *alist_remove_at(struct alist *l, int i)
{
        assert(l != NULL && "l is null");
        assert(0 <= i && i < l->length);

        void *old_elem = l->elems[i];

        memmove(l->elems + i,
                l->elems + i + 1,
                (l->length - i - 1) * sizeof(*l->elems));
        l->length -= 1;

        return old_elem;
}

void alist_print(struct alist *l, FILE *file)
{
        assert(l != NULL && file != NULL);

        for (int i = 0; i < l->length; i++) {
                fprintf(file, "l[%d] = %p\n", i, l->elems[i]);
        }
}

bool alist_is_empty(struct alist *l)
{
        return alist_len(l) == 0;
}

void **alist_at(struct alist *l, int i)
{
        assert(l != NULL);
        assert(0 <= i && i < l->length);

        return &l->elems[i];
}


