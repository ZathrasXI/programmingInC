#include "../bsa.h"
#include <assert.h>

#define INITIAL_SIZE 1

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





