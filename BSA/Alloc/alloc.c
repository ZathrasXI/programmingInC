#include "specific.h"


bsa* bsa_init(void)
{
    bsa *b = (bsa *) calloc(INITIAL_SIZE, sizeof(bsa));
    if (b == NULL)
    {
        fprintf(stderr, "error allocating space for bsa\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < BSA_ROWS; i++)
    {
        b->rows[i] = (row *) calloc(INITIAL_SIZE, sizeof(row));
        if (b->rows[i] == NULL)
        {
            fprintf(stderr, "error allocating space for row #%d\n", i);
        }
        b->rows[i]->data = NULL;
        b->rows[i]->in_use = NULL;
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
    if(b == NULL)
    {
        return NO_CELLS_WRITTEN;
    }

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
        return NO_CELLS_WRITTEN;
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
    if (indx < MIN_INDEX || indx > MAX_INDEX)
    {
        return false;
    }

    int row = _get_row(indx);
    if (b->rows[row]->data == NULL)
    {
        int len = b->rows[row]->length;
        b->rows[row]->data = (int *) calloc(len, sizeof(int));
        b->rows[row]->in_use = (bool *) calloc(len, sizeof(bool));
        if (b->rows[row]->data == NULL || b->rows[row]->in_use == NULL)
        {
            fprintf(stderr, "index not set: error allocating memory for row #%d\n", row);
            exit(EXIT_FAILURE);
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
    if (b->rows[row]->data == NULL || 
        *(b->rows[row]->in_use + relative_index) == false)
    {
        return NULL;
    }
    return (b->rows[row]->data + relative_index);
}

bool bsa_delete(bsa *b, int indx)
{
    int row = _get_row(indx);
    int relative_index = _get_index_in_row(indx, row);
    if (b->rows[row]->in_use == NULL || 
        *(b->rows[row]->in_use + relative_index) == false)
    {
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
    
    if (indx == b->max_index)
    {
        _next_lowest_max_index(b);
    }

    if (row_empty)
    {
        free(b->rows[row]->data);
        b->rows[row]->data = NULL;
        free(b->rows[row]->in_use);
        b->rows[row]->in_use = NULL;
    }
    return true;
}

int _get_global_index(int row, int rel_index)
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
                    _get_global_index(max_row, i) < b->max_index && 
                    *(b->rows[max_row]->in_use + i) &&
                    !new_max_index_found
                    )
                    {
                        new_max_index_found = true;
                        b->max_index = _get_global_index(max_row, i);
                    }
            }
        }
        max_row--;
    }
    if (!new_max_index_found)
    {
        b->max_index = NO_CELLS_WRITTEN;
    }
}

bool bsa_free(bsa *b)
{
    if (b == NULL)
    {
        return false;
    }

    for (int i = 0; i < BSA_ROWS; i++)
    {
        free(b->rows[i]->data);
        free(b->rows[i]->in_use);
        free(b->rows[i]);
    }
    free(b);
    return true;
}

bool bsa_tostring(bsa *b, char *str)
{
    strcpy(str, "");
    if (b == NULL)
    {
        return false;
    }
    if (b->max_index == NO_CELLS_WRITTEN)
    {
        return true;
    }

    int max_row = _get_row(b->max_index);
    for (int row = 0; row <= max_row; row++)
    {
        strcat(str,"{");
        if (b->rows[row]->data != NULL)
        {
            int spaces_total = _in_use_count(b, row) - 1;
            int spaces = 0;
            for (int d = 0; d < b->rows[row]->length; d++)
            {
                if (*(b->rows[row]->in_use + d))
                {

                    char tmp[100];
                    sprintf(tmp,"[%d]=%d", _get_global_index(row, d), *(b->rows[row]->data + d));
                    strcat(str, tmp);
                    if (spaces < spaces_total)
                    {
                        strcat(str, " ");
                        spaces++;
                    }
                }
            }
        }
        strcat(str,"}");
    }
    return true;
}

int _in_use_count(bsa *b, int row)
{
    int count = 0;
    for (int i = 0; i < b->rows[row]->length; i++)
    {
        if (*(b->rows[row]->in_use + i))
        {
            count++;
        }
    }
    return count;
}

void bsa_foreach(void (*func)(int *p, int *n), bsa *b, int *acc)
{
    
    for (int row = 0; row < BSA_ROWS; row++)
    {
        if (b->rows[row]->data != NULL)
        {
            for (int i = 0; i < b->rows[row]->length; i++)
            {
                if (*(b->rows[row]->in_use + i))
                {
                    func((b->rows[row]->data + i), acc);
                }
            }
        }
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
    bsa_maxindex() returns -1/NO_CELLS_WRITTEN 
    when given a NULL BSA
    */
    assert(test_bsa->max_index == NO_CELLS_WRITTEN);
    assert(bsa_maxindex(test_bsa) == NO_CELLS_WRITTEN);

    // free bsa *test_bsa after test
    for (int i = 0; i < BSA_ROWS; i++)
    {
        free(test_bsa->rows[i]->data);
        free(test_bsa->rows[i]->in_use);
        free(test_bsa->rows[i]);
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
    assert(!bsa_set(test_set, -1, 2));
    assert(!bsa_set(test_set, final_index + 1, 2));

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
    assert(bsa_free(test_set));

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
    assert(bsa_free(test_get));

    /*
    can delete value
    when last value deleted, then row is free()'d
    */
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
    assert(bsa_set(test_delete, 30, 1));
    assert(bsa_delete(test_delete, 15));
    assert(!bsa_delete(test_delete, 15));
    assert(bsa_delete(test_delete, 30));
    assert(!bsa_delete(test_delete, 30));
    assert(test_delete->rows[4]->data == NULL);
    assert(test_delete->rows[4]->in_use == NULL);

    //clean up
    assert(bsa_free(test_delete));

    /*
    get index based on row and relative index
    */
    assert(_get_global_index(0,0) == 0);
    assert(_get_global_index(1,1) == 2);
    assert(_get_global_index(3,3) == 10);
    assert(_get_global_index(29,3) == 536870914);

    /*
    _next_lowest_max_index() 
    */
    bsa *test_next_index = bsa_init();
    //2 indexes, on same row
    assert(bsa_set(test_next_index, 15, 3));
    assert(bsa_set(test_next_index, 30, 10));
    *(test_next_index->rows[4]->in_use + 15) = false;
    _next_lowest_max_index(test_next_index);
    assert(test_next_index->max_index == 15);
    //clean up
    assert(bsa_delete(test_next_index, 15));
    assert(test_next_index->rows[4]->data == NULL);
    assert(test_next_index->rows[4]->in_use == NULL);

    //2 indexes, on different rows
    assert(bsa_set(test_next_index, 14, 7));
    assert(bsa_set(test_next_index, 30, 15));
    *(test_next_index->rows[4]->in_use + 15) = false;
    _next_lowest_max_index(test_next_index);
    assert(test_next_index->max_index == 14);
    //clean up
    assert(bsa_delete(test_next_index, 14));

    //delete both indexes
    assert(bsa_set(test_next_index, 14, 7));
    assert(bsa_set(test_next_index, 30, 15));
    *(test_next_index->rows[4]->in_use + 15) = false;
    _next_lowest_max_index(test_next_index);
    *(test_next_index->rows[3]->in_use + 7) = false;
    _next_lowest_max_index(test_next_index);
    assert(test_next_index->max_index == NO_CELLS_WRITTEN);
    //clean up
    assert(bsa_free(test_next_index));

    /*
    bsa_maxindex() returns max index, unless
    all rows are NULL, then returns 
    -1/NO_CELLS_WRITTEN
    */

    //max index is -1/NO_CELLS_WRITTEN after init
    bsa *test_max = bsa_init();
    assert(bsa_maxindex(test_max) == NO_CELLS_WRITTEN);

    //max index updates when greater index is used
    assert(bsa_set(test_max, 1,10));    
    assert(bsa_maxindex(test_max) == 1);

    assert(bsa_set(test_max, 2,20));    
    assert(bsa_maxindex(test_max) == 2);

    assert(bsa_set(test_max, 3,30));    
    assert(bsa_maxindex(test_max) == 3);

    assert(bsa_set(test_max, 4,40));    
    assert(bsa_maxindex(test_max) == 4);

    assert(bsa_set(test_max, 5,50));    
    assert(bsa_maxindex(test_max) == 5);

    //max index updates when greatest index is no longer in use
    assert(bsa_delete(test_max, 5));
    assert(bsa_maxindex(test_max) == 4);
    assert(bsa_delete(test_max, 4));
    assert(bsa_maxindex(test_max) == 3);
    assert(bsa_delete(test_max, 2));
    assert(bsa_maxindex(test_max) == 3);
    assert(bsa_delete(test_max, 3));
    assert(bsa_maxindex(test_max) == 1);
    assert(bsa_delete(test_max, 1));
    assert(bsa_maxindex(test_max) == NO_CELLS_WRITTEN);

    for (int i = 0; i < BSA_ROWS; i++)
    {
        assert(test_max->rows[i]->data == NULL);
        assert(test_max->rows[i]->in_use == NULL);
    }
    //clean up
    assert(bsa_free(test_max));

    /*
    bsa_free() frees all space used
    */

    //returns false when already is a NULL pointer
    bsa *test_free = NULL;
    assert(!bsa_free(test_free));

    //returns true when it has free()'d memory
    test_free = bsa_init();
    assert(bsa_set(test_free, 0, 0));
    assert(bsa_set(test_free, 2, 2));
    assert(bsa_set(test_free, 4, 4));
    assert(bsa_set(test_free, 6, 6));
    assert(bsa_set(test_free, 8, 8));
    assert(bsa_set(test_free, 10, 10));
    assert(bsa_set(test_free, 12, 12));
    assert(bsa_set(test_free, zeroth_index_final_row, 99));
    assert(bsa_set(test_free, final_index, 100));
    assert(bsa_free(test_free));


    /*
    _in_use_count() returns total of indexes in use
    else returns -1/NO_CELLS_WRITTEN
    */
    bsa *test_in_use_count = bsa_init();
    assert(bsa_set(test_in_use_count,3,3));
    assert(bsa_set(test_in_use_count,4,3));
    assert(bsa_set(test_in_use_count,5,3));
    assert(bsa_set(test_in_use_count,6,3));
    assert(_in_use_count(test_in_use_count, 2) == 4);

    assert(bsa_delete(test_in_use_count, 4));
    assert(_in_use_count(test_in_use_count, 2) == 3);

    assert(bsa_delete(test_in_use_count, 5));
    assert(_in_use_count(test_in_use_count, 2) == 2);

    assert(bsa_delete(test_in_use_count, 3));
    assert(_in_use_count(test_in_use_count, 2) == 1);

    //clean up
    assert(bsa_free(test_in_use_count));

    /*
    bsa_tostring()
    creates string from structure
    */
    bsa *test_str = bsa_init();
    char str[STR_LEN] = "";
    // false when bsa is not in use - 
    // max_index == -1/NO_CELLS_WRITTEN
    // assert(!bsa_tostring(test_str, str));

    //basic string
    assert(bsa_set(test_str, 0, 0));
    assert(bsa_tostring(test_str, str));
    assert(strcmp(str, "{[0]=0}") == 0);
    //clean up
    strcpy(str, "");

    assert(bsa_set(test_str, 1, 1));
    assert(bsa_set(test_str, 2, 2));
    assert(bsa_tostring(test_str, str));
    assert(strcmp(str, "{[0]=0}{[1]=1 [2]=2}") == 0);
    //clean up
    strcpy(str, "");
    assert(bsa_delete(test_str, 0));

    // spaces between values on same row
    assert(bsa_set(test_str, 1, 2));
    assert(bsa_set(test_str, 2, 4));
    assert(bsa_set(test_str, 3, 6));
    assert(bsa_set(test_str, 4, 8));
    assert(bsa_set(test_str, 5, 10));
    assert(bsa_set(test_str, 6, 12));
    assert(bsa_tostring(test_str, str));
    assert(strcmp(str, "{}{[1]=2 [2]=4}{[3]=6 [4]=8 [5]=10 [6]=12}") == 0);
    //clean up
    strcpy(str, "");  
    bsa_delete(test_str,1);
    bsa_delete(test_str,2);
    bsa_delete(test_str,3);
    bsa_delete(test_str,4);
    bsa_delete(test_str,5);
    bsa_delete(test_str,6);

    // string spanning many rows
    assert(bsa_set(test_str, 0, 1));
    assert(bsa_set(test_str, 1, 2));
    assert(bsa_set(test_str, 2, 6));
    assert(bsa_set(test_str, 30, 300));
    assert(bsa_set(test_str, 254, 254));
    assert(bsa_tostring(test_str, str));
    assert(strcmp(str, "{[0]=1}{[1]=2 [2]=6}{}{}{[30]=300}{}{}{[254]=254}") == 0);
    //clean up
    strcpy(str, "");  
    bsa_free(test_str);

    // when all are deleted str is empty
    bsa *test_str2 = bsa_init();
    assert(bsa_set(test_str2, 1, 2));
    assert(bsa_set(test_str2, 2, 4));
    assert(bsa_set(test_str2, 3, 6));
    assert(bsa_set(test_str2, 4, 8));
    assert(bsa_set(test_str2, 5, 10));
    assert(bsa_set(test_str2, 6, 12));
    assert(bsa_delete(test_str2,1));
    assert(bsa_delete(test_str2,2));
    assert(bsa_delete(test_str2,3));
    assert(bsa_delete(test_str2,4));
    assert(bsa_delete(test_str2,5));
    assert(bsa_delete(test_str2,6));
    assert(bsa_tostring(test_str2, str));
    assert(strcmp(str, "") == 0);
    //clean up
    assert(bsa_free(test_str2));
}   


