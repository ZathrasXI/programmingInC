#include "specific.h"

int main(void)
{
    test();
    return 0;
}

bsa *bsa_init(void)
{
    bsa *b = (bsa *) calloc(INIT_SIZE, sizeof(bsa));
    if (b == NULL)
    {
        fprintf(stderr, "failed to allocate memory for BSA\n");
        exit(EXIT_FAILURE);
    }
    b->max_index = NOT_SET;
    b->root = NULL;
    return b;
}

bool bsa_set(bsa *b, int indx, int d)
{
    //tree is empty
    if (b->root == NULL)
    {
        b->root = (Node *) calloc(INIT_SIZE, sizeof(Node));
        if (b->root == NULL)
        {
            fprintf(stderr, "index not set: error allocating memory\n");
            exit(EXIT_FAILURE);
        }
        b->root->index = indx;
        b->root->data = d;
        b->root->height = 0;
        b->root->left = NULL;
        b->root->right = NULL;
        b->max_index = indx;
    return true;
    }

    if (indx >= b->root->index)
    {
        b->root->right = (Node *) calloc(INIT_SIZE, sizeof(Node));
        if (b->root->right == NULL)
        {
            fprintf(stderr, "failed to allocate memory for node\n");
            exit(EXIT_FAILURE);
        }
        b->root->right->index = indx;
        b->root->right->data = d;
        b->root->right->left = NULL;
        b->root->right->right = NULL;
        b->max_index = indx;
        return true;
    }
    else if (indx < b->root->index)
    {
        b->root->left = (Node *) calloc(INIT_SIZE, sizeof(Node));
        if (b->root->left == NULL)
        {
            fprintf(stderr, "failed to allocate memory for node\n");
            exit(EXIT_FAILURE);
        }
        b->root->left->index = indx;
        b->root->left->data = d;
        b->root->left->left = NULL;
        b->root->left->right = NULL;
        return true;
    }
    return false;
}

void test()
{
    /*
    bsa_init() 
    create empty BSA
    */
    bsa *test_init = bsa_init();
    assert(test_init->max_index == NOT_SET);
    assert(test_init->root == NULL);
    //clean up
    free(test_init);

    /*
    bsa_set()
    assign data to index
    */
    bsa *test_set = bsa_init();
    //set root
    assert(bsa_set(test_set, 10,1));
    assert(test_set->root->index == 10);
    assert(test_set->root->data == 1);
    assert(test_set->root->height == 0);
    assert(test_set->root->left == NULL);
    assert(test_set->root->right == NULL);
    assert(test_set->max_index == 10);

    //can set left child
    assert(bsa_set(test_set,9, 0));
    assert(test_set->root->left->index == 9);
    assert(test_set->root->left->data == 0);
    assert(test_set->max_index == 10);

    //clean up
    free(test_set->root->left);
    free(test_set->root);
    free(test_set);




}
