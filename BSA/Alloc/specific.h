#include "../bsa.h"
#include <assert.h>

#define INITIAL_SIZE 1

int _pow_2(int x);
int _get_index_in_row(int i, int row);

typedef struct row
{
    int *data;
    bool *in_use;
    int index;
    int length;
} row;

struct bsa {
    row *rows[BSA_ROWS];
    int max_index;
};





