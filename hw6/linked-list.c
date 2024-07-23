#include "linked-list.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/* You don't need to understand the implementation of this function */
struct llist *llist_list(void *x, ...)
{
    va_list args;
    struct llist *list = NULL;
    struct llist **curr_p = &list;

    va_start(args, x);
    for(; x != NULL; x = va_arg(args, void *)) {
        *curr_p = malloc(sizeof(**curr_p));
        (*curr_p)->elem = x;
        curr_p = &(*curr_p)->next;
    }
    va_end(args);

    *curr_p = NULL;
    return list;
}

void llist_free(struct llist *l)
{
    while (l != NULL) {
        struct llist *next = l->next;
        free(l);
        l = next;
    }
}

struct llist *llist_append(struct llist *l, void *elem)
{
    return llist_concat(l, llist_list(elem, NULL));
}

struct llist *llist_prepend(struct llist *l, void *elem)
{
    struct llist *n = malloc(sizeof(*n));
    n->elem = elem;
    n->next = l;

    return n;
}

void llist_print(struct llist *l, FILE *file)
{
    for (; l != NULL; l = l->next) {
        fprintf(file, "%p->", l->elem);
    }
    fputs("NULL", file);
}


struct llist *llist_concat(struct llist *l, struct llist *tail)
{
    struct llist **p;
    for (p = &l; *p != NULL; p = &(*p)->next);
    *p = tail;

    return l;
}

struct llist *llist_copy(struct llist *l)
{
    struct llist *new;
    struct llist **parent = &new;

    for (struct llist *head = l; head != NULL; head = head->next) {
        *parent = malloc(sizeof(**parent));
        (*parent)->elem = head->elem;
        parent = &(*parent)->next;
    }

    *parent = NULL;
    return new;
}

struct llist *llist_reverse(struct llist *l)
{
    struct llist *prev = NULL, *curr = l, *next;

    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    return prev;
}

struct llist *llist_insert_at(struct llist *l, int i, void *elem)
{
    struct llist **curr_p = &l;
    for (int j = 0; j < i; j++) {
        if (*curr_p == NULL) {
            fprintf(stderr, "i out of range\n");
            abort();
        }
        curr_p = &(*curr_p)->next;
    }

    struct llist *node = malloc(sizeof(*node));
    node->elem = elem;
    node->next = *curr_p;
    *curr_p = node;

    return l;
}

/* return the resulting list, but assign the removed element to *elem_p */
struct llist *llist_remove_at(struct llist *l, int i, void **elem_p)
{
    struct llist **curr_p = &l;
    for (int j = 0; j < i; j++) {
        if (*curr_p == NULL) {
            fprintf(stderr, "i out of range\n");
            abort();
        }
        curr_p = &(*curr_p)->next;
    }

    if (*curr_p == NULL) {
        fprintf(stderr, "i out of range\n");
        abort();
    }

    struct llist *next = (*curr_p)->next;
    if (elem_p != NULL) {
        *elem_p = (*curr_p)->elem;
    }

    free(*curr_p);
    *curr_p = next;

    return l;
}

int llist_len(struct llist *l)
{
    int len = 0;
    for (struct llist *head = l; head != NULL; head = head->next) {
        len++;
    }
    return len;
}
