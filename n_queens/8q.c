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

// I am making an array to represent the unique locations a queen can be in. starting with 1 queen, ending on `n` queens


typedef struct board
{
    int queens;
    int queen_coords[MAX_QUEENS];
    bool in_use;
} Board;


// I need to add n boards with 1 queen to the array. Then find their unique children - I don't know how many there will be.

void test(void);
bool parse_args(int *n, char *argv[], int argc, bool *verbose);
int add_initial_boards(Board *unique_boards, int *boards_to_add);
bool find_next_safe_location(int queen_coords[MAX_QUEENS], int *n);
bool on_diagonals(int row_start, int col_start, int row_new, int col_new, int *n);
int index_for_next_queen(int queen_coords[MAX_QUEENS], int *n);
bool board_is_unique(Board *unique_boards, int *b, int *n);
bool is_safe_space(int queen_coords[MAX_QUEENS], int row, int col, int *n);
int next_available_index_in_array(Board *unique_boards);


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
    // find all permutations for each initial board
    for (int board = 1; board < initial_boards; board++)
    {
        // for this board, append every unique child at the end of the list
        // append all children for this board
        // 
        // check if they are unique

        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                if(is_safe_space(unique_locations[board].queen_coords, row, col, &n))
                {
                    //append child e.g.
                    int index = next_available_index_in_array(unique_locations);
                    unique_locations[index].queen_coords[row] = col;
                    //set number of queens
                    unique_locations[index].queens = unique_locations[board].queens + 1;
                    unique_locations[index].in_use = true;
                }
            }
        }
    }

   





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
    int col = 0, queen = 0;

    for (int i = 0; i < *boards_to_add; i++)
    {
        unique_boards[0].queen_coords[i] = UNUSED;       
    }
    unique_boards[0].queens = 0;
    unique_boards[0].in_use = true;

    int counter = 1;

    for (int b = 1; b < *boards_to_add * *boards_to_add + 1; b++)
    {
        for (int i = 0; i < MAX_QUEENS; i++)
        {
            if (i < *boards_to_add)
            {
                unique_boards[b].queen_coords[i] = UNUSED;
            }
            else
            {
                unique_boards[b].queen_coords[i] = OUT_OF_BOUNDS;
            }
        }
    }

    for (int b = 1; b < *boards_to_add * *boards_to_add + 1; b++)
    {


        unique_boards[b].queen_coords[queen] = col;   
        col++;

        unique_boards[b].in_use = true;
        unique_boards[b].queens = 1;

        if (col == *boards_to_add)
        {
            col = 0;
            queen++;
        }
        counter++;
    }

   
    return counter;
}

bool find_next_safe_location(int queen_coords[MAX_QUEENS], int *n)
{
    int queen = index_for_next_queen(queen_coords, n);
    if (queen == ERROR)
    {
        fprintf(stderr, "couldn't find index to store co-ords for next queen\n");
        exit(EXIT_FAILURE);
    }

    int col_previous = queen_coords[queen - 1];

    for (int row = 0; row < *n; row++)
    {
        for (int col = 0; col < *n; col++)
        {
            if (row != queen - 1 && col != col_previous && on_diagonals(queen - 1, col_previous, row, col, n))
            {
                queen_coords[queen] = col;
                return true;
            }
        }
    }

    return false;
}

bool on_diagonals(int row_start, int col_start, int row_new, int col_new, int *n)
{
    // to top left
    int r1 = row_start, c1 = col_start;
    while (r1 >= 0 && c1 >= 0)
    {
        if (r1 == row_new && c1 == col_new)
        {
            return true;
        }
        r1--, c1--;
    }

    // to bottom right
    int r2 = row_start, c2 = col_start;
    while (r2 <= *n && c2 <= *n)
    {
        if (r2 == row_new && c2 == col_new)
        {
            return true;
        }
        r2++, c2++;
    }

    // to top right
    int r3 = row_start, c3 = col_start;
    while (r3 >= 0 && c3 <= *n)
    {
        if (r3 == row_new && c3 == col_new)
        {
            return true;
        }
        r3--, c3++;
    }

    // to bottom left
    int r4 = row_start, c4 = col_start;
    while (r4 <= *n && c4 >= 0)
    {
        if (r4 == row_new && c4 == col_new)
        {
            return true;
        }
        r4++, c4--;
    }
    return false;
}

int index_for_next_queen(int queen_coords[MAX_QUEENS], int *n)
{
    for (int i = 0; i < *n; i++)
    {
        if (queen_coords[i] == UNUSED)
        {
            return i;
        }
    }
    
    return ERROR;
}

bool board_is_unique(Board *unique_boards, int *b, int *n)
{
    bool unique;
    int matching_coords = 0;
    for (int stored = 1; stored < *b; stored++)
    {
        for (int new = 0; new < *n; new++)
        {
            for (int stored = 0; stored < *n; stored++)
            {
                if (unique_boards[*b].queen_coords[new] == unique_boards[stored].queen_coords[stored])
                {
                    matching_coords++;
                }
            }
        }
    }
                
    if (matching_coords == index_for_next_queen(unique_boards[*b].queen_coords, n) - 1)
    {
        unique = false;
    }
    else
    {
        unique = true;
    }
    return unique;
}

