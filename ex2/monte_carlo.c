#include "stdio.h"
#include "stdlib.h"

#define RANGE 1000


int main(void){
    int r;
    printf("Enter a value for r: ");
    scanf("%d", &r);

    int area_of_square = r * r;
    int hc = 0;

    for (int i = 0; i < RANGE; i++) {
        int x = rand() % r;
        int y = rand() % r;
        if((x * x) + (y * y) <= area_of_square){
            hc++;
        } 
    }

    printf("hc: %d\n", hc);
    float pi = (4 * (float) hc) / (float) RANGE; 
    printf("PI: %f\n", pi);
}
