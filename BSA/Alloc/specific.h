#include "../bsa.h"
#include <assert.h>

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





