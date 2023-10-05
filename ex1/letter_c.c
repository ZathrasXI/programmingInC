#include "stdio.h"
#define ROWS 10
int main(void) {
    printf("\n");
    int length_of_rows[ROWS] = {15, 3, 2, 1, 1, 1, 1, 2, 3, 15};
    int spaces[ROWS] = {3, 2, 1, 0, 0, 0, 0, 0, 1, 3};

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < spaces[i]; j++)
        {
            printf(" ");
        }
        for (int j = 0; j < length_of_rows[i]; j++)
        {
            printf("C");
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}