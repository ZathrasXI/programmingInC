#include <stdio.h>

int get_next_number_of_hailstone_seq(int current_number);

int main(void) {
	int starting_number;
	printf("What is your starting number for the hailstone sequence? ");
	int number_of_values = scanf("%i",&starting_number);
    if (number_of_values != 1) {
        return 1;
    };

    int next_number = get_next_number_of_hailstone_seq(starting_number);
    while (next_number != 1) {
        printf("%d\n", next_number);
        next_number = get_next_number_of_hailstone_seq(next_number);
    };
};

int get_next_number_of_hailstone_seq(int current_number) {
    int next_number;
    if (current_number % 2 == 0) {
        next_number = current_number / 2;
    } else {
        next_number = 3 * current_number + 1;
    }
    return next_number;
};