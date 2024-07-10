#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <stdio.h>

struct llist {
    void *elem;
    struct llist *next;
};

/*
 * The function accepts a variable number of arguments,
 * all of which are of type void *. The last argument must be NULL.
 * Each argument is added as an element to the linked list in the order
 * it was given.
 */
struct llist *llist_list(void *x, ...);

/* llist_free: Free the memory used by the linked list. */
void          llist_free(struct llist *l);

/* llist_append: Add a new element to the end of the linked list.
 *
 * l: The linked list to add the element to.
 * elem: The element to be added.
 * return: A pointer to the linked list with the new element added.
 */
struct llist *llist_append(struct llist *l, void *elem);

/* llist_prepend: Add a new element to the start of the linked list.
 *
 * l: The linked list to add the element to.
 * elem: The element to be added.
 * return: A pointer to the linked list with the new element added at the start.
 */
struct llist *llist_prepend(struct llist *l, void *elem);

/* llist_concat: Concatenate two linked lists.
 *
 * l: The first linked list.
 * tail: The second linked list.
 * return: A pointer to the concatenated linked list.
 */
struct llist *llist_concat(struct llist *l, struct llist *tail);

/* llist_copy: Make a copy of a linked list. */
struct llist *llist_copy(struct llist *l);

/* llist_print: Print the entire list to file */
void          llist_print(struct llist *l, FILE *file);

/* llist_reverse: Reverse the order of elements in the linked list.
 *
 * l: The linked list to be reversed.
 * return: A pointer to the reversed linked list.
 */
struct llist *llist_reverse(struct llist *l);

/* llist_insert_at: Insert an element at a given position in the linked list.
 *
 * l: The linked list to insert the element into.
 * i: The index at which to insert the element (0-based).
 * elem: The element to be inserted.
 * return: A pointer to the linked list with the new element inserted.
 */
struct llist *llist_insert_at(struct llist *l, int i, void *elem);

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
struct llist *llist_remove_at(struct llist *l, int i, void **elem_p);

/* llist_len: Calculate the length of the linked list. */
int           llist_len(struct llist *l);

#endif
