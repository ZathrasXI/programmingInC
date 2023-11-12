#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define BOARDS 1000000
#define MAX_QUEENS 10
#define ROW_COL 2
#define QUEEN 1
#define UNUSED -1
#define ROW 0
#define COL 1
#define ERROR -2

// I am making an array to represent the unique locations a queen can be in. starting with 1 queen, ending on `n` queens


typedef struct board
{
    int queens;
    int queen_coords[MAX_QUEENS][ROW_COL];
} Board;


// I need to add n boards with 1 queen to the array. Then find their unique children - I don't know how many there will be.

void test(void);
bool parse_args(int *n, char *argv[], int argc, bool *verbose);
int add_initial_boards(Board *unique_boards, int *boards_to_add);
bool add_next_safe_location(int queen_coords[MAX_QUEENS][ROW_COL], int *n);
bool not_on_diagonals(int row_start, int col_start, int row_new, int col_new, int *n);
int index_for_next_queen(int queen_coords[MAX_QUEENS][ROW_COL], int *n);

int main(int argc, char* argv[])
{
    test();
    int n;
    bool verbose = false;
    bool valid_input = parse_args(&n, argv, argc, &verbose);

    if (!valid_input)
    {
        fprintf(stderr,"usage: ./8q <1-10> <optional: -verbose>\n");
        exit(EXIT_FAILURE);
    }

    static Board unique_locations[BOARDS];
    int initial_boards = add_initial_boards(unique_locations, &n);
    
    





    if (verbose)
    {
        printf("n = %d verbose = %d, initial_boards = %d\n", n, verbose, initial_boards);
    }

}

bool parse_args(int *n, char* argv[], int argc, bool *verbose)
{
    if (argc < 2 || argc > 3)
    {
        return false;
    }

    for (int i = 0; i < argc; i++)
    {
        if (sscanf(argv[i], "%d", n) == 1)
        {
           if (*n < 1 || *n > 10)
           {
                return false;
           } 
        }
        if (strcmp(argv[i], "-verbose") == 0)
        {
            *verbose = true;
        }
    }

    if (argc == 3 && *verbose == false)
    {
        return false;
    }
    return true;
}

int add_initial_boards(Board *unique_boards, int *boards_to_add)
{
    int counter = 0;
    // for all boards set first co-ord, then the rest = UNFINISHED
    int row = 0, col = 0;
    for (int b = 0; b < *boards_to_add * *boards_to_add + 1; b++)
    {
        for (int queen = 0; queen < MAX_QUEENS; queen++)
        {
            if (b > 0 && queen == 0)
            {
                unique_boards[b].queen_coords[queen][ROW] = row;
                unique_boards[b].queen_coords[queen][COL] = col;   
                col++;
  
            }
            else
            {
                unique_boards[b].queen_coords[queen][ROW] = UNUSED;
                unique_boards[b].queen_coords[queen][COL] = UNUSED;
            }
        }
        if (col == *boards_to_add)
        {
            col = 0;
            row++;
        }
        counter++;
    }

    unique_boards[0].queens = 0;
    return counter;
}

bool add_next_safe_location(int queen_coords[MAX_QUEENS][ROW_COL], int *n)
{
    int index_for_next_coords = index_for_next_queen(queen_coords, n);
    if (index_for_next_coords == ERROR)
    {
        fprintf(stderr, "couldn't find index to store co-ords for next queen\n");
        exit(EXIT_FAILURE);
    }

    int row_most_recent = queen_coords[index_for_next_coords - 1][ROW];
    int col_most_recent = queen_coords[index_for_next_coords - 1][COL];

    for (int row = 0; row < *n; row++)
    {
        for (int col = 0; col < *n; col++)
        {
            // only look at cells after starting cell
            if (row > row_most_recent)
            {
                if (row != row_most_recent && col != col_most_recent && not_on_diagonals(row_most_recent, col_most_recent, row, col, n))
                {
                    queen_coords[index_for_next_coords][ROW] = row;
                    queen_coords[index_for_next_coords][COL] = col;
                    return true;
                }
            }
        }
    }

    return false;
}

bool not_on_diagonals(int row_start, int col_start, int row_new, int col_new, int *n)
{
    // to top left
    int r1 = row_start, c1 = col_start;
    while (r1 >= 0 && c1 >= 0)
    {
        if (r1 == row_new && c1 == col_new)
        {
            return false;
        }
        r1--, c1--;
    }

    // to bottom right
    int r2 = row_start, c2 = col_start;
    while (r2 <= *n && c2 <= *n)
    {
        if (r2 == row_new && c2 == col_new)
        {
            return false;
        }
        r2++, c2++;
    }

    // to top right
    int r3 = row_start, c3 = col_start;
    while (r3 >= 0 && c3 <= *n)
    {
        if (r3 == row_new && c3 == col_new)
        {
            return false;
        }
        r3--, c3++;
    }

    // to bottom left
    int r4 = row_start, c4 = col_start;
    while (r4 <= *n && c4 >= 0)
    {
        if (r4 == row_new && c4 == col_new)
        {
            return false;
        }
        r4++, c4--;
    }
    return true;
}

