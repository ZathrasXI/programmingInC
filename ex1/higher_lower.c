#include "stdio.h"
#include "stdlib.h"

int get_random_number(int upper_limit);

int main(void) {
    printf("%d\n", rand());
    int random_number = get_random_number(1000);
    printf("%d\n", random_number);
    int user_input;
    do  {
        printf("Guess the number: ");
        scanf("%d", &user_input);
    } while (user_input != random_number);
    printf("Congratulations! You guessed correctly!\n");
    return 0;
}


int get_random_number(int upper_limit) {
    return rand() % upper_limit;
}