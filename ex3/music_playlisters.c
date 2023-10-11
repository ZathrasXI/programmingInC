#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 20


int main(void){
	int numbers_printed[MAX_LENGTH] = {0};
	int songs_required, random_number = 0;
	printf("How many songs required? ");
	scanf("%d", &songs_required);
	int counter = 0;

	while (counter < songs_required){
		random_number = rand() % songs_required + 1;
		if (numbers_printed[random_number] == 0){
			printf("%d ", random_number);
			numbers_printed[random_number] = 1;
			counter++;
		}
	}
	printf("\n");
	return 0;
}




