#include "stdio.h"

int main(void) {
    int a, b = 0, c = 0;
    a = (++b) + (++c);
    printf("%i %i %i\n", a, b, c);
    a = (b++) + (c++);
    printf("%i %i %i\n", a, b, c);
    a = (++b) + (c++);
    printf("%i %i %i\n", a, b, c);
    a = (b--) + (--c);
    printf("%i %i %i\n", a, b, c);
    // a = 5, b = 2, c = 2
}