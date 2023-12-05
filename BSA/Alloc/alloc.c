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
    bool bsa_in_use = false;
    for (int i = 0; i < BSA_ROWS; i++)
    {
        if (b->rows[i]->data != NULL)
        {
            bsa_in_use = true;
        }
    }
    if (!bsa_in_use)
    {
        return -1;
    }
    else
    {
        return b->max_index;
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
            fprintf(stderr, "index not set: error allocating memory for unset row #%d\n", row);
            return false;
        }
    }

    int i = _get_index_in_row(indx, row);
    *(b->rows[row]->data + i) = d;
    *(b->rows[row]->in_use + i) = true;
    if (indx > b->max_index)
    {
        b->max_index = indx;
    }

    return true;
}

int *bsa_get(bsa *b, int index)
{
    int row = _get_row(index);
    int relative_index = _get_index_in_row(index, row);
    if (b->rows[row]->data == NULL || *(b->rows[row]->in_use + relative_index) == false)
    {
        return NULL;
    }
    return (b->rows[row]->data + relative_index);
}

bool bsa_delete(bsa *b, int indx)
{
    int row = _get_row(indx);
    int relative_index = _get_index_in_row(indx, row);
    if (b->rows[row]->in_use == NULL)
    {
        fprintf(stderr, "index #%d already deleted\n", indx);
        return false;
    }
    else if (*(b->rows[row]->in_use + relative_index))
    {
        *(b->rows[row]->in_use + relative_index) = false;
    }

    bool row_empty = true;
    for (int i = 0; i < b->rows[row]->length; i++)
    {
        if (*(b->rows[row]->in_use + i))
        {
            row_empty = false;
        }
    }
    
    // if (indx == b->max_index)
    // {
    //     _next_lowest_max_index(b);
    // }

    if (row_empty)
    {
        free(b->rows[row]->data);
        b->rows[row]->data = NULL;
        free(b->rows[row]->in_use);
        b->rows[row]->in_use = NULL;
        // free(b->rows[row]);
        // b->rows[row] = NULL;
    }

    return true;
}

int _get_current_index(int row, int rel_index)
{
    return _pow_2(row) - 1 + rel_index;
}

void _next_lowest_max_index(bsa* b)
{
        bool new_max_index_found = false;
        int max_row = _get_row(b->max_index);

        while (!new_max_index_found && max_row >= 0)
        {
            if (b->rows[max_row]->data != NULL)
            {
                for (int i = b->rows[max_row]->length - 1; i >= 0; i--)
                {
                    if (
                        _get_current_index(max_row, i) < b->max_index && 
                        *(b->rows[max_row]->in_use + i)
                        )
                        {
                            new_max_index_found = true;
                            b->max_index = _get_current_index(max_row, i);
                        }
                }
            }
            max_row--;
        }
        if (!new_max_index_found)
        {
            b->max_index = -1;
        }
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
        assert(test_bsa->rows[i]->in_use == NULL);
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

    row = _get_row(2);
    assert(_get_index_in_row(2, row) == 1);

    row = _get_row(536870911);
    assert(_get_index_in_row(536870911, row) == 0);

    row = _get_row(30);
    assert(_get_index_in_row(30, row) == 15);

    /*
    bsa_set()
    can store value in index
    max_index is updated when new index is greater
    matching index in `in_use` is set to true when index is used
    */
    bsa *test_set = bsa_init();
    assert(bsa_set(test_set,0,0));
    assert(*test_set->rows[0]->data == 0);
    assert(*test_set->rows[0]->in_use);
    assert(test_set->max_index == 0);

    assert(bsa_set(test_set,2,1));
    assert(*(test_set->rows[1]->data + 1) == 1);
    assert(*(test_set->rows[1]->in_use + 1));
    assert(test_set->max_index == 2);

    assert(bsa_set(test_set, 6, 36));
    assert(*(test_set->rows[2]->data + 3) == 36);
    assert(*(test_set->rows[2]->in_use + 3));
    assert(test_set->max_index == 6);

    assert(bsa_set(test_set,zeroth_index_final_row,1));
    assert(*test_set->rows[29]->data == 1);
    assert(*test_set->rows[29]->in_use);
    assert(test_set->max_index == zeroth_index_final_row);

    assert(bsa_set(test_set, final_index, 99));
    assert(*(test_set->rows[29]->data + zeroth_index_final_row) == 99);
    assert(*(test_set->rows[29]->in_use + zeroth_index_final_row));
    assert(test_set->max_index == final_index);

    // clean up
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

    assert(bsa_set(test_get, 1, 11));
    assert(*bsa_get(test_get, 1) == 11);

    assert(bsa_set(test_get, 1, 12));
    assert(*bsa_get(test_get, 1) == 12);

    assert(bsa_set(test_get, zeroth_index_final_row, 10));
    assert(*bsa_get(test_get, zeroth_index_final_row) == 10);

    assert(bsa_set(test_get, final_index, 99));
    assert(*bsa_get(test_get, final_index) == 99);

    //clean up
    free(test_get->rows[0]->data);
    free(test_get->rows[0]->in_use);
    free(test_get->rows[1]->data);
    free(test_get->rows[1]->in_use);
    free(test_get->rows[29]->data);
    free(test_get->rows[29]->in_use);
    free(test_get);

    /*
    can delete value
    when last value deleted, then row is free()'d
    */
   //TODO return false when element not deleted, e.g. index already not in use
    bsa *test_delete = bsa_init();
    assert(bsa_set(test_delete, 0, 1));
    assert(bsa_delete(test_delete, 0));
    assert(test_delete->rows[0]->data == NULL);
    assert(test_delete->rows[0]->in_use == NULL);

    assert(bsa_set(test_delete, 15, 99));
    assert(bsa_set(test_delete, 30, 100));
    assert(bsa_delete(test_delete, 15));
    assert(test_delete->rows[4]->data != NULL);
    assert(test_delete->rows[4]->in_use != NULL);
    assert(bsa_delete(test_delete, 30));
    assert(test_delete->rows[4]->data == NULL);
    assert(test_delete->rows[4]->in_use == NULL);

    assert(bsa_set(test_delete, 15, 1));
    assert(bsa_delete(test_delete, 15));
    assert(!bsa_delete(test_delete, 15));
    
    //clean up
    free(test_delete);

    /*
    get index based on row and relative index
    */
    assert(_get_current_index(0,0) == 0);
    assert(_get_current_index(1,1) == 2);
    assert(_get_current_index(3,3) == 10);
    assert(_get_current_index(29,3) == 536870914);

    /*
    _next_lowest_max_index() 
    */


}   


