#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <regex.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define TOKEN_LEN 30
#define ONE_ARG 1
#define PRINT_TERMINAL 2
#define OUTPUT_FILE 3
#define FILE_NAME_LEN 200
#define INIT_SIZE 1
#define STR_ONLY_QUOTES 2
#define SPACE_ASCII 32
//TODO find less rigid alternative
#define PATH 1000
#define WIDTH 51
#define HEIGHT 33
#define COL_START 25
#define ROW_START 16
#define ANSI_COL_START 1
#define ANSI_ROW_START 1
#define MAX_VARS 26
#define ASCII_TO_NUM 65
#define PI 3.14159265358979323846
#define TOLERANCE 0.000001
#define NULL_CHAR 1
#define MAX_STACK_SIZE 100
#define EMPTY -1
#define PADDING 2
#define NOT_FOUND -2
#define NEXT_INDEX 1
#define X_Y 2
#define PS_START_X 30
#define PS_START_Y 40

typedef struct f 
{
    FILE *file;
    char *fname;
} File_type;

typedef struct line
{
    float x0;
    float y0;
    float x1;
    float y1;
    char *colour;
    struct line *next;

} Line;

enum ansi_colours 
{
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    RESET = 0
};

typedef struct 
{
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
    bool fwd_ins;
    struct loc *next;
} Loc;

typedef struct turtle 
{
    Loc path[PATH];
    int len;
    Loc *path_start;
    Loc *path_end;
    int path_len;
    int capacity;
    char colour;
    double direction;
    Var vars[MAX_VARS];
    type_used type_in_use[MAX_VARS];
    bool ps_mode;
    Line *ps_start;
    Line *ps_last;
    bool valid;
} Turtle;

typedef struct token
{
    char *str;
    struct token *next;
    char *filename;
} Token;

typedef struct prog_args
{
    Token *head;
    Turtle *ttl;
} Prog_args;

//Extension
void init_turtles_cc(Prog_args *ttl_token, pthread_t *ttl_threads, int files);
void set_flags(bool *txt, bool *ps, int *file_count, int argc, char **argv);
char *extract_name(char *c);
void *cc_txt_file(void *ttl_tok);
bool is_ttl_file(char *f);
void *init_cc_ttl();
void *read_file_cc(void *filename);
void *tokenise_cc(void *file);
void *is_prog_cc(void *token_turtle);

void test_extract_name(void);

//Parser & interpreter
bool coords_within_ansi_boundaries(Loc *head);
bool path_to_txt_file(char *name, Turtle *ttl);
void update_colour(Turtle *ttl, char *colour);
void evaluate_postfix_expression(Turtle *ttl, Token *head, int token_count, int dest_index);
void copy_word_var_to_var(Turtle *ttl, int src_index, int dest_index);
void update_txt_ins(Turtle *ttl, int steps);
void update_postscript_ins(Turtle *ttl, int steps);
bool ps2pdf_cmd(char *filepath);
char *create_file_path(char *filename);
char *create_ps_file(Line *l, char *filename);
char *set_postscript_colour(char c);
Line *new_line(float x0, float y0, float x1, float y1);
Loc *new_loc(int col, int row, char c, bool fwd_ins);
bool ps_mode(char *filename);
void print_to_terminal(Turtle *ttl);
int get_ansi_colour(char c);
int next_fwd_ins(Turtle *ttl, int start);
bool create_txt_file(char *name, Turtle *ttl);
void find_end_points(float x0, float y0, int len, float x1_y1[2], Turtle *ttl);
void calculate_line_coords(int x0, int y0, int x1, int y1, Turtle *ttl);
void calculate_loc_coords(int x0, int y0, int x1, int y1, Turtle *ttl);
void represent_coords(char **screen);
void update_var(char *token_str, int dest_index, Turtle *ttl);
Turtle *init_ttl();
int get_var_index(char var_name);
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
bool is_rgt(Token *t, Turtle *ttl);
bool is_forward(Token *t, Turtle *ttl);
bool is_item(char *c);
bool is_items(Token *t);
bool is_lst(Token *t);
bool is_pfix(Token *t);
bool is_set(Token *t, Turtle *ttl);
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
double evaluate(char **exp, int len, Turtle *ttl);
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
void test_printing_tools(void);
void test_postscript(void);
void free_ttl(Turtle *ttl);
void test_new_loc();



