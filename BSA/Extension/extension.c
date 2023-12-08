#include "specific.h"

int main(void)
{
    test();
    return 0;
}

bsa *bsa_init(void)
{
    bsa *new = (bsa *) calloc(INIT_SIZE, sizeof(bsa));
    if (new == NULL)
    {
        fprintf(stderr, "error allocating memory for new BSA");
        exit(EXIT_FAILURE);
    }
    new->max_index = NOT_SET;
    for (int i = 0 ; i < BSA_ROWS; i++)
    {
        new->head[i] = NULL;
    }
    return new;
}

bool bsa_set(bsa *b, int indx, int d)
{
    if (indx < MIN_INDEX)
    {
        return false;
    }

    int row = _get_row(indx);
    b->head[row] = _insert(b->head[row], indx, d);
    return true;
}

int _get_row(int index)
{
    int row = 0;
    index++;
    while (index >>= 1)
    {
        row++;
    }
    return row;
}

Node *_new_node(int i, int d)
{
    Node *new = (Node *) calloc(INIT_SIZE, sizeof(Node));
    if (new == NULL)
    {
        fprintf(stderr, "error allocating space for new node\n");
        exit(EXIT_FAILURE);
    }
    new->index = i;
    new->value = d;
    new->left = NULL;
    new->right = NULL;
    return new;
}

Node *_insert(Node *root, int i, int d)
{
    if (root == NULL)
    {
        root = _new_node(i, d);
    }
    else if (i > root->index)
    {
        root->right = _insert(root->right, i, d);
    }
    else if (i < root->index)
    {
        root->left = _insert(root->left, i, d);
    }
    else 
    {
        root->value = d;
    }

    return root;
}

void _free_tree(Node *n)
{
    if (n == NULL)
    {
        return;
    }
    _free_tree(n->left);
    _free_tree(n->right);
    free(n);
}

void test(void)
{
    /*
    can init new bsa
    */
    bsa *new_test = bsa_init();
    for (int i = 0 ; i < BSA_ROWS; i++)
    {
        assert(new_test->head[i] == NULL);
    }
    assert(new_test->max_index == NOT_SET);
    free(new_test);

    /*
    can insert value at index
    */
    bsa *set_test = bsa_init();
    //false when index out of bounds
    assert(!bsa_set(set_test, -1, 0));
    //can set node to NULL head 
    int row = _get_row(10);
    assert(bsa_set(set_test, 10, 20));
    assert(set_test->head[row]->index == 10);
    assert(set_test->head[row]->value == 20);
    //can create node with smaller index in same row
    assert(bsa_set(set_test, 9, 19));
    assert(set_test->head[row]->left->index == 9);
    assert(set_test->head[row]->left->value == 19);
    //can create node with larger index in same row
    assert(bsa_set(set_test, 11, 21));
    assert(set_test->head[row]->right->index == 11); 
    assert(set_test->head[row]->right->value == 21);
    //can create grandchild
    assert(bsa_set(set_test, 13, 23));
    assert(set_test->head[row]->right->right->index == 13); 
    assert(set_test->head[row]->right->right->value == 23);
    //can update first node
    assert(bsa_set(set_test, 10, 200));
    assert(set_test->head[row]->index == 10);
    assert(set_test->head[row]->value == 200);
    //can update child
    assert(bsa_set(set_test, 11, 211));
    assert(set_test->head[row]->right->index == 11); 
    assert(set_test->head[row]->right->value == 211);
    //can update grandchild
    assert(bsa_set(set_test, 13, 233));
    assert(set_test->head[row]->right->right->index == 13); 
    assert(set_test->head[row]->right->right->value == 233);  

    _free_tree(set_test->head[row]);
    free(set_test);

    /*
    can get row for index of array
    */
    assert(_get_row(0) == 0);
    assert(_get_row(1) == 1);
    assert(_get_row(2) == 1);
    assert(_get_row(29) == 4);
    assert(_get_row(536870912) == 29);

    /*
    can create new node
    */
    Node *new = _new_node(10, 20);
    assert(new->index == 10);
    assert(new->value == 20);
    assert(new->left == NULL);
    assert(new->right == NULL);
    free(new);

}