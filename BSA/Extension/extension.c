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
        b->root->value = d;
        b->root->height = 0;
        b->root->left = NULL;
        b->root->right = NULL;
    }
    return true;
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
    assign value to index
    */
   bsa *test_set = bsa_init();
   //set root
   assert(bsa_set(test_set, 10,10));
   assert(test_set->root->index == 10);
   assert(test_set->root->value == 10);
   assert(test_set->root->height == 0);
   assert(test_set->root->left == NULL);
   assert(test_set->root->right == NULL);
   free(test_set->root);
   free(test_set);




}
