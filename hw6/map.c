/* implementation of the map module using binary search tree */
#include "map.h"
#include "array-list.h"

#include <assert.h>
#include <stdlib.h>

/* helper function: find a reference to the leftmost node in the sub tree */
static struct tree_node **find_leftmost_ref(struct tree_node **root);

/* helper function: print the tree to fp with indentation */
static void map_print_node(struct tree_node *root, int indent, FILE *fp);

struct map *map_create(int (*cmp)(void *, void *))
{
    assert(cmp != NULL);

    struct map *m = malloc(sizeof(*m));
    m->cmp = cmp;
    m->root = NULL;

    return m;
}

void map_free(struct map *m)
{
    assert(m != NULL);

    struct alist *todo = alist_create();

    struct tree_node *root = m->root;
    if (root != NULL) {
        alist_append(todo, root);
    }

    while (!alist_is_empty(todo)) {
        int len = alist_len(todo);
        struct tree_node *curr = alist_remove_at(todo, len - 1);

        if (curr->left != NULL) {
            alist_append(todo, curr->left);
        }
        if (curr->right != NULL) {
            alist_append(todo, curr->right);
        }
        free(curr);
    }

    alist_free(todo);
    free(m);
}

void *map_get(struct map *m, void *key)
{
    assert(m != NULL && key != NULL);

    struct tree_node *curr = m->root;
    while (curr != NULL) {
        int cmp_result = m->cmp(key, curr->key);
        if (cmp_result == 0) {
            return curr->value;
        } else if (cmp_result < 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    return NULL;
}

void *map_insert(struct map *m, void *key, void *value)
{
    assert(m != NULL && key != NULL && value != NULL);

    struct tree_node **tree_p = &m->root;
    while (*tree_p != NULL) {
        int cmp_result = m->cmp(key, (*tree_p)->key);

        if (cmp_result == 0) {
            void *old_value = (*tree_p)->value;
            (*tree_p)->value = value;
            return old_value;
        } else if (cmp_result < 0) {
            tree_p = &(*tree_p)->left;
        } else {
            tree_p = &(*tree_p)->right;
        }
    }

    struct tree_node *r = malloc(sizeof(*r));
    r->key = key;
    r->value = value;
    r->left = NULL;
    r->right = NULL;

    *tree_p = r;
    return NULL;
}

void *map_remove(struct map *m, void *key)
{
    assert(m != NULL && key != NULL);

    struct tree_node **tree_p = &m->root;

    while (*tree_p != NULL) {
        int cmp_result = m->cmp(key, (*tree_p)->key);

        if (cmp_result == 0) {
            break;
        } else if (cmp_result < 0) {
            tree_p = &(*tree_p)->left;
        } else {
            tree_p = &(*tree_p)->right;
        }
    }

    if (*tree_p == NULL) {
        /* key does not exist in the tree */
        return NULL;
    }

    void *old_value = (*tree_p)->value;

    if ((*tree_p)->left == NULL) {
        /* if root has only right child or no children */
        struct tree_node *tmp = (*tree_p)->right;
        free(*tree_p);
        *tree_p = tmp;
    } else if ((*tree_p)->right == NULL) {
        /* if root has only left child */
        struct tree_node *tmp = (*tree_p)->left;
        free(*tree_p);
        *tree_p = tmp;
    } else {
        /* if the root has both children, we find the leftmost node of
         * the right sub-tree. This will give us the closest node to
         * root. We will then replace the node with the closest node,
         * and free the node */
        struct tree_node *tree = *tree_p;
        struct tree_node **closest_p = find_leftmost_ref(&tree->right);

        tree->key   = (*closest_p)->key;
        tree->value = (*closest_p)->value;

        free(*closest_p);
        *closest_p = (*closest_p)->right;
    }

    return old_value;
}

static void print_kv(void *key, void *value, void *data)
{
    FILE *fp = data;
    fprintf(fp, "%p -> %p\n", key, value);
}


void map_print(struct map *m, FILE *fp)
{
    map_walk(m, print_kv, fp);
}


void map_print_internal(struct map *m, FILE *fp)
{
    assert(m != NULL && fp != NULL);
    map_print_node(m->root, 0, fp);
}

void map_walk(struct map *m,
        void (*visit)(void *key, void *value, void *data),
        void *data)
{
    assert(m != NULL && visit != NULL);

    struct tree_node *curr = m->root;
    struct alist *todo = alist_create();

    while (!alist_is_empty(todo) || curr != NULL) {
        if (curr != NULL) {
            alist_append(todo, curr);
            curr = curr->left;
        } else {
            int len = alist_len(todo);
            curr = alist_remove_at(todo, len - 1);

            visit(curr->key, curr->value, data);
            curr = curr->right;
        }
    }

    alist_free(todo);
}

static struct tree_node **find_leftmost_ref(struct tree_node **root_p)
{
    for (; (*root_p)->left != NULL; root_p = &(*root_p)->left);
    return root_p;
}

static void map_print_node(struct tree_node *root, int indent, FILE *fp)
{
    if (root == NULL) {
        return;
    }

    for (int i = 0; i < indent * 4; i++) {
        fputc(' ', fp);
    }
    map_print_node(root->right, indent + 1, fp);
    fprintf(fp, "%p -> %p\n", root->key, root->value);
    map_print_node(root->left, indent + 1, fp);
}
