#include "table.h"
#include "tests.h"
#include "hash.h"

#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* convert an integer to a pointer */
static void *_p(int i)
{
    if (i == 0) {
        i = ~0;
    };

    return (void *)(intptr_t) i;
}

/* convert a pointer to an integer */
static int _i(void *p)
{
    int i = (int)(uintptr_t) p;
    if (~i == 0) {
        return 0;
    }
    return i;
}

static struct table *mktable(void)
{
    return table_create(0, string_cmp, string_hash);
}


static void new_free(void)
{
    struct table *t = mktable();
    table_free(t);
}

static void get_empty(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);
    void *val = table_get(t, "alice");
    expect_null(val);

    table_free(t);
}

static void remove_empty(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);
    void *val = table_remove(t, "alice");
    expect_null(val);

    table_free(t);
}

static void insert_get(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);
    table_insert(t, "alice", _p(42));
    expect_eq(1, table_length(t));
    void *val = table_get(t, "alice");
    expect_eq(42, _i(val));

    table_free(t);
}

static void insert_insert(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);
    table_insert(t, "alice", _p(42));
    expect_eq(1, table_length(t));
    void *old = table_insert(t, "alice", _p(52));
    expect_eq(1, table_length(t));
    expect_eq(42, _i(old));

    void *value = table_get(t, "alice");
    expect_eq(52, _i(value));

    table_free(t);
}

static void remove_get(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);
    table_insert(t, "alice", _p(42));
    expect_eq(1, table_length(t));
    void *old = table_remove(t, "alice");
    expect_eq(0, table_length(t));
    expect_eq(42, _i(old));

    void *value = table_get(t, "alice");
    expect_null(value);

    table_free(t);
}

static void remove_remove(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);
    table_insert(t, "alice", _p(42));
    void *old = table_remove(t, "alice");
    expect_eq(42, _i(old));

    void *value = table_remove(t, "alice");
    expect_null(value);

    table_free(t);
}

static void large_insert(void)
{
    const int n = 10000;
    char **strs = mk_random_strs(n);
    struct table *t = mktable();

    for (int i = 0; i < n; i++) {
        void *old = table_insert(t, strs[i], _p(i + 1));
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
        void *value = table_get(t, strs[i]);
        expect_non_null(value);
        expect_eq(i + 1, _i(value));
    }

    for (int i = 0; i < n; i++) {
        free(strs[i]);
    }
    free(strs);

    table_free(t);
}

/* these are a list of strings that will be hashed to 0 */
static char *COLLISIONS[] = {
    "\xed\xf5\x7e\x79\x3b\xfa\x16\x0c\xb3\xaf\x3e\x5f\xf3\xef\x03\x84\x80\x58\x2e\xe2\xfb\x67\x32\xbb\xdf\xcb\x07\xd2\x5c\x2f\x1f\x1f",
    "\x74\xd5\x32\x4e\x26\x0a\x55\xab\x35\x5f\x64\x2d\x1a\x75\x86\x2f\xd7\x69\x9c\xa7\x3a\x80\x59\xe0\x88\xea\x28\x5d\x2f\xe7\x1a\x5c",
    "\xb1\xbe\x1e\x67\xd4\xe4\x4f\x7a\xa3\x38\xf4\xd7\xc4\x4b\x26\xdf\x95\x67\xf5\x0d\x02\x1b\x43\xa0\x74\x0c\xdb\x5b\x2a\xdd\x79\xd8",
    "\x5f\xe2\x6e\x5a\x86\xad\xbd\xe7\x08\xe9\x9e\xd1\xa8\xbf\x4c\x30\x89\x8e\xa0\xd7\x21\x0b\xc1\x34\x68\x26\x76\x37\xb0\x8f\x8c\x43",
    "\x1d\x37\xa1\x76\xe4\xd0\x41\x42\x65\x24\x0d\xfa\x20\x16\x4d\x70\xd2\x6c\xb1\x5a\x8b\xac\x1e\x9d\xaf\x43\xd5\xaa\xd6\xe0\xdf\x35",
    "\xce\x94\xd4\xd7\x3e\xcb\x89\xde\x2b\x47\x88\x4b\xdb\x8e\x80\x9f\x83\x13\xd8\x41\x1b\x6e\x4e\xbc\x8f\xdb\xe1\x24\x6b\x49\x5d\xcf",
    "\xaf\x8c\x78\x47\x25\xde\x0d\xfc\x95\x12\xf9\x46\xa5\x64\x45\x01\xe5\xeb\x0a\x53\x42\x5f\xa9\x20\xb9\xc5\xe2\x5d\x9d\xd7\x90\xc8",
    "\x4d\xf4\xec\x59\x54\xe7\x96\x9a\x5a\x79\xe2\x39\xbf\x5d\x7c\x02\x20\xc1\xa2\x2f\x64\x8f\x4a\x4a\x87\xaa\x88\x90\x1b\x1e\xb9\xae",
    "\xff\x56\x81\xef\xd5\xcb\x23\x8c\xc0\xd9\x19\xfb\x07\x79\x5b\x27\xdb\x28\x15\xe8\xe2\xb7\x32\x75\x8d\xc0\x2e\x62\xb7\x31\x24\x2f",
};
static int N_COLLISIONS = sizeof(COLLISIONS) / sizeof(COLLISIONS[0]);

