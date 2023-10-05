#include "stdio.h"

int main(void) {
    printf("What triangle number value for which int would you like?\n");
    int triangle_int;
    scanf("%d", &triangle_int);
    int triangle_value = triangle_int * (triangle_int + 1) / 2;
    printf("The triangle number is %d\n", triangle_value);
    return 0;
}