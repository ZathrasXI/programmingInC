#include "stdio.h"

int main(void) {
    int amount_of_numbers_from_user;
    printf("How many numbers do you wish to enter? \n");
    scanf("%d", &amount_of_numbers_from_user);

    int users_numbers[amount_of_numbers_from_user];
    for (int input = 0; input < amount_of_numbers_from_user; input++) {
        printf("Enter number #%d: \n", input + 1);
        int user_input;
        scanf("%d", &user_input);
        if (user_input % 2 == 0) {
            users_numbers[input] = user_input;
        }
    }
    int largest_number = users_numbers[0];
    for (int num = 0; num < sizeof(users_numbers)/sizeof(users_numbers[0]); num ++) {
        if (users_numbers[num] > largest_number) {
            largest_number = users_numbers[num];
        }
    }
    printf("The largest even number is: %d", largest_number);
}