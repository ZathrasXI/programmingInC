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

void test(void)
{
    bsa *new_test = bsa_init();
    for (int i = 0 ; i < BSA_ROWS; i++)
    {
        assert(new_test->head[i] == NULL);
    }
    assert(new_test->max_index == NOT_SET);
    free(new_test);
}