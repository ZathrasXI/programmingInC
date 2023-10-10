#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


#define MAX_LENGTH 20

bool is_number_in_list(int number, int list[]);
int get_number_not_in_list(int number, int list[], int upper_limit);



int main(void){
	assert(is_number_in_list(1, (int []) {3,4,5}) == false);
	assert(is_number_in_list(1, (int []) {3,4,5,1}) == true);

	int numbers_printed[MAX_LENGTH] = {0};
	int songs_required, random_number = 0;
	printf("How many songs required? ");
	scanf("%d", &songs_required);
	

	for (int i = 0; i <= songs_required; i++){
		random_number = rand() % 10;
		bool number_already_printed = is_number_in_list(random_number, numbers_printed);
		
		if (!number_already_printed){
			printf("%d\n", random_number);
			numbers_printed[i] = random_number;
		} else {
			int another_random_number = get_number_not_in_list(random_number, numbers_printed, songs_required);
			printf("%d\n", another_random_number);
		}
	}
}


bool is_number_in_list(int number, int list[]) {
	for (int i = 0; i < MAX_LENGTH; i++) {
		if (number == list[i]){
			return true;
		}
	}
	return false;
}


int get_number_not_in_list(int number, int list[], int upper_limit){
	int new_random_number = rand() % upper_limit;
	while(is_number_in_list(new_random_number, list)){
		new_random_number = rand() % upper_limit;
	}
	return new_random_number;
}


