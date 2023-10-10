#include <stdio.h>
#define BUTTONS 3

int main(void){
	int time_required, button_presses = 0, presses_per_time = 0, remainder = 0;
	printf("Type the time required\n");
	scanf("%d", &time_required);
	
	int times[BUTTONS] = {600, 60, 10};
	int i = 0;

	while (i < BUTTONS) {
		if (times[i] > time_required){
			i++;
		} else {
			button_presses += time_required / times[i];
			printf("presses for %d = %d\n", times[i], presses_per_time);
			remainder = time_required % times[i];
			i++;
		}
	}
	printf("Number of button presses = %d", button_presses);

	
}
