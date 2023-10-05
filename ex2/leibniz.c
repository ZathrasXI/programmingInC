#include "stdio.h"
#include "stdbool.h"
#include "assert.h"
#include "math.h"
#define PI 3.1415926

bool doubles_are_close_enough(double a, double b);


int main(void) {
    assert(doubles_are_close_enough(PI, 3.1415927) == false);
    assert(doubles_are_close_enough(PI, 3.14159261) == true);
    assert(doubles_are_close_enough(PI, 3.1415925) == false);
    assert(doubles_are_close_enough(PI, 3.14159260) == true);
    
    double i = 3.0;
    double fraction = 4 / i;
    double calculation = 4 - fraction;
    bool minus = false;
    long counter = 0;


    while (!doubles_are_close_enough(calculation, PI)){
        i += 2.0;
        fraction = 4 / i;

        if (minus){
            calculation = calculation - fraction;
        } else {
            calculation = calculation + fraction;
        }
        minus = !minus;
        counter++;
    };
    printf("Counter = %.8ld\n", counter);
}


bool doubles_are_close_enough(double a, double b) {
    double diff;
    diff = fabs(a - b);
    if (diff >= 0.00000001) {
        return false;
    }
    return true;
}