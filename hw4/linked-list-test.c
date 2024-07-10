/* unit-testing for linked-list */

#include "linked-list.h"
#include "tests.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

/* Fake a pointer using an integer. */
static void *_p(int i)
{
    assert(i != 0 && "0 is a NULL pointer");
    return (void *)(intptr_t) i;
}

/* Convert the fake pointer back to an integer */
static int _i(void *p)
{
    return (int)(uintptr_t) p;
}

/******************************************************************************/
/*                                  GROUP 1                                   */
/*  These tests should pass after implementing llist_free. Don't forget to    */
/*  check for memory leak.                                                    */
/******************************************************************************/

static void simple_create(void)
{
    struct llist *list = llist_list(_p(1), _p(2), _p(3), NULL);

    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem)); // expect the two values to be equal
        i++;
    }

    llist_free(list);
}

static void test_reverse(void)
{
    struct llist *list = llist_list(_p(3), _p(2), _p(1), NULL);
    list = llist_reverse(list);
    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list);
}

static void test_reverse_null(void)
{
    struct llist *list = NULL;
    list = llist_reverse(list);
    expect_eq(0, llist_len(list));
}

static void test_copy(void)
{
    struct llist *list1 = llist_list(_p(1), _p(2), NULL);
    struct llist *list2 = llist_copy(list1);

    int i = 1;
    for (struct llist *head = list2; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list1);
    llist_free(list2);
}

static void test_copy_null(void)
{
    struct llist *list1 = NULL;
    struct llist *list2 = llist_copy(list1);

    expect_eq(0, llist_len(list1));
    expect_eq(0, llist_len(list2));
}

/******************************************************************************/
/*                                  GROUP 2                                   */
/*  These tests should pass after implementing llist_prepend.                 */
/******************************************************************************/

static void test_prepend(void)
{
    struct llist *list = llist_list(_p(2), _p(3), _p(4), NULL);
    list = llist_prepend(list, _p(1));
    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }

    llist_free(list);
}

static void test_prepend_null(void)
{
    struct llist *list = llist_prepend(NULL, _p(1));
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(1, _i(head->elem));
    }

    llist_free(list);
}

/******************************************************************************/
/*                                  GROUP 3                                   */
/*  These tests should pass after implementing llist_len.                     */
/******************************************************************************/

static void test_len(void)
{
    struct llist *list = llist_list(_p(1), _p(2), _p(3), NULL);
    expect_eq(3, llist_len(list));
    llist_free(list);
}

static void test_len_null(void)
{
    expect_eq(0, llist_len(NULL));
}

/******************************************************************************/
/*                                  GROUP 4                                   */
/*  These tests should pass after implementing llist_concat.                  */
/******************************************************************************/

static void test_concat(void)
{
    struct llist *list1 = llist_list(_p(1), _p(2), NULL);
    struct llist *list2 = llist_list(_p(3), _p(4), NULL);

    struct llist *list3 = llist_concat(list1, list2);
    int i = 1;
    for (struct llist *head = list3; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list3);
}

static void test_concat_left_null(void)
{
    struct llist *list1 = NULL;
    struct llist *list2 = llist_list(_p(1), _p(2), NULL);

    struct llist *list3 = llist_concat(list1, list2);
    int i = 1;
    for (struct llist *head = list3; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list3);
}

static void test_concat_right_null(void)
{
    struct llist *list1 = llist_list(_p(1), _p(2), NULL);
    struct llist *list2 = NULL;

    struct llist *list3 = llist_concat(list1, list2);
    int i = 1;
    for (struct llist *head = list3; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list3);
}

/******************************************************************************/
/*                                  GROUP 5                                   */
/*  These tests should pass after implementing llist_append.                  */
/******************************************************************************/

static void test_append(void)
{
    struct llist *list = llist_list(_p(1), _p(2), _p(3), NULL);
    list = llist_append(list, _p(4));
    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }

    llist_free(list);
}

static void test_append_null(void)
{
    struct llist *list = llist_append(NULL, _p(4));
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(4, _i(head->elem));
    }

    llist_free(list);
}

/******************************************************************************/
/*                                  GROUP 6                                   */
/*  These tests should pass after implementing llist_remove_at.               */
/******************************************************************************/

static void test_remove_start(void)
{
    struct llist *list = llist_list(_p(1), _p(1), _p(2), _p(3), NULL);
    list = llist_remove_at(list, 0, NULL);
    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list);
}

static void test_remove_end(void)
{
    struct llist *list = llist_list(_p(1), _p(2), _p(3), _p(1), NULL);
    list = llist_remove_at(list, 3, NULL);
    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list);
}

static void test_remove_mid(void)
{
    struct llist *list = llist_list(_p(1), _p(2), _p(1), _p(3), NULL);
    list = llist_remove_at(list, 2, NULL);
    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list);
}

static void test_remove_non_null(void)
{
    struct llist *list = llist_list(_p(1), _p(1), _p(2), _p(3), NULL);
    void *elem;
    list = llist_remove_at(list, 0, &elem);
    expect_eq(1, _i(elem));

    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list);
}

/******************************************************************************/
/*                                  GROUP 7                                   */
/*  These tests should pass after implementing llist_insert_at.               */
/******************************************************************************/

static void test_insert_start(void)
{
    struct llist *list = llist_list(_p(2), _p(3), NULL);

    list = llist_insert_at(list, 0, _p(1));

    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list);
}

static void test_insert_mid(void)
{
    struct llist *list = llist_list(_p(1), _p(3), NULL);

    list = llist_insert_at(list, 1, _p(2));

    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list);
}

static void test_insert_end(void)
{
    struct llist *list = llist_list(_p(1), _p(2), NULL);

    list = llist_insert_at(list, 2, _p(3));

    int i = 1;
    for (struct llist *head = list; head != NULL; head = head->next) {
        expect_eq(i, _i(head->elem));
        i++;
    }
    llist_free(list);
}

/******************************************************************************/
/*                           END OF PROVIDED TESTS                            */
/******************************************************************************/

struct unittest tests[] = {
    /* 1. implement llist_free */
    Test(simple_create),
    Test(test_reverse),
    Test(test_reverse_null),
    Test(test_copy),
    Test(test_copy_null),

    /* 2. implement llist_prepend */
    Test(test_prepend),
    Test(test_prepend_null),

    /* 3. implement llist_len */
    Test(test_len),
    Test(test_len_null),

    /* 4. implement llist_concat */
    Test(test_concat),
    Test(test_concat_left_null),
    Test(test_concat_right_null),

    /* 5. implement llist_append */
    Test(test_append),
    Test(test_append_null),

    /* 6. implement llst_remove_at */
    Test(test_remove_start),
    Test(test_remove_mid),
    Test(test_remove_end),
    Test(test_remove_non_null),

    /* 7. implement llst_insert_at */
    Test(test_insert_start),
    Test(test_insert_mid),
    Test(test_insert_end),

    /* YOU CAN ADD MORE TESTS HERE */
};

const int n_tests = sizeof(tests) / sizeof(tests[0]);

int main(int argc, char *argv[])
{
    return test_main(argc, argv, tests, n_tests);
}

