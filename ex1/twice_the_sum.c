#include "stdio.h"

int main(void) {
    int a, b, c, twice_the_sum;
    printf("Input three integers: \n");
    scanf("%d %d %d", &a, &b, &c);
    twice_the_sum = 2 * (a + b + c) + 7;
    printf("Twice the sum of integers plus 7 is: %d \n", twice_the_sum);
    return 0;
}