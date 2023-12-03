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
        b->rows[i]->length = _pow_2(i);
    }
    b->max_index = bsa_maxindex(b);
    return b;
}

int _pow_2(int x)
{
    return 1 << x;
}

int bsa_maxindex(bsa *b)
{
    //TODO return maxindex for when a row has been written to
    bool written_to = false;
    for (int i = 0; i < BSA_ROWS; i++)
    {
        if (b->rows[i]->data != NULL)
        {
            written_to = true;
        }
    }
    if (!written_to)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int _get_row_index(int cell_index)
{
    int row = 0;
    cell_index++;
    while (cell_index >>= 1)
    {
        row++;
    }
    return row;
}

void test(void)
{   
    //bitwise i ** 2
    assert(_pow_2(0) == 1);
    assert(_pow_2(1) == 2);
    assert(_pow_2(2) == 4);
    assert(_pow_2(29) == 536870912);

    /*
    can init a BSA, 
    all pointers in array are set to NULL, 
    length = i^^2
    */
    
    bsa *test_bsa = bsa_init();
    for (int i = 0; i < BSA_ROWS; i++)
    {
        assert(test_bsa->rows[i]->data == NULL);
        assert(test_bsa->rows[i]->length == _pow_2(i));
    }

    // bsa_maxindex() returns -1 when given a NULL BSA
    assert(test_bsa->max_index == -1);
    assert(bsa_maxindex(test_bsa) == -1);

    // free after test
    for (int i = 0; i < BSA_ROWS; i++)
    {
        free(test_bsa->rows[i]->data);
    }
    free(test_bsa);

    // can get index of row based on index given
    assert(_get_row_index(0) == 0);
    assert(_get_row_index(1) == 1);
    assert(_get_row_index(2) == 1);
    assert(_get_row_index(29) == 4);
    assert(_get_row_index(536870912) == 29);

}


