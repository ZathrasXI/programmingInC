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
    if (b->head[row] == NULL)
    {
        b->head[row] = _new_node(indx, d);
    }

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
    assert(!bsa_set(set_test, -1, 0));
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