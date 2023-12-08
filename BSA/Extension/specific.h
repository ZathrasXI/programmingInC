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
} Node;

struct bsa
{
    Node *head[BSA_ROWS];
    int max_index;
};

int _get_row(int index);
Node *_new_node(int i, int d);
Node *_insert(Node *, int i, int d);
void _free_tree(Node *n);