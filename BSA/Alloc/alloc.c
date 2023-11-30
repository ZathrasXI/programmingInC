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
        b->rows[i]->data = (int *) calloc(1, sizeof(int *));
        b->rows[i]->data = NULL;
    }
    return b;
}

void test(void)
{
    // can init a BSA && all pointers in array are set to NULL
    bsa *test_init = bsa_init();
    for (int i = 0; i < BSA_ROWS; i++)
    {
        printf("%ls \n", test_init->rows[i]->data);
        // assert(test_init->rows[i]->data == NULL);
    }


    // can set a value at an index
    // bool set = bsa_set(test_init, 0, 1);
    // assert(set);
    // assert(test_init->rows[0]->data == 1);

}

// bool bsa_set(bsa *b, int indx, int d)
// {
//     // which row do I need?
//     // 
//     // is row in use? calloc space if not
//     // go to row
//     return true;
// }
