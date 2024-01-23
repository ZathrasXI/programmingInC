#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>
#include <math.h>
#include <unistd.h>

#define TOKEN_LEN 30
#define ONE_ARG 1
#define OUTPUT_FILE 3
#define FILE_NAME_LEN 200
#define INIT_SIZE 1
#define STR_ONLY_QUOTES 2
#define SPACE_ASCII 32
//TODO find less rigid alternative
#define PATH 1000
//2D array is 51 wide, 33 high
#define WIDTH 51
#define HEIGHT 33
#define COL_START 25
#define ROW_START 16
#define MAX_VARS 26
#define ASCII_TO_NUM 65
#define PI 3.14159265358979323846
#define TOLERANCE 0.000001
#define NULL_CHAR 1
#define MAX_STACK_SIZE 100
#define EMPTY -1

typedef struct {
    //TODO is this a good size?
    double data[MAX_STACK_SIZE];
    int top;
} Stack;

typedef union var 
{
    double num;
    char *word;
} Var;
typedef enum {not_set, union_double, union_char} type_used;

typedef struct loc
{
    int row;
    int col;
    char colour;
} Loc;

typedef struct turtle 
{
    Loc path[PATH];
    int len;
    int capacity;
    char colour;
    double direction;
    Var vars[MAX_VARS];
    type_used type_in_use[MAX_VARS];
} Turtle;

typedef struct token
{
    char *str;
    struct token *next;
} Token;

//TODO no global variables
static Turtle ttl;

//Parser & interpreter
bool create_file(char *name);
void find_end_points(int x0, int y0, int len, int x1_y1[2]);
void calculate_line_coords(int x0, int y0, int x1, int y1, Turtle *ttl);
void represent_coords(char **screen);
void update_var(char *token_str, int dest_index);
Turtle *init_ttl();
int get_var_index(char var_name);
int next_row(int line_start, int step_n);
int next_col(int line_start, int step_n);
double degrees_to_radians(double degrees);
Token *new_token(char *c);
Token *tokenise(FILE *ttl_file);
void free_tokens(Token* head);
bool is_number(char *c);
bool is_letter(char *c);
bool is_op(char *c);
bool is_var(char *c);
bool is_varnum(char *c);
bool is_word(char *c);
bool is_col(Token *t, Turtle *ttl);
bool is_rgt(Token *t);
bool is_forward(Token *t, Turtle *ttl);
bool is_item(char *c);
bool is_items(Token *t);
bool is_lst(Token *t);
bool is_pfix(Token *t);
bool is_set(Token *t);
bool is_loop(Token *t, Turtle *ttl);
bool is_ins(Token *t, Turtle *ttl);
bool is_inslst(Token *t, Turtle *ttl);
bool loop_closed(Token *t);
bool is_prog(Token *t, Turtle *ttl);
void panic_msg(char *msg);
//Stack
Stack *stack_init(void);
bool is_empty(int top);
bool is_full(int top);
bool push(Stack *stack, double value);
double pop(Stack *stack);
double evaluate(char **exp, int len);
//Testing
void test(void);
void test_stack(void);
void test_ttl(void);
void test_new_token(void);
void test_is_num(void);
void test_is_letter(void);
void test_is_op(void);
void test_is_var(void);
void test_is_varnum(void);
void test_deg_2_rad(void);
void test_next_row_index(void);
void test_next_col_index(void);
void test_is_fwd(void);
void test_is_rgt(void);
void test_get_var_index(void);
void test_is_word(void);
void test_is_item(void);
void test_items(void);
void test_is_lst(void);
void test_is_col(void);
void test_is_pfix(void);
void test_is_set(void);
void test_update_var(void);
void test_is_loop(void);
void test_loop_closed(void);
void test_is_inslst(void);
void test_is_prog(void);
void test_integration(void);
void test_tokenise(void);
void test_printing(void);
void free_ttl(Turtle *ttl);



