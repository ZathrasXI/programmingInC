#include "stdio.h"

int main(void) {
    int amount_requested;
    do {
        printf("How much money would you like?\n");
        scanf("%d", &amount_requested);
        if (amount_requested % 20 != 0) {
            int mod_remainder = amount_requested % 20;
            int withdrawal_option_1 = amount_requested - mod_remainder;

            int difference_to_twenty = 20 - mod_remainder;
            int withdrawal_option_2 = amount_requested + difference_to_twenty;
            printf("I can give you %i or %i try again\n", withdrawal_option_1, withdrawal_option_2);
        }
    } while(amount_requested % 20 != 0);
    printf("OK, dispensing...\n");



}
