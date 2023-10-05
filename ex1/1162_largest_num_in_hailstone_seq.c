#include "stdio.h"
#define LIMIT 10000000 
#include "assert.h"

long int largest_number_in_sequence(long int starting_value);
long int get_next_number_of_hailstone_seq(long int current_number);


int main(void) {
    assert(get_next_number_of_hailstone_seq(4)==2);
    assert(get_next_number_of_hailstone_seq(21)==64);
    assert(largest_number_in_sequence(10)==16);
    assert(largest_number_in_sequence(77)==232);
    assert(largest_number_in_sequence(77671)==1570824736);

    long int current_largest_number = 0;
    long int largest_in_current_sequence = 0;
    long int largest_initial_value = 0;
    long int i;

    for (i = 1; i < LIMIT; i++) {
        largest_in_current_sequence = largest_number_in_sequence(i);
        if (largest_in_current_sequence > current_largest_number) {
            current_largest_number = largest_in_current_sequence;
            largest_initial_value = i;
        }
    }
    printf("The largest number in a sequence is %lu, this is from %lu's sequence\n", current_largest_number, largest_initial_value);
    return 0;
}


long int largest_number_in_sequence(long int start_value) {
    long int largest_number = 0;
    long int next_number = start_value;
    while (next_number != 1) {
        next_number = get_next_number_of_hailstone_seq(next_number);
        if (next_number > largest_number) {
            largest_number = next_number;
        }
    }
    return largest_number;
};


long int get_next_number_of_hailstone_seq(long int current_number) {
    long int next_number = current_number;
    if (current_number % 2 == 0) {
        next_number = current_number / 2;
    } else {
        next_number = 3 * current_number + 1;
    }
    return next_number;
};