bool is_safe_space(int queen_coords[MAX_QUEENS], int row, int col, int *n)
{
    //iterate through coords if coords are on same row, column or diagonal
    for (int queen = 0; queen < *n; queen++)
    {
        if (col == queen_coords[queen])
        {
            return false;
        }
        if (queen_coords[queen] != UNUSED && row == queen)
        {
            return false;
        }
        if (queen_coords[queen] != UNUSED && on_diagonals(queen, queen_coords[queen], queen, col, n))
        {
            return false;
        }
    }
    return true;
}

int next_available_index_in_array(Board *unique_boards)
{
    int index = ERROR;
    for (int i = 0; i < BOARDS; i++)
    {
        if (!unique_boards[i].in_use)
        {
            return i;
        }
    }
    return index;
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
    assert(add_initial_boards(test_boards, &n_test)== n_test *n_test + 1);
    assert(test_boards[0].queens == 0);
    assert(test_boards[0].in_use);
    for (int row = 0; row < MAX_QUEENS; row++)
    {
        assert(test_boards[0].queen_coords[row] == UNUSED);
    }
    
    
    // queen on each board needs a unique location within an n*n board
    static Board test_locations[BOARDS];
    n_test = 8;
    int initial_boards = add_initial_boards(test_locations, &n_test);
    int col = 0, queen = 0;
    for (int b = 1; b < initial_boards; b++)
    {
        assert(test_locations[b].queens == 1);
        assert(test_locations[b].queen_coords[queen] == col);

        for (int i = 0; i < MAX_QUEENS; i++)
        {
            if (i < n_test && i != queen)
            {
                assert(test_locations[b].queen_coords[i] == UNUSED);
            }
            else if (i > n_test && i != queen)
            {
                assert(test_locations[b].queen_coords[i] == OUT_OF_BOUNDS);
            }
        }

        col++;

        assert(test_locations[b].in_use);
        if (col == n_test)
        {
            col = 0;
            queen++;
        }

    }

    // can add a queen in next safe, and unique location
    // check starting location
    n_test = 9;
    assert(test_locations[1].queen_coords[0] == 0);

    assert(!on_diagonals(4,4,2,3,&n_test));
    assert(!on_diagonals(4,4,2,5,&n_test));
    assert(!on_diagonals(4,4,6,3,&n_test));
    assert(!on_diagonals(4,4,6,5,&n_test));
    assert(on_diagonals(4,4,0,0,&n_test));
    assert(on_diagonals(4,4,0,8,&n_test));
    assert(on_diagonals(4,4,8,8,&n_test));
    assert(on_diagonals(4,4,8,0,&n_test));

    //is safe space
    int queens[MAX_QUEENS] = {0,UNUSED,UNUSED,UNUSED};
    n_test = 4;
    assert(!is_safe_space(queens,0,2,&n_test));
    assert(is_safe_space(queens,1,3,&n_test));
    assert(is_safe_space(queens,2,1,&n_test));
    assert(is_safe_space(queens,3,1,&n_test));
    int queens1[MAX_QUEENS] = {1,3,0,UNUSED};
    assert(is_safe_space(queens1,3,2,&n_test));
    assert(!is_safe_space(queens1,3,0,&n_test));
    assert(!is_safe_space(queens1,3,1,&n_test));
    assert(!is_safe_space(queens1,3,3,&n_test));

    //index for next board
    static Board board_index_n_3[BOARDS];
    n_test = 3;
    int initiate_boards = add_initial_boards(board_index_n_3, &n_test);
    int next_index = next_available_index_in_array(board_index_n_3);
    assert(next_index == initiate_boards);

    static Board board_index_n_10[BOARDS];
    n_test = 10;
    initiate_boards = add_initial_boards(board_index_n_10, &n_test);
    next_index = next_available_index_in_array(board_index_n_10);
    assert(next_index == initiate_boards);


    //get next available index in board.queen_coords for next queen
    // n_test = 9;
    // assert(index_for_next_queen(test_locations[1].queen_coords, &n_test) == 1);
    // int mock_coords[MAX_QUEENS] = {1,2,3,4,5,UNUSED,UNUSED,UNUSED,UNUSED};
    // assert(index_for_next_queen(mock_coords, &n_test) == 5);
    // int mock_coords2[MAX_QUEENS] = {1,2,3,4,5,6,6,6,6};
    // assert(index_for_next_queen(mock_coords2, &n_test) == ERROR);
    
    // // get next safest location
    // bool queen_added = find_next_safe_location(test_locations[1].queen_coords, &n_test);
    // assert(queen_added && test_locations[1].queen_coords[1] == 2);

    // queen_added = find_next_safe_location(mock_coords, &n_test);
    // assert(queen_added && mock_coords[5] == 0);

    // int mock_coords4[MAX_QUEENS] = {8,UNUSED,UNUSED,UNUSED,UNUSED,UNUSED,UNUSED,UNUSED,UNUSED};
    // queen_added = find_next_safe_location(mock_coords4, &n_test);
    // assert(!queen_added);
    
    // mock_coords4[0] = 0;
    // queen_added = find_next_safe_location(mock_coords4, &n_test);
    // assert(!queen_added);

    // mock_coords4[0] = 8;
    // queen_added = find_next_safe_location(mock_coords4, &n_test);
    // assert(queen_added);

    // int mock_coords3[MAX_QUEENS] = {1,2,3,4,5,3,3,3,UNUSED};
    // int next_index = index_for_next_queen(mock_coords3, &n_test);
    // assert(next_index == 8);
    // queen_added = find_next_safe_location(mock_coords3, &n_test);
    // assert(queen_added && mock_coords3[next_index] == 0);


 


    

}
