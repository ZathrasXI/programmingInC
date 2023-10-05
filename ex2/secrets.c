#include "stdio.h"
#include "ctype.h"
#include "assert.h"

char convert_to_secret_char(char c);

int main(void) {
    assert(convert_to_secret_char('a') == 'z');
    assert(convert_to_secret_char('c') == 'x');
    assert(convert_to_secret_char('A') == 'Z');
    assert(convert_to_secret_char('C') == 'X');
    assert(convert_to_secret_char(' ') == ' ');
    assert(convert_to_secret_char('!') == '!');

    char c;
    do {
        c = getchar();
        char secret_char = convert_to_secret_char(c);
        putchar(secret_char);
    } while (c != '\n');
}


char convert_to_secret_char(char c) {
    int diff;
    if (!isalpha(c)){
        return c;
    } else {
        if (isupper(c)){
            diff = c - 'A';
            c = 'Z' - diff;
        } else {
            diff = c - 'a';
            c = 'z' - diff;
        }
    }
    return c;
}
