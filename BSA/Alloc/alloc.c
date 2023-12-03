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

int _get_index_in_row(int index, int row)
{
    int zeroth_index_of_row = _pow_2(row) - 1;
    int relative_index = index - zeroth_index_of_row;
    return relative_index;
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

int _get_row(int cell_index)
{
    int row = 0;
    cell_index++;
    while (cell_index >>= 1)
    {
        row++;
    }
    return row;
}

bool bsa_set(bsa *b, int indx, int d)
{
    //TODO check that I have understood when to return false
    int row = _get_row(indx);
    if (b->rows[row]->data == NULL)
    {
        b->rows[row]->data = (int *) calloc(b->rows[row]->length, sizeof(int));
        if (b->rows[row]->data == NULL)
        {
            fprintf(stderr, "error allocating memory for unset row #%d\n", row);
            return false;
        }
    }

    int i = _get_index_in_row(indx, row);
    *(b->rows[row]->data + i) = d;

    return true;
}

void test(void)
{   
    /*
    bitwise i ** 2
    */
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

    /*
    bsa_maxindex() returns -1 when given a NULL BSA
    */
    assert(test_bsa->max_index == -1);
    assert(bsa_maxindex(test_bsa) == -1);

    // free bsa *test_bsa after test
    for (int i = 0; i < BSA_ROWS; i++)
    {
        free(test_bsa->rows[i]->data);
    }
    free(test_bsa);

    /*
    can get index of row based on index given
    */ 
    assert(_get_row(0) == 0);
    assert(_get_row(1) == 1);
    assert(_get_row(2) == 1);
    assert(_get_row(29) == 4);
    assert(_get_row(536870912) == 29);

    /*
    get index for data within the row 
    based upon the requested index
    */
    int row = _get_row(0);
    assert(_get_index_in_row(0, row) == 0);

    row = _get_row(536870911);
    assert(_get_index_in_row(536870911, row) == 0);

    row = _get_row(30);
    assert(_get_index_in_row(30, row) == 15);

    /*
    can set value of requested index
    */
    bsa *test_set = bsa_init();
    assert(bsa_set(test_set,0,0));
    assert(*test_set->rows[0]->data == 0);

    assert(bsa_set(test_set,2,1));
    assert(*(test_set->rows[1]->data + 1) == 1);

    assert(bsa_set(test_set, 6, 36));
    assert(*(test_set->rows[2]->data + 3) == 36);

    assert(bsa_set(test_set,536870911,1));
    assert(*(test_set->rows[29]->data + 0) == 1);

    assert(bsa_set(test_set,536870911 * 2, 99));
    assert(*(test_set->rows[29]->data + 536870911) == 99);

    // free bsa *test_set after test
    for (int i = 0; i < BSA_ROWS; i++)
    {
        free(test_set->rows[i]->data);
    }
    free(test_set);
}


