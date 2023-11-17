#include "extension.h"


int main(int argc, char* argv[])
{
    // test();
    int n;
    bool verbose = false;
    bool valid_input = parse_args(&n, argv, argc, &verbose);

    if (!valid_input)
    {
        fprintf(stderr,"usage: ./ext <1-10> <optional: -verbose>\n");
        exit(EXIT_FAILURE);
    }

    Board *start = NULL;
    Board f;
    f.queens = 0;
    for (int q = 0; q < MAX_QUEENS; q++)
    {
        if (q < n)
        {
            f.queen_coords[q] = UNUSED;
        }
        else
        {
            f.queen_coords[q] = OUT_OF_BOUNDS;
        }
    }
    add_new_board(&start,f);

    int initial_boards = add_initial_boards(&start, &n);
    if (initial_boards != n * n + 1)
    {
        fprintf(stderr, "%d boards not initialised\n", initial_boards);
        exit(EXIT_FAILURE);
    }

    Board *current = start;
    while (current != NULL)
    {
        append_all_children(&current, &start, &n);
        current = current->next;
    }

    print_solved_nodes(start, n, verbose);
}

void print_solved_nodes(Board *start, int n, bool verbose)
{
    int counter = 0;
    while (start != NULL)
    {
        if (start->queens == n)
        {
            if (verbose)
            {
                for (int q = 0; q < n; q++)
                {
                    if (start->queen_coords[q] + 1 == 10)
                    {
                        printf("A");
                    }
                    else
                    {
                        printf("%d", start->queen_coords[q] + 1);
                    }
                }
                printf("\n");
            }
            counter++;
        }
        start = start->next;
    }
    printf("%d solutions\n", counter);
}

Board *create_new_board(Board b)
{
    Board *new = (Board*) calloc(1, sizeof(Board));
    if (new == NULL)
    {
        fprintf(stderr, "error allocating space for new board\n");
        exit(EXIT_FAILURE);
    }

    new->queens = b.queens;
    for (int i = 0; i < MAX_QUEENS; i++)
    {
        new->queen_coords[i] = b.queen_coords[i];
    }

    new->next = NULL;
    return new;
}

void add_new_board(Board** start, Board candidate)
{
    Board *new = create_new_board(candidate);

    if (*start == NULL)
    {
        *start = new;
        return;
    }

    Board *current = *start;
    while (current->next != NULL)
    {
        current = current->next;
    }

    current->next = new;
}

