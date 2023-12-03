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
        b->rows[row]->in_use = (bool *) calloc(b->rows[row]->length, sizeof(bool));

        if (b->rows[row]->data == NULL || b->rows[row]->in_use == NULL)
        {
            fprintf(stderr, "error allocating memory for unset row #%d\n", row);
            return false;
        }
    }

    int i = _get_index_in_row(indx, row);
    *(b->rows[row]->data + i) = d;
    *(b->rows[row]->in_use + i) = true;


    return true;
}

int *bsa_get(bsa *b, int index)
{
    int row = _get_row(index);
    int relative_index = _get_index_in_row(index, row);
    if (b->rows[row]->data == NULL)
    {
        return NULL;
    }
    return (b->rows[row]->data + relative_index);
}

bool bsa_delete(bsa *b, int indx)
{
    // todo return false when value not deleted
    int row = _get_row(indx);
    int relative_index = _get_index_in_row(indx, row);
    if (*(b->rows[row]->in_use + relative_index) == true)
    {
        *(b->rows[row]->in_use + relative_index) = false;
    }

    bool row_empty = true;
    int i = 0;
    while(*(b->rows[row]->in_use + i) == false &&
            i < b->rows[row]->length)
    {
        if (*(b->rows[row]->in_use + i) == true)
        {
            row_empty = false;
        }
        i++;
    }
    // printf("empty row? %d row %d\n", row_empty, row);
    if (row_empty)
    {
        // printf("len %d\n", b->rows[row]->length);
        // for (int i = b->rows[row]->length - 1; i >= 0; i--)
        // {
        //     printf("i %d  data %d  in_use %d\n", i, *(b->rows[row]->data + i), *(b->rows[row]->in_use + i));
        //     free((b->rows[row]->data + i));    
        //     printf("after data free\n");
        //     free((b->rows[row]->in_use + i)); 
        //     printf("after in_use free\n\n");

        // }

        //TODO check that I don't need to free ->data and ->in_use
        free(b->rows[row]->data);
        free(b->rows[row]->in_use);
        free(b->rows[row]);
        b->rows[row] = NULL;
    }

    return true;
}

void test(void)
{   
    int zeroth_index_final_row = 536870911;
    int final_index = 536870911 * 2;
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
    assert(*test_set->rows[0]->in_use);


    assert(bsa_set(test_set,2,1));
    assert(*(test_set->rows[1]->data + 1) == 1);
    assert(*(test_set->rows[1]->in_use + 1));


    assert(bsa_set(test_set, 6, 36));
    assert(*(test_set->rows[2]->data + 3) == 36);
    assert(*(test_set->rows[2]->in_use + 3));

    assert(bsa_set(test_set,zeroth_index_final_row,1));
    assert(*test_set->rows[29]->data == 1);
    assert(*test_set->rows[29]->in_use);

    assert(bsa_set(test_set, final_index, 99));
    assert(*(test_set->rows[29]->data + zeroth_index_final_row) == 99);
    assert(*(test_set->rows[29]->in_use + zeroth_index_final_row));

    // free bsa *test_set after test
    for (int i = 0; i < BSA_ROWS; i++)
    {
        free(test_set->rows[i]->data);
    }
    free(test_set);


    /*
    can get value of requested index
    */
    bsa *test_get = bsa_init();

    assert(bsa_get(test_get, 0) == NULL);
    assert(bsa_get(test_get, 1) == NULL);
    assert(bsa_get(test_get, zeroth_index_final_row) == NULL);
    assert(bsa_get(test_get, final_index) == NULL);

    assert(bsa_set(test_get, 0, 0));
    assert(*bsa_get(test_get, 0) == 0);

    assert(bsa_set(test_get, 1, 1));
    assert(*bsa_get(test_get, 1) == 1);

    assert(bsa_set(test_get, zeroth_index_final_row, 10));
    assert(*bsa_get(test_get, zeroth_index_final_row) == 10);

    assert(bsa_set(test_get, final_index, 99));
    assert(*bsa_get(test_get, final_index) == 99);

    /*
    can delete value
    when last value deleted, then row is free()'d
    */
    bsa *test_delete = bsa_init();
    assert(bsa_set(test_delete, 0, 1));
    assert(test_delete->rows[0]->in_use);
    assert(bsa_delete(test_delete, 0));
    assert(test_delete->rows[0] == NULL);

    assert(bsa_set(test_delete, 1, 1));
    row = _get_row(1);
    assert(*test_delete->rows[row]->data == 1);
    assert(test_delete->rows[row]->in_use);
    assert(bsa_delete(test_delete, 1));
    assert(test_delete->rows[row] == NULL);

}


