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
			time_required %= times[i];
			i++;
			if (time_required > 1 && time_required < times[BUTTONS-1]) {
				button_presses++;			
			}
		}
	}
	printf("Number of button presses = %d", button_presses);

	
}
