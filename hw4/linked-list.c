#include "linked-list.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * This function constructs a linked list containing all arguments passed to it.
 * The implementation involves the handling of variadic arguments in C. Although
 * you do not need to understand the implementation, you should know how a list
 * is constructed.
 *
 * struct llist *my_list = llist_list("hello", "world", NULL);
 *
 * The above line constructs a linked list with two nodes:
 *
 *             +---------+  +-->+---------+
 *             | "hello" |  |   | "world" |
 * my_list --> +---------+  |   +---------+
 *             | next    |--+   | next    |-->NULL
 *             +---------+      +---------+
 *
 * The element pointers are passed in and _not_ allocated in this function. Your
 * free function should not free it.
 */
struct llist *llist_list(void *x, ...)
{
    va_list args; // Representing the list of arguments
    struct llist *list = NULL;     // The head of linked list
    struct llist **curr_p = &list; // Pointing to the `next` field of the
                                   // previous list

    va_start(args, x);
    for(; x != NULL; x = va_arg(args, void *)) {
        *curr_p = malloc(sizeof(**curr_p));  // Allocate memory for a new node
        (*curr_p)->elem = x;                 // Write elem field
        curr_p = &(*curr_p)->next;           // Advance the curr_p pointer
    }
    va_end(args);

    *curr_p = NULL;
    return list;
}

/******************************************************************************/
/*                            Your Implementations                            */
/******************************************************************************/

/* llist_free: Free the memory used by the linked list.
 *   NOTE: elem field does not need to be freed.
 */
void llist_free(struct llist *l)
{
    (void) l;
    assert(0 && "TODO");
}

/* llist_append: Add a new element to the end of the linked list.
 *
 * l: The linked list to add the element to.
 * elem: The element to be added.
 * return: A pointer to the linked list with the new element added.
 */
struct llist *llist_append(struct llist *l, void *elem)
{
    (void) l, (void) elem;
    assert(0 && "TODO");
    return NULL;
}

/* llist_prepend: Add a new element to the start of the linked list.
 *
 * l: The linked list to add the element to.
 * elem: The element to be added.
 * return: A pointer to the linked list with the new element added at the start.
 */
struct llist *llist_prepend(struct llist *l, void *elem)
{
    (void) l, (void) elem;
    assert(0 && "TODO");
    return NULL;
}

/* llist_concat: Concatenate two linked lists.
 *
 * l: The first linked list.
 * tail: The second linked list.
 * return: A pointer to the concatenated linked list.
 */
struct llist *llist_concat(struct llist *l, struct llist *tail)
{
    (void) l, (void) tail;
    assert(0 && "TODO");
    return NULL;
}

/* llist_len: Calculate the length of the linked list. */
int llist_len(struct llist *l)
{
    (void) l;
    return 0;
}

/* llist_insert_at: Insert an element at a given position in the linked list.
 *
 * l: The linked list to insert the element into.
 * i: The index at which to insert the element (0-based).
 * elem: The element to be inserted.
 * return: A pointer to the linked list with the new element inserted.
 */
struct llist *llist_insert_at(struct llist *l, int i, void *elem)
{
    (void) l, (void) i, (void) elem;
    assert(0 && "TODO");
    return NULL;
}

/* llist_remove_at: Remove an element from a given position in the linked list.
 *
 * The function assigns the removed element to *elem_p and returns the
 * resulting list.
 *
 * l: The linked list to remove the element from.
 * i: The index from which to remove the element (0-based).
 * elem_p: A pointer to a pointer where to store the removed element. If elem_p
 *        is NULL, then this is ignored.
 * return: A pointer to the linked list with the element removed.
 */
struct llist *llist_remove_at(struct llist *l, int i, void **elem_p)
{
    (void) l, (void) i, (void) elem_p;
    assert(0 && "TODO");
    return NULL;
}

/******************************************************************************/
/*                          Provided Implementations                          */
/******************************************************************************/

/* llist_print: Print the entire list to file */
void llist_print(struct llist *l, FILE *file)
{
    for (; l != NULL; l = l->next) {
        fprintf(file, "%p->", l->elem);
    }
    fputs("NULL", file);
}

/* llist_copy: Make a copy of a linked list. */
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

/* llist_reverse: Reverse the order of elements in the linked list.
 *
 * l: The linked list to be reversed.
 * return: A pointer to the reversed linked list.
 */
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
