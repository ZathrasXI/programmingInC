#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#define TOKEN_LEN 30
#define MAX_TOKENS 200
#define ONE_ARG 1
#define FILE_NAME_LEN 20


typedef struct instructions
{
    char words[MAX_TOKENS][TOKEN_LEN];

} Instructions;

void test(void);
bool is_number(char *s);