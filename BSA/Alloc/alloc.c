#include "specific.h"

int main(void)
{
    test();
    return 0;
}

bsa* bsa_init(void)
{
    bsa *b = (bsa *) calloc(1, sizeof(bsa));
    for (int i = 0; i < BSA_ROWS; i++)
    {
        b->arr[i] = NULL;
    }
    return b;
}

void test(void)
{
    // can init a BSA && all pointers in array are set to NULL
    bsa *test_init = bsa_init();
    for (int i = 0; i < BSA_ROWS; i++)
    {
        assert(test_init->arr[i] == NULL);
    }

    // can set a value at an index
    bool set = bsa_set(test_init, 0, 1);
    assert(set);
    assert(test_init->arr[0] = 1);
}

bool bsa_set(bsa *b, int indx, int d)
{
    // is row in use? calloc space if not
    // go to row
}
