/* implementation of the map module using binary search tree */
#include "map.h"
#include "array-list.h"

#include <assert.h>
#include <stdlib.h>

struct map *map_create(int (*cmp)(void *, void *))
{
    assert(cmp != NULL);

    struct map *m = malloc(sizeof(*m));
    m->cmp = cmp;   // store the comparison function
    m->root = NULL; // empty tree

    return m;
}

/******************************************************************************/
/*                            Your Implementations                            */
/******************************************************************************/

void map_free(struct map *m)
{
    assert(m != NULL);

    assert(0 && "TODO"); // DELETE THIS LINE
}

void *map_get(struct map *m, void *key)
{
    assert(m != NULL && key != NULL);

    assert(0 && "TODO"); // DELETE THIS LINE

    return NULL;
}

void *map_insert(struct map *m, void *key, void *value)
{
    assert(m != NULL && key != NULL && value != NULL);

    assert(0 && "TODO"); // DELETE THIS LINE

    return NULL;
}

/******************************************************************************/
/*                          Provided Implementations                          */
/******************************************************************************/

/* helper function: find a reference to the leftmost node in the sub tree */
static struct tree_node **find_leftmost_ref(struct tree_node **root);

/* helper function: print the tree to fp with indentation */
static void map_print_node(struct tree_node *root, int indent, FILE *fp);

/* map_remove: removes a key-value pair from the map.
 *
 * m: pointer to the map
 * key: pointer to the key to remove
 * return: pointer to the value of the removed key. NULL if the key does not
 * exist in the map.
 */
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

/* map_print: prints the map to the given file pointer.
 *
 * m: pointer to the map
 * fp: file pointer to print to
 */
void map_print(struct map *m, FILE *fp)
{
    map_walk(m, print_kv, fp);
}

/* map_print_internal: prints the internal structure of the map to the given
 * file pointer for debugging.
 *
 * m: pointer to the map
 * fp: file pointer to print to
 */
void map_print_internal(struct map *m, FILE *fp)
{
    assert(m != NULL && fp != NULL);
    map_print_node(m->root, 0, fp);
}

/* map_walk: walks through the map and applies the visit function to each
 * key-value pair.
 *
 * m: pointer to the map
 * visit: a function pointer that takes a key, value and data and performs some
 *        operation
 * data: pointer to additional data that visit might need, data will be passed
 *       back to `visit`
 */
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
