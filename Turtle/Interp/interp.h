#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>
#include <math.h>

#define TOKEN_LEN 30
#define ONE_ARG 1
#define FILE_NAME_LEN 200
#define INIT_SIZE 1
#define STR_ONLY_QUOTES 2
#define SPACE_ASCII 32
//TODO find less rigid alternative
#define PATH 100
//2D array is 51 wide, 33 high
#define COL_START 25
#define ROW_START 16
#define MAX_VARS 26
#define ASCII_TO_NUM 65
#define PI 3.14159265358979323846

typedef union var 
{
    double num;
    char *word;
} Var;
typedef enum {union_double, union_char} type_used;

typedef struct loc
{
    int row;
    int col;
} Loc;

typedef struct turtle 
{
    Loc *path;
    int len;
    int capacity;
    char colour;
    double direction;
    Var *vars;
    type_used *type_in_use;
} Turtle;

typedef struct token
{
    char *str;
    struct token *next;
} Token;

static Turtle ttl;

void test(void);
void init_ttl();
int get_var_index(char var_name);
int next_row(int line_start, int step_n);
int next_col(int line_start, int step_n);
double degrees_to_radians(double degrees);
Token *new_token(char *c);
void free_tokens(Token* head);
bool is_number(char *c);
bool is_letter(char *c);
bool is_op(char *c);
bool is_var(char *c);
bool is_varnum(char *c);
bool is_word(char *c);
bool is_col(Token *t);
bool is_rgt(Token *t);
bool is_forward(Token *t);
bool is_items(Token *t);
bool is_lst(Token *t);
bool is_pfix(Token *t);
bool is_set(Token *t);
bool is_loop(Token *t);
bool is_ins(Token *t);
bool is_inslst(Token *t);
bool loop_closed(Token *t);
bool is_prog(Token *t);
void panic_msg(char *msg);