#include "stdio.h"
#include "stdlib.h"
#define LOOPS 15
#define A 7
#define C 5
#define M 11

int main(void) {
    /* Linear Congruential Generator */
    int seed = 0;
    int starting_number;
    for (int i = 0; i < LOOPS; i++) {
        seed = (A * seed + C) % M;
        if (i == 0) {
            starting_number = seed;
        } else if (seed == starting_number) {
            printf("%d\n", i);
        }
    }
}