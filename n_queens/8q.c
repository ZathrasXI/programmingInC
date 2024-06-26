#include "8q.h"


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
    int initial_boards = add_initial_boards(unique_locations, n);
    int index_n_queens = 0;
    if (initial_boards != n * n + 1)
    {
        fprintf(stderr, "boards not initialised\n");
        exit(EXIT_FAILURE);
    }

    int f = 1;
    do
    {
        if (unique_locations[f - 1].queens == unique_locations[f].queens - 1)
        {
            index_n_queens = f;
        }
        append_all_children(unique_locations, f, n, &index_n_queens);
        f++;
    } while (unique_locations[f].in_use);
 

    print_solutions(unique_locations, verbose, n, f, index_n_queens);
    return 0;
}




bool parse_args(int *n, char* argv[], int argc, bool *verbose)
{
    // args can be in any order, only int 1-10 and -verbose are allowed
    if (argc < 2 || argc > 3)
    {
        return false;
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-verbose") == 0)
        {
            *verbose = true;
        }
        else if (sscanf(argv[i], "%d", n) == 1)
        {
           if (*n < 1 || *n > 10)
           {
                return false;
           } 
        }
        else
        {
            return false;
        }
        
    }

    if (argc == 3 && *verbose == false)
    {
        return false;
    }
    return true;
}

int add_initial_boards(Board *unique_boards, int n)
{
    int col = 0, queen = 0;
    unique_boards[0].queens = 0;
    unique_boards[0].in_use = true;

    int counter = 1;

    for (int b = 0; b < n * n + 1; b++)
    {
        for (int i = 0; i < MAX_QUEENS; i++)
        {
            if (i < n)
            {
                unique_boards[b].queen_coords[i] = UNUSED;
            }
            else
            {
                unique_boards[b].queen_coords[i] = OUT_OF_BOUNDS;
            }
        }

        if (b > 0)
        {
            unique_boards[b].queen_coords[queen] = col;   
            col++;

            unique_boards[b].in_use = true;
            unique_boards[b].queens = 1;

            if (col == n)
            {
                col = 0;
                queen++;
            }
            counter++;
        }
    }
    return counter;
}

bool on_diagonals(int row_start, int col_start, int row_new, int col_new, int n)
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
    while (r2 <= n && c2 <= n)
    {
        if (r2 == row_new && c2 == col_new)
        {
            return true;
        }
        r2++, c2++;
    }

    // to top right
    int r3 = row_start, c3 = col_start;
    while (r3 >= 0 && c3 <= n)
    {
        if (r3 == row_new && c3 == col_new)
        {
            return true;
        }
        r3--, c3++;
    }

    // to bottom left
    int r4 = row_start, c4 = col_start;
    while (r4 <= n && c4 >= 0)
    {
        if (r4 == row_new && c4 == col_new)
        {
            return true;
        }
        r4++, c4--;
    }
    return false;
}

bool is_safe_space(int queen_coords[MAX_QUEENS], int row, int col, int n)
{
    for (int queen_i = 0; queen_i < n; queen_i++)
    {
        if (col == queen_coords[queen_i])
        {
            return false;
        }
        if (queen_coords[queen_i] != UNUSED && row == queen_i)
        {
            return false;
        }
        if (queen_coords[queen_i] != UNUSED && on_diagonals(queen_i, queen_coords[queen_i], row, col, n))
        {
            return false;
        }
    }
    return true;
}

int next_available_index_in_array(Board *unique_boards, int current_board)
{
    int index = ERROR;
    for (int i = current_board; i < BOARDS; i++)
    {
        if (!unique_boards[i].in_use)
        {
            return i;
        }
    }
    return index;
}

void append_all_children(Board *unique_boards, int current_board, int n, int *index_n_queens)
{
    int next_free_index = next_available_index_in_array(unique_boards, current_board);
    if (next_free_index == ERROR)
    {
        fprintf(stderr, "next available index not found");
        exit(EXIT_FAILURE);
    }

    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (is_safe_space(unique_boards[current_board].queen_coords, row, col, n))
            {
                Board candidate = unique_boards[current_board];
                candidate.queens++;
                candidate.queen_coords[row] = col;
                if (is_unique(candidate, unique_boards, index_n_queens, next_free_index))
                {
                    unique_boards[next_free_index] = candidate;
                    next_free_index++;
                }
            }
        }
    }
}

bool is_unique(Board candidate, Board *unique_boards, int *index_n_queens, int current_board)
{
    for (int b = *index_n_queens; b < current_board; b++)
    {
        if (unique_boards[b].queens == candidate.queens && 
        memcmp(candidate.queen_coords, unique_boards[b].queen_coords, MAX_QUEENS * sizeof(int)) == 0)
        {
            return false;
        }
    }
    return true;
}

