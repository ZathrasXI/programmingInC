#include "specific.h"

int main(void)
{
    test();
    bsa *b = bsa_init();
    printf("%d\n", b->max_index);
    return 0;
}

bsa* bsa_init(void)
{
    bsa *b = (bsa *) calloc(INITIAL_SIZE, sizeof(bsa));
    for (int i = 0; i < BSA_ROWS; i++)
    {
        b->rows[i] = (row *) calloc(INITIAL_SIZE, sizeof(row));
        if (i > 0)
        {
            b->rows[i]->length = 1;
        }
        b->rows[i]->length <<= i;
    }
    return b;
}

int pow_2(int x)
{
    int index = 0;
    if (x > 0)
    {
        index = 1;
    }
    return index << x;
}

void test(void)
{
    // can init a BSA, all pointers in array are set to NULL, length = i^^2
    bsa *test_bsa = bsa_init();
    for (int i = 0; i < BSA_ROWS; i++)
    {
        assert(test_bsa->rows[i]->data == NULL);
        assert(test_bsa->rows[i]->length == pow_2(i));
    }

    for (int i = 0; i < BSA_ROWS; i++)
    {
        free(test_bsa->rows[i]->data);
    }
    free(test_bsa);




}