int index_for_next_queen(int queen_coords[MAX_QUEENS][ROW_COL], int *n)
{
    for (int i = 0; i < *n; i++)
    {
        if (queen_coords[i][ROW] == UNUSED && queen_coords[i][COL] == UNUSED)
        {
            return i;
        }
    }
    
    return ERROR;
}


void test(void)
{
    int n_test;
    bool verbose_test = false;

    char *test_argv[] = {"./8q", "-verbose", "3"};
    assert(parse_args(&n_test, test_argv, 3, &verbose_test) == true);
    assert(n_test == 3);
    assert(verbose_test = true);

    verbose_test = false;
    char *test_argv1[] = {"./8q", "10", "-verbose"};
    assert(parse_args(&n_test, test_argv1, 3, &verbose_test) == true);
    assert(n_test == 10);
    assert(verbose_test = true);

    verbose_test = false;
    char *test_argv2[] = {"./8q", "11"};
    assert(parse_args(&n_test, test_argv2, 3, &verbose_test) == false);
    assert(n_test == 11);
    assert(verbose_test == false);


    static Board test_boards[BOARDS];
    n_test = 10;
    // n * n + 1 boards added
    assert(add_initial_boards(test_boards, &n_test)== n_test *n_test + 1);
    // board[0] has 0 queens 
    assert(test_boards[0].queens == 0);
    for (int row = 0; row < MAX_QUEENS; row++)
    {
        for (int col = 0; col < ROW_COL; col++)
        {
            assert(test_boards[0].queen_coords[row][0] == UNUSED);
            assert(test_boards[0].queen_coords[row][1] == UNUSED);
        }
    }
    
    
    // queen on each board needs a unique location within an n*n board
    static Board test_locations[BOARDS];
    n_test = 8;
    int initial_boards = add_initial_boards(test_locations, &n_test);
    int row = 0, col = 0;
    for (int b = 0; b < initial_boards; b++)
    {
        for (int queen = 0; queen < MAX_QUEENS; queen++)
        {
            // not the 0th board, only set the 0th co-ord
            // for the 0th queen on all boards apart from the 0th
            if (b > 0 && queen == 0)
            {
                assert(test_locations[b].queen_coords[queen][ROW] == row);
                assert(test_locations[b].queen_coords[queen][COL] == col);
                col++;
            }
            else
            {
                assert(test_locations[b].queen_coords[queen][ROW] == UNUSED);
                assert(test_locations[b].queen_coords[queen][COL] == UNUSED);
            }
        }
        if (col == n_test)
        {
            col = 0;
            row++;
        }
    }

    // can add a queen in next safe, and unique location
    // check starting location
    n_test = 9;
    assert(test_locations[1].queen_coords[0][ROW] == 0);
    assert(test_locations[1].queen_coords[0][COL] == 0);

    assert(not_on_diagonals(4,4,2,3,&n_test));
    assert(not_on_diagonals(4,4,2,5,&n_test));
    assert(not_on_diagonals(4,4,6,3,&n_test));
    assert(not_on_diagonals(4,4,6,5,&n_test));
    assert(!not_on_diagonals(4,4,0,0,&n_test));
    assert(!not_on_diagonals(4,4,0,8,&n_test));
    assert(!not_on_diagonals(4,4,8,8,&n_test));
    assert(!not_on_diagonals(4,4,8,0,&n_test));

    //get next available index in board.queen_coords for next queen
    assert(index_for_next_queen(test_locations[1].queen_coords, &n_test) == 1);
    int mock_coords[MAX_QUEENS][ROW_COL] = {{1,1},{2,2}, {3,3}, {4,4}, {5,5}, {UNUSED,UNUSED}, {UNUSED,UNUSED}, {UNUSED,UNUSED}, {UNUSED,UNUSED}};
    assert(index_for_next_queen(mock_coords, &n_test) == 5);
    int mock_coords2[MAX_QUEENS][ROW_COL] = {{1,1},{2,2}, {3,3}, {4,4}, {5,5}, {6,6}, {6,6}, {6,6}, {6,6}};
    assert(index_for_next_queen(mock_coords2, &n_test) == ERROR);
    
    // get next safest location
    bool queen_added = add_next_safe_location(test_locations[1].queen_coords, &n_test);
    assert(queen_added && test_locations[1].queen_coords[1][ROW] == 1);
    assert(queen_added && test_locations[1].queen_coords[1][COL] == 2);

    queen_added = add_next_safe_location(mock_coords, &n_test);
    assert(queen_added && mock_coords[5][ROW] == 6);
    assert(queen_added && mock_coords[5][COL] == 0);


    int mock_coords3[MAX_QUEENS][ROW_COL] = {{1,1},{2,2}, {3,3}, {4,4}, {5,5}, {3,3}, {3,3}, {3,3}, {UNUSED,UNUSED}};
    int next_index = index_for_next_queen(mock_coords3, &n_test);
    assert(next_index == 8);
    queen_added = add_next_safe_location(mock_coords3, &n_test);
    assert(queen_added && mock_coords3[next_index][ROW] == 4);
    assert(queen_added && mock_coords3[next_index][COL] == 0);
}
