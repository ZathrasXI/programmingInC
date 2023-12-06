#include "../bsa.h"
#include <assert.h>

#define INITIAL_SIZE 1
#define STR_LEN 1000
#define NO_CELLS_WRITTEN -1
#define MAX_INDEX 1073741822
#define MIN_INDEX 0

int _pow_2(int x);
int _get_index_in_row(int i, int row);
void _next_lowest_max_index(bsa *b);
int _get_global_index(int row, int rel_index);
int _in_use_count(bsa *b, int row);

typedef struct row
{
    int *data;
    bool *in_use;
    int index;
    int length;
} row;

struct bsa
{
    row *rows[BSA_ROWS];
    int max_index;
};


