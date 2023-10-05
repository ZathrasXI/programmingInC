#include <stdio.h>
#include <math.h>
#define E 2.71828182845904523536

int main(void){
	long denominators = 100000;
	long double best_approx = 87/32, current_calculation, i, j, top, bottom;

	printf("poor approx = %Lf\n", best_approx);

	for (i = 1; i <= denominators; i++){
		for (j = 1; j <= denominators; j++){
			current_calculation = fabsl(i / j);
			printf("current calculation = %Lf\n", current_calculation);
			if (fabsl(E - current_calculation) <= best_approx){
				best_approx = current_calculation;
				top = i;
				bottom = j;
			}
		}
	}

	printf("%Lf / %Lf \n", top, bottom);
	printf("%Lf\n", best_approx);

}