static void insert_collision(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);

    for (int i = 0; i < N_COLLISIONS; i++) {
        void *value = table_insert(t, COLLISIONS[i], _p(i));
        expect_null(value);
    }

    for (int i = 0; i < N_COLLISIONS; i++) {
        void *val = table_get(t, COLLISIONS[i]);
        expect_eq(i, _i(val));
    }

    table_free(t);
}

static void remove_first_collision(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);

    for (int i = 0; i < N_COLLISIONS; i++) {
        void *value = table_insert(t, COLLISIONS[i], _p(i));
        expect_null(value);
    }

    int idx_remove = 0;

    void *remove = table_remove(t, COLLISIONS[idx_remove]);
    expect_eq(idx_remove, _i(remove));

    for (int i = 0; i < N_COLLISIONS; i++) {
        if (i != idx_remove) {
            void *val = table_get(t, COLLISIONS[i]);
            expect_eq(i, _i(val));
        }
    }

    table_free(t);
}

static void remove_last_collision(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);

    for (int i = 0; i < N_COLLISIONS; i++) {
        void *value = table_insert(t, COLLISIONS[i], _p(i));
        expect_null(value);
    }

    int idx_remove = N_COLLISIONS - 1;

    void *remove = table_remove(t, COLLISIONS[idx_remove]);
    expect_eq(idx_remove, _i(remove));

    for (int i = 0; i < N_COLLISIONS; i++) {
        if (i != idx_remove) {
            void *val = table_get(t, COLLISIONS[i]);
            expect_eq(i, _i(val));
        }
    }

    table_free(t);
}

static void remove_mid_collision(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);

    for (int i = 0; i < N_COLLISIONS; i++) {
        void *value = table_insert(t, COLLISIONS[i], _p(i));
        expect_null(value);
    }

    int idx_remove = 3;

    void *remove = table_remove(t, COLLISIONS[idx_remove]);
    expect_eq(idx_remove, _i(remove));

    for (int i = 0; i < N_COLLISIONS; i++) {
        if (i != idx_remove) {
            void *val = table_get(t, COLLISIONS[i]);
            expect_eq(i, _i(val));
        }
    }

    table_free(t);
}

static void remove_all_collision(void)
{
    struct table *t = table_create(0, string_cmp, string_hash);

    for (int i = 0; i < N_COLLISIONS; i++) {
        void *value = table_insert(t, COLLISIONS[i], _p(i));
        expect_null(value);
    }

    for (int i = 0; i < N_COLLISIONS; i++) {
        void *val = table_remove(t, COLLISIONS[i]);
        expect_eq(i, _i(val));
    }

    for (int i = 0; i < N_COLLISIONS; i++) {
        void *val = table_get(t, COLLISIONS[i]);
        expect_null(val);
    }

    table_free(t);
}

struct unittest tests[] = {
    Test(new_free),
    Test(get_empty),
    Test(remove_empty),
    Test(insert_get),
    Test(insert_insert),
    Test(remove_get),
    Test(remove_remove),
    Test(insert_collision),
    Test(remove_first_collision),
    Test(remove_last_collision),
    Test(remove_mid_collision),
    Test(remove_all_collision),
    Test(large_insert),
};

const int n_tests = sizeof(tests) / sizeof(tests[0]);

int main(int argc, char *argv[])
{
    return test_main(argc, argv, tests, n_tests);
}

