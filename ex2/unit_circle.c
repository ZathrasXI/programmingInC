#include "stdio.h"
#include "math.h"
#include "stdbool.h"

int main(void){
    double x;
    printf("Enter a value for x: ");
    scanf("%f", &x);
    
    double formula = (sin(x) * sin(x)) + (cos(x) * cos(x));
    printf("%f\n", formula);
}
