#include "../bsa.h"
#include <assert.h>

#define INIT_SIZE 1
#define NOT_SET -1
#define MIN_INDEX 0

typedef struct node
{
    int index;
    int value;
    struct node *left;
    struct node *right;
    int *tree_size;
} Node;

struct bsa
{
    Node *head[BSA_ROWS];
    int max_index;
    int tree_sizes[BSA_ROWS];
};

int _get_row(int index);
Node *_new_node(int i, int d);
Node *_insert(Node *, int i, int d);
void _free_tree(Node *n);
void _reset_row(Node **n);
void _rm_node(Node **n, int i);
void _update_max_index(bsa *b, int indx);
void _run_func(void (*func)(int *p, int *n), Node *n, int *acc);


