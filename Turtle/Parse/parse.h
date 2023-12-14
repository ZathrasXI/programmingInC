#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define TOKEN_LEN 30
#define MAX_TOKENS 100
#define ONE_ARG 1
#define FILE_NAME_LEN 20


typedef struct file
{
    char **words[TOKEN_LEN][MAX_TOKENS];

} File;

void test(void);
bool is_number(char s[TOKEN_LEN]);