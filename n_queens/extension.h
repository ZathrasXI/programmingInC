#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define BOARDS 1000000
#define MAX_QUEENS 10
#define QUEEN 1
#define UNUSED -1
#define OUT_OF_BOUNDS 99
#define ERROR -2

typedef struct board
{
    int queens;
    int queen_coords[MAX_QUEENS];
    struct board *next;
} Board;


void test(void);
bool parse_args(int *n, char *argv[], int argc, bool *verbose);
int add_initial_boards(Board** start, int *boards_to_add);
bool on_diagonals(int row_start, int col_start, int row_new, int col_new, int *n);
bool is_safe_space(int queen_coords[MAX_QUEENS], int row, int col, int *n);
void append_all_children(Board **current,Board **start, int *n);
bool is_unique(Board candidate, Board **start);
void add_new_board(Board **last_board, Board candidate);
Board *create_new_board(Board b);
void print_list(Board *location);
Board *end_of_list(Board **location);
void print_solved_nodes(Board *start, int n, bool verbose);
