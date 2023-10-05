#include "stdio.h"

int main(void) {
    int amount_of_numbers_from_user;
    printf("How many numbers do you wish to enter? \n");
    scanf("%d", &amount_of_numbers_from_user);

    float current_highest = 0;
    for (int input = 0; input < amount_of_numbers_from_user; input++) {
        printf("Enter number #%d: \n", input + 1);
        float user_input;
        scanf("%f", &user_input);
        if (user_input > current_highest) {
            current_highest = user_input;
        }
    }
    printf("The largest number is: %.2f\n", current_highest);
}