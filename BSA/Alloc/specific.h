#include "../bsa.h"
#include <assert.h>

#define INITIAL_SIZE 1

int _pow_2(int x);
int _get_row_index(int cell_index);

typedef struct row
{
    int *data;
    int index;
    int length;
} row;

struct bsa {
    row *rows[BSA_ROWS];
    int max_index;
};





