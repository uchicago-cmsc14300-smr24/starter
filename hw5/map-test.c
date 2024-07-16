#include "map.h"
#include "tests.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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

static int strcmp_(void *s1, void *s2)
{
    return strcmp(s1, s2);
}

/******************************************************************************/
/*                                  GROUP 1                                   */
/*                                Simple tests                                */
/******************************************************************************/

/* create a map and immediately free it */
static void new_free(void)
{
    struct map *m = map_create(strcmp_);
    map_free(m);
}

/* insert a value and retrieve it */
static void insert_get(void)
{
    struct map *m = map_create(strcmp_);

    /* insert "alice" with value 42 into the map */
    void *old = map_insert(m, "alice", _p(42));
    expect_null(old); // There is no "old" value with this key

    /* lookup "alice" */
    void *value = map_get(m, "alice");
    expect_eq(42, _i(value));

    map_free(m);
}

/* insert a value on the root's left */
static void insert_left(void)
{
    struct map *m = map_create(strcmp_);

    map_insert(m, "bob", _p(52));
    map_insert(m, "alice", _p(42)); // "alice" should be on "bob"'s left

    void *value = map_get(m, "alice");
    expect_eq(42, _i(value));

    value = map_get(m, "bob");
    expect_eq(52, _i(value));

    map_free(m);
}

/* insert a value on the root's right */
static void insert_right(void)
{
    struct map *m = map_create(strcmp_);

    map_insert(m, "alice", _p(42));
    map_insert(m, "bob", _p(52)); // "bob" should be on "alice"'s right

    void *value = map_get(m, "alice");
    expect_eq(42, _i(value));

    value = map_get(m, "bob");
    expect_eq(52, _i(value));

    map_free(m);
}

/* insert a duplicate key */
static void insert_insert(void)
{
    struct map *m = map_create(strcmp_);

    map_insert(m, "alice", _p(42));             // insert "alice"
    void *old = map_insert(m, "alice", _p(52)); // insert "alice" again
    expect_eq(42, _i(old));

    void *value = map_get(m, "alice");  // lookup "alice"
    expect_eq(52, _i(value));

    map_free(m);
}

/* insert and then remove */
static void insert_remove(void)
{
    struct map *m = map_create(strcmp_);
    map_insert(m, "alice", _p(42));
    void *value = map_remove(m, "alice");

    expect_eq(42, _i(value)); // expect 42 to be returned

    value = map_get(m, "alice"); // looking up "alice" should be NULL
    expect_null(value);

    map_free(m);
}

/* lookup a non-existent key */
static void get_empty(void)
{
    struct map *m = map_create(strcmp_);
    void *value = map_get(m, "alice");

    expect_null(value);

    map_free(m);
}

/* remove a non-existent key */
static void remove_empty(void)
{
    struct map *m = map_create(strcmp_);
    void *value = map_remove(m, "alice");

    expect_null(value);

    map_free(m);
}

static void remove_left_child(void)
{
    struct map *m = map_create(strcmp_);
    map_insert(m, "bob", _p(1));
    map_insert(m, "alice", _p(2));

    void *v = map_remove(m, "alice");
    expect_eq(2, _i(v));

    expect_eq(1, _i(map_get(m, "bob")));
    map_free(m);
}

static void remove_right_child(void)
{
    struct map *m = map_create(strcmp_);
    map_insert(m, "alice", _p(2));
    map_insert(m, "bob", _p(1));

    void *v = map_remove(m, "bob");
    expect_eq(1, _i(v));

    expect_eq(2, _i(map_get(m, "alice")));
    map_free(m);
}

static void remove_root(void)
{
    struct map *m = map_create(strcmp_);
    map_insert(m, "bob", _p(1));
    map_insert(m, "alice", _p(2));
    map_insert(m, "carol", _p(3));

    void *v = map_remove(m, "alice");
    expect_eq(2, _i(v));

    expect_eq(1, _i(map_get(m, "bob")));
    expect_eq(3, _i(map_get(m, "carol")));

    map_free(m);
}

static void remove_all(void)
{
    struct map *m = map_create(strcmp_);
    map_insert(m, "bob", _p(1));
    map_insert(m, "alice", _p(2));
    map_insert(m, "carol", _p(3));

    void *v = map_remove(m, "alice");
    expect_eq(2, _i(v));

    v = map_remove(m, "bob");
    expect_eq(1, _i(v));

    v = map_remove(m, "carol");
    expect_eq(3, _i(v));

    map_free(m);
}

/******************************************************************************/
/*                                  GROUP 2                                   */
/*                                Stress tests                                */
/******************************************************************************/
static void large_insert(void)
{
    const int n = 10000;
    char **strs = mk_random_strs(n);
    struct map *m = map_create(strcmp_);

    for (int i = 0; i < n; i++) {
        void *old = map_insert(m, strs[i], _p(i + 1));
        if (old != NULL) {
            int old_idx = _i(old) - 1;
            free(strs[old_idx]);
            strs[old_idx] = NULL;
        }
    }

    for (int i = 0; i < n; i++) {
        if (strs[i] == NULL) {
            continue;
        }
        void *value = map_get(m, strs[i]);
        expect_non_null(value);
        expect_eq(i + 1, _i(value));
    }

    for (int i = 0; i < n; i++) {
        free(strs[i]);
    }
    free(strs);

    map_free(m);
}

static void large_remove(void)
{
    const int n = 10000;
    char **strs = mk_random_strs(n);
    struct map *m = map_create(strcmp_);

    for (int i = 0; i < n; i++) {
        void *old = map_insert(m, strs[i], _p(i + 1));
        if (old != NULL) {
            int old_idx = _i(old) - 1;
            free(strs[old_idx]);
            strs[old_idx] = NULL;
        }
    }

    for (int i = n - 1; i >= 0; i--) {
        if (strs[i] == NULL) {
            continue;
        }
        void *value = map_remove(m, strs[i]);
        expect_non_null(value);
        expect_eq(i + 1, _i(value));
    }

    for (int i = 0; i < n; i++) {
        free(strs[i]);
    }
    free(strs);

    map_free(m);
}

struct unittest tests[] = {
    Test(new_free),
    Test(insert_get),
    Test(insert_left),
    Test(insert_right),
    Test(insert_insert),
    Test(insert_remove),
    Test(get_empty),
    Test(remove_empty),
    Test(remove_left_child),
    Test(remove_right_child),
    Test(remove_root),
    Test(remove_all),
    Test(large_insert),
    Test(large_remove),
};

const int n_tests = sizeof(tests) / sizeof(tests[0]);

int main(int argc, char *argv[])
{
    return test_main(argc, argv, tests, n_tests);
}