void print_solutions(Board *unique_locations, bool verbose, int n, int board_count, int first_index_nth_queen)
{
    int solutions_count = 0;
    for (int b = first_index_nth_queen; b < board_count; b++)
    {
        if(unique_locations[b].queens == n)
        {
            if (verbose)
            {
                for (int queen = 0; queen < n; queen++)
                {
                    unique_locations[b].queen_coords[queen]++;
                    if (unique_locations[b].queen_coords[queen] == 10)
                    {
                        printf("A");
                    }
                    else
                    {
                        printf("%d", unique_locations[b].queen_coords[queen]);
                    }
                }
                printf("\n");
            }
            solutions_count++;
        }
    }

    printf("%d solutions\n", solutions_count);
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
    assert(add_initial_boards(test_boards, n_test)== n_test *n_test + 1);
    assert(test_boards[0].queens == 0);
    assert(test_boards[0].in_use);
    for (int row = 0; row < MAX_QUEENS; row++)
    {
        assert(test_boards[0].queen_coords[row] == UNUSED);
    }
    
    
    // queen on each board needs a unique location within an nn board
    static Board test_locations[BOARDS];
    n_test = 8;
    int initial_boards = add_initial_boards(test_locations, n_test);
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

    assert(on_diagonals(0,1,1,2,n_test));
    assert(on_diagonals(0,1,2,3,n_test));
    assert(!on_diagonals(4,4,2,3,n_test));
    assert(!on_diagonals(4,4,2,5,n_test));
    assert(!on_diagonals(4,4,6,3,n_test));
    assert(!on_diagonals(4,4,6,5,n_test));
    assert(on_diagonals(4,4,0,0,n_test));
    assert(on_diagonals(4,4,0,8,n_test));
    assert(on_diagonals(4,4,8,8,n_test));
    assert(on_diagonals(4,4,8,0,n_test));

    //is safe space
    int queens[MAX_QUEENS] = {1,UNUSED,UNUSED,UNUSED};
    int queens1[MAX_QUEENS] = {1,3,0,UNUSED};
    n_test = 4;
    // row
    assert(!is_safe_space(queens,0,2,n_test));
    assert(!is_safe_space(queens,0,0,n_test));
    assert(is_safe_space(queens,1,3,n_test));
    //column
    assert(!is_safe_space(queens,0,1,n_test));
    assert(!is_safe_space(queens,2,1,n_test));
    assert(!is_safe_space(queens,3,1,n_test));
    assert(is_safe_space(queens,3,2,n_test));
    //diagonal
    assert(!is_safe_space(queens,2,3,n_test));
    assert(!is_safe_space(queens,1,2,n_test));
    assert(is_safe_space(queens1,3,2,n_test));
    assert(!is_safe_space(queens1,2,3,n_test));

    //index for next board
    static Board board_index_n_3[BOARDS];
    n_test = 3;
    int initiate_boards = add_initial_boards(board_index_n_3, n_test);
    int next_index = next_available_index_in_array(board_index_n_3,0);
    assert(next_index == initiate_boards);

    static Board board_index_n_10[BOARDS];
    n_test = 10;
    initiate_boards = add_initial_boards(board_index_n_10, n_test);
    next_index = next_available_index_in_array(board_index_n_10,0);
    assert(next_index == initiate_boards);

    //append children
    static Board test_child_boards[BOARDS];
    n_test = 4;
    int first_children = 6;
    int children[6][MAX_QUEENS] = {
        {0,2,UNUSED,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
        {0,3,UNUSED,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
        {0,UNUSED,1,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
        {0,UNUSED,3,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
        {0,UNUSED,UNUSED,1,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
        {0,UNUSED,UNUSED,2,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
    };
    int parent_boards = add_initial_boards(test_child_boards, n_test);
    int n_index = 1;
    append_all_children(test_child_boards, 1, n_test,&n_index);
    for (int i = 0; i < first_children; i++)
    {
        assert(memcmp(children[i], test_child_boards[parent_boards + i].queen_coords, first_children * sizeof(int)) == 0);
    }
    
    
    test_child_boards[17].in_use = true;
    test_child_boards[17].queens = 2;
    test_child_boards[17].queen_coords[0] = 1;
    test_child_boards[17].queen_coords[1] = 3;
    test_child_boards[17].queen_coords[2] = UNUSED;
    test_child_boards[17].queen_coords[3] = UNUSED;
    test_child_boards[17].queen_coords[4] = OUT_OF_BOUNDS;
    test_child_boards[17].queen_coords[5] = OUT_OF_BOUNDS;
    test_child_boards[17].queen_coords[6] = OUT_OF_BOUNDS;
    test_child_boards[17].queen_coords[7] = OUT_OF_BOUNDS;
    test_child_boards[17].queen_coords[8] = OUT_OF_BOUNDS;
    test_child_boards[17].queen_coords[9] = OUT_OF_BOUNDS;
    int start_index_uniqueness = 0;
    append_all_children(test_child_boards, 17, n_test, &start_index_uniqueness);
    int third_child[MAX_QUEENS] = {1,3,0,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS};
    assert(memcmp(third_child, test_child_boards[23].queen_coords, MAX_QUEENS * sizeof(int)) == 0);
    assert(test_child_boards[23].in_use);
    assert(test_child_boards[23].queens == 3);

    memcpy(test_child_boards[18].queen_coords, test_child_boards[17].queen_coords, MAX_QUEENS * sizeof(int));
    test_child_boards[18].queen_coords[2] = 0;
    test_child_boards[18].queens = 3;
    append_all_children(test_child_boards, 18, n_test,&start_index_uniqueness);
    int fourth_child[MAX_QUEENS] = {1,3,0,2,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS};    
    assert(memcmp(fourth_child, test_child_boards[26].queen_coords, MAX_QUEENS * sizeof(int)) == 0);
    assert(test_child_boards[26].in_use);
    assert(test_child_boards[26].queens == 4);


    // is board unique?
    static Board test_uniqueness[BOARDS] = {
        {4,{UNUSED,3,0,2,4,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true},
        {4,{3,UNUSED,0,2,4,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true},
        {4,{1,UNUSED,0,2,4,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true},
    };
    n_test = 5;
    Board test_unique_1 = {4,{1,2,3,4,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true};
    Board test_unique_2 = {4,{3,UNUSED,0,2,4,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true};
    int x = 0;
    assert(is_unique(test_unique_1, test_uniqueness,&x,2));
    assert(!is_unique(test_unique_2, test_uniqueness,&x,2));    

}
