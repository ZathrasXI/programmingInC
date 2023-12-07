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

void test()
{
    bsa *test_init = bsa_init();
    assert(test_init->max_index == NOT_SET);
    assert(test_init->root == NULL);
    //clean up
    free(test_init);

}
