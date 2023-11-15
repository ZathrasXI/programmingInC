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

// TODO will I lose marks for not using a 2D array?
typedef struct board
{
    int queens;
    int queen_coords[MAX_QUEENS];
    bool in_use;
} Board;


void test(void);
bool parse_args(int *n, char *argv[], int argc, bool *verbose);
int add_initial_boards(Board *unique_boards, int *boards_to_add);
bool on_diagonals(int row_start, int col_start, int row_new, int col_new, int *n);
bool is_safe_space(int queen_coords[MAX_QUEENS], int row, int col, int *n);
int next_available_index_in_array(Board *unique_boards, int current_board);
void append_all_children(Board *unique_boards, int index, int *n, int *index_n_queens);
bool is_unique(Board candidate, Board *unique_boards, int *index_n_queens, int current_board);
void print_solutions(Board *unique_locations, bool *verbose, int *n, int *board_count);
