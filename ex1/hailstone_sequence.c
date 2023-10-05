#include "stdio.h"
#include "assert.h"
#define LIMIT 10000000

unsigned long int get_length_of_hailstone_seq(unsigned long int start_number);


int main(void) {

    unsigned long int longest_seq = 0;
    unsigned long int value_of_longest_seq = 0;
    unsigned long int i;
    for (i = 1; i < LIMIT; i++) {
        unsigned long int length_of_current_hailstone_seq = get_length_of_hailstone_seq(i);
        if (length_of_current_hailstone_seq > longest_seq) {
            longest_seq = length_of_current_hailstone_seq;
            value_of_longest_seq = i;
        };
    };
    printf("%lu has the longest hailstone sequence with: %lu\n", value_of_longest_seq, longest_seq);

    assert(get_length_of_hailstone_seq(6) == 9);
    assert(get_length_of_hailstone_seq(11) == 15);

    return 0;
}


unsigned long int get_length_of_hailstone_seq(unsigned long int start_number) {
    unsigned long int length = 1;
    unsigned long int next_number = start_number;

    while (next_number != 1) {
        if (next_number % 2 == 0) {
            next_number = next_number / 2;
        } else {
            next_number = 3 * next_number + 1;
        }
        length++;
    };
    return length;
}