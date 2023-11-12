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
    unique_boards[0].queens = 0;
    // empty board for 0th queen
    for (int row = 0; row < MAX_QUEENS; row++)
    {
        for (int col = 0; col < ROW_COL; col++)
        {
            unique_boards[0].queen_coords[row][0] = 0;
            unique_boards[0].queen_coords[row][1] = 0;
        }
    }


    int counter = 1, row = 0, col = 0;
    for (int i = 1; i <= (*boards_to_add * *boards_to_add); i++)
    {
        unique_boards[i].queens = 1;
        counter++;
        
        unique_boards[i].queen_coords[0][0] = row;
        unique_boards[i].queen_coords[0][1] = col;
        col++;
        if (col == *boards_to_add)
        {
            col = 0;
            row++;
        }
    }
    return counter;
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
            assert(test_boards[0].queen_coords[row][0] == 0);
            assert(test_boards[0].queen_coords[row][1] == 0);
        }
    }
    
    
    // queen on each board needs a unique location within an n*n board
    static Board test_locations[BOARDS];
    n_test = 8;
    int initial_boards = add_initial_boards(test_locations, &n_test);
    int row = 0, col = 0;
    for (int i = 1; i < initial_boards; i++)
    {
        assert(test_locations[i].queen_coords[0][0] == row);
        assert(test_locations[i].queen_coords[0][1] == col);

        col++;
        if (col == n_test)
        {
            col = 0;
            row++;
        }
    }
}
