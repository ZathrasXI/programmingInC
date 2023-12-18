#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>

#define TOKEN_LEN 30
#define ONE_ARG 1
#define FILE_NAME_LEN 200
#define INIT_SIZE 1
#define STR_ONLY_QUOTES 2
#define SPACE_ASCII 32


typedef struct token
{
    char *str;
    struct token *next;
    int length;
} Token;


typedef struct instructions
{
    Token *head;

} Instructions;

void test(void);
Token *new_token(char *c, int len);
void free_tokens(Token* head);
bool is_number(char *c);
bool is_letter(char *c);
bool is_op(char *c);
bool is_var(char *c);
bool is_varnum(char *c);
bool is_forward(char *c);
bool is_rgt(char *c);
bool is_word(char *c);
bool is_items(Token *t);
bool is_lst(Token *t);