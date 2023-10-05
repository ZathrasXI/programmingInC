#include "stdio.h"
#include "stdlib.h"
#define LOOP 500

int main(void) {
    int median = RAND_MAX / 2;
    int minus_cnt = 0, plus_cnt = 0;
    for (int i = 0; i < LOOP; i++) {
        if (rand() < median) {
            minus_cnt++;
        } else {
            plus_cnt++;
        }
        printf("random numbers less than median: %i; more than median: %i; different: %i\n", minus_cnt, plus_cnt, minus_cnt-plus_cnt);
    }
    return 0;
}