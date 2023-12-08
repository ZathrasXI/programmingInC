#include "../bsa.h"
#include <assert.h>

#define INIT_SIZE 1
#define NOT_SET -1


typedef struct node
{
    int index;
    int value;
    struct node *left;
    struct node *right;
    int height;
} Node;

struct bsa
{
    Node *head[BSA_ROWS];
    int max_index;
};