void print_list(Board *location)
{
    while (location != NULL)
    {
        printf("%d currantz\n", location->queens);
        for (int i = 0; i < MAX_QUEENS; i++)
        {
            printf("q %d\n", location->queen_coords[i]);
        }
        location = location ->next;
    }
    printf("end.\n");
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

int add_initial_boards(Board **start, int *boards_to_add)
{
    int col = 0, queen = 0;
    int counter = 0;

    Board temp;

    for (int b = 0; b < *boards_to_add * *boards_to_add + 1; b++)
    {
        for (int i = 0; i < MAX_QUEENS; i++)
        {
            if (i < *boards_to_add)
            {
                temp.queen_coords[i] = UNUSED;
            }
            else
            {
                temp.queen_coords[i] = OUT_OF_BOUNDS;
            }
        }

        temp.queen_coords[queen] = col;   

        temp.queens = 1;

        //TODO refactor this
        if (col < *boards_to_add)
        {
            add_new_board(start, temp);
        }
        counter++;
        col++;
        if (col == *boards_to_add && queen < *boards_to_add - 1)
        {
            col = 0;
            queen++;
        }

    }
    return counter;
}

Board *end_of_list(Board **location)
{
    Board *end = *location;
    while (end->next != NULL)
    {
        end = end->next;
    }
    return end;
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

bool is_safe_space(int queen_coords[MAX_QUEENS], int row, int col, int *n)
{
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
        if (queen_coords[queen] != UNUSED && on_diagonals(queen, queen_coords[queen], row, col, n))
        {
            return false;
        }
    }
    return true;
}

void append_all_children(Board **current, Board **start, int *n)
{
    Board *head = *current;
    Board *start_of_list = *start;
    Board *end_board = end_of_list(current);
    for (int row = 0; row < *n; row++)
    {
        for (int col = 0; col < *n; col++)
        {
            if (is_safe_space(head->queen_coords, row, col, n))
            {
                Board candidate;
                candidate.queens = head->queens + 1;
                for (int q = 0; q < MAX_QUEENS; q++)
                {
                    candidate.queen_coords[q] = head->queen_coords[q];
                }
                candidate.queen_coords[row] = col;
                if (is_unique(candidate, &start_of_list))
                {
                    add_new_board(&end_board,candidate);
                }
            }
        }
    }
}

bool is_unique(Board candidate, Board **start)
{
    Board *s = *start;
    while ( s != NULL)
    {
        if (s->queens == candidate.queens
        && memcmp(s->queen_coords, candidate.queen_coords, MAX_QUEENS * sizeof(int)) == 0)
        {
            return false;
        }
        s = s->next;
    }
    return true;
}


// void test(void)
// {
//     int n_test;
//     bool verbose_test = false;

//     char *test_argv[] = {"./8q", "-verbose", "3"};
//     assert(parse_args(&n_test, test_argv, 3, &verbose_test) == true);
//     assert(n_test == 3);
//     assert(verbose_test = true);

//     verbose_test = false;
//     char *test_argv1[] = {"./8q", "10", "-verbose"};
//     assert(parse_args(&n_test, test_argv1, 3, &verbose_test) == true);
//     assert(n_test == 10);
//     assert(verbose_test = true);

//     verbose_test = false;
//     char *test_argv2[] = {"./8q", "11"};
//     assert(parse_args(&n_test, test_argv2, 3, &verbose_test) == false);
//     assert(n_test == 11);
//     assert(verbose_test == false);


//     static Board test_boards[BOARDS];
//     n_test = 10;
//     assert(add_initial_boards(test_boards, &n_test)== n_test *n_test + 1);
//     assert(test_boards[0].queens == 0);
//     assert(test_boards[0].in_use);
//     for (int row = 0; row < MAX_QUEENS; row++)
//     {
//         assert(test_boards[0].queen_coords[row] == UNUSED);
//     }
    
    
//     // queen on each board needs a unique location within an n*n board
//     static Board test_locations[BOARDS];
//     n_test = 8;
//     int initial_boards = add_initial_boards(test_locations, &n_test);
//     int col = 0, queen = 0;
//     for (int b = 1; b < initial_boards; b++)
//     {
//         assert(test_locations[b].queens == 1);
//         assert(test_locations[b].queen_coords[queen] == col);

//         for (int i = 0; i < MAX_QUEENS; i++)
//         {
//             if (i < n_test && i != queen)
//             {
//                 assert(test_locations[b].queen_coords[i] == UNUSED);
//             }
//             else if (i > n_test && i != queen)
//             {
//                 assert(test_locations[b].queen_coords[i] == OUT_OF_BOUNDS);
//             }
//         }

//         col++;

//         assert(test_locations[b].in_use);
//         if (col == n_test)
//         {
//             col = 0;
//             queen++;
//         }

//     }

//     // can add a queen in next safe, and unique location
//     // check starting location
//     n_test = 9;
//     assert(test_locations[1].queen_coords[0] == 0);

//     assert(on_diagonals(0,1,1,2,&n_test));
//     assert(on_diagonals(0,1,2,3,&n_test));
//     assert(!on_diagonals(4,4,2,3,&n_test));
//     assert(!on_diagonals(4,4,2,5,&n_test));
//     assert(!on_diagonals(4,4,6,3,&n_test));
//     assert(!on_diagonals(4,4,6,5,&n_test));
//     assert(on_diagonals(4,4,0,0,&n_test));
//     assert(on_diagonals(4,4,0,8,&n_test));
//     assert(on_diagonals(4,4,8,8,&n_test));
//     assert(on_diagonals(4,4,8,0,&n_test));

//     //is safe space
//     int queens[MAX_QUEENS] = {1,UNUSED,UNUSED,UNUSED};
//     int queens1[MAX_QUEENS] = {1,3,0,UNUSED};
//     n_test = 4;
//     // row
//     assert(!is_safe_space(queens,0,2,&n_test));
//     assert(!is_safe_space(queens,0,0,&n_test));
//     assert(is_safe_space(queens,1,3,&n_test));
//     //column
//     assert(!is_safe_space(queens,0,1,&n_test));
//     assert(!is_safe_space(queens,2,1,&n_test));
//     assert(!is_safe_space(queens,3,1,&n_test));
//     assert(is_safe_space(queens,3,2,&n_test));
//     //diagonal
//     assert(!is_safe_space(queens,2,3,&n_test));
//     assert(!is_safe_space(queens,1,2,&n_test));
//     assert(is_safe_space(queens1,3,2,&n_test));
//     assert(!is_safe_space(queens1,2,3,&n_test));

//     //index for next board
//     static Board board_index_n_3[BOARDS];
//     n_test = 3;
//     int initiate_boards = add_initial_boards(board_index_n_3, &n_test);
//     int next_index = next_available_index_in_array(board_index_n_3,0);
//     assert(next_index == initiate_boards);

//     static Board board_index_n_10[BOARDS];
//     n_test = 10;
//     initiate_boards = add_initial_boards(board_index_n_10, &n_test);
//     next_index = next_available_index_in_array(board_index_n_10,0);
//     assert(next_index == initiate_boards);

//     //append children
//     static Board test_child_boards[BOARDS];
//     n_test = 4;
//     int first_children = 6;
//     int children[6][MAX_QUEENS] = {
//         {0,2,UNUSED,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
//         {0,3,UNUSED,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
//         {0,UNUSED,1,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
//         {0,UNUSED,3,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
//         {0,UNUSED,UNUSED,1,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
//         {0,UNUSED,UNUSED,2,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS},
//     };
//     int parent_boards = add_initial_boards(test_child_boards, &n_test);
//     int n_index = 1;
//     append_all_children(test_child_boards, 1, &n_test,&n_index);
//     for (int i = 0; i < first_children; i++)
//     {
//         assert(memcmp(children[i], test_child_boards[parent_boards + i].queen_coords, first_children * sizeof(int)) == 0);
//     }
    
//     //TODO ask in lab if there is a better way to re-write:
//     test_child_boards[17].in_use = true;
//     test_child_boards[17].queens = 2;
//     test_child_boards[17].queen_coords[0] = 1;
//     test_child_boards[17].queen_coords[1] = 3;
//     test_child_boards[17].queen_coords[2] = UNUSED;
//     test_child_boards[17].queen_coords[3] = UNUSED;
//     test_child_boards[17].queen_coords[4] = OUT_OF_BOUNDS;
//     test_child_boards[17].queen_coords[5] = OUT_OF_BOUNDS;
//     test_child_boards[17].queen_coords[6] = OUT_OF_BOUNDS;
//     test_child_boards[17].queen_coords[7] = OUT_OF_BOUNDS;
//     test_child_boards[17].queen_coords[8] = OUT_OF_BOUNDS;
//     test_child_boards[17].queen_coords[9] = OUT_OF_BOUNDS;
//     int start_index_uniqueness = 0;
//     append_all_children(test_child_boards, 17, &n_test, &start_index_uniqueness);
//     int third_child[MAX_QUEENS] = {1,3,0,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS};
//     assert(memcmp(third_child, test_child_boards[23].queen_coords, MAX_QUEENS * sizeof(int)) == 0);
//     assert(test_child_boards[23].in_use);
//     assert(test_child_boards[23].queens == 3);

//     memcpy(test_child_boards[18].queen_coords, test_child_boards[17].queen_coords, MAX_QUEENS * sizeof(int));
//     test_child_boards[18].queen_coords[2] = 0;
//     test_child_boards[18].queens = 3;
//     append_all_children(test_child_boards, 18, &n_test,&start_index_uniqueness);
//     int fourth_child[MAX_QUEENS] = {1,3,0,2,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS};    
//     assert(memcmp(fourth_child, test_child_boards[26].queen_coords, MAX_QUEENS * sizeof(int)) == 0);
//     assert(test_child_boards[26].in_use);
//     assert(test_child_boards[26].queens == 4);


//     // is board unique?
//     static Board test_uniqueness[BOARDS] = {
//         {4,{UNUSED,3,0,2,4,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true},
//         {4,{3,UNUSED,0,2,4,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true},
//         {4,{1,UNUSED,0,2,4,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true},
//     };
//     n_test = 5;
//     Board test_unique_1 = {4,{1,2,3,4,UNUSED,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true};
//     Board test_unique_2 = {4,{3,UNUSED,0,2,4,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS,OUT_OF_BOUNDS}, true};
//     int x = 0;
//     assert(is_unique(test_unique_1, test_uniqueness,&x,2));
//     assert(!is_unique(test_unique_2, test_uniqueness,&x,2));    

// }
