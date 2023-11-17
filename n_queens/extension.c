#include "extension.h"


int main(int argc, char* argv[])
{
    test();
    int n;
    bool verbose = false;
    bool valid_input = parse_args(&n, argv, argc, &verbose);

    if (!valid_input)
    {
        fprintf(stderr,"usage: ./ext <1-10> <optional: -verbose>\n");
        exit(EXIT_FAILURE);
    }

    Board *start = NULL;
    Board zero;
    zero.queens = 0;
    for (int q = 0; q < MAX_QUEENS; q++)
    {
        if (q < n)
        {
            zero.queen_coords[q] = UNUSED;
        }
        else
        {
            zero.queen_coords[q] = OUT_OF_BOUNDS;
        }
    }
    add_new_board(&start,zero);

    int initial_boards = add_initial_boards(&start, n);
    if (initial_boards != n * n + 1)
    {
        fprintf(stderr, "%d boards not initialised\n", initial_boards);
        exit(EXIT_FAILURE);
    }

    Board *current = start;
    while (current != NULL)
    {
        append_all_children(&current, &start, n);
        current = current->next;
    }

    print_solved_nodes(start, n, verbose);

    if (!free_list(start))
    {
        fprintf(stderr, "error freeing allocated memory for the linked list\n");
        exit(EXIT_FAILURE);
    }

    return 0;
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

int add_initial_boards(Board **start, int n)
{
    int col = 0, queen = 0;
    int counter = 0;

    Board temp;

    for (int b = 0; b < n * n + 1; b++)
    {
        for (int i = 0; i < MAX_QUEENS; i++)
        {
            if (i < n)
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

        if (col < n)
        {
            add_new_board(start, temp);
        }
        counter++;
        col++;
        if (col == n && queen < n - 1)
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
    for (int queen = 0; queen < n; queen++)
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

void append_all_children(Board **current, Board **start, int n)
{
    Board *head = *current;
    Board *start_of_list = *start;
    Board *end_board = end_of_list(current);
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
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
        if (s->queens == candidate.queens &&
            memcmp(s->queen_coords, candidate.queen_coords, MAX_QUEENS * sizeof(int)) == 0)
        {
            return false;
        }
        s = s->next;
    }
    return true;
}

bool free_list(Board *start)
{
    if (start)
    {
        Board *delete = start;
        while (delete != NULL)
        {
            Board *tmp = delete->next;
            free(delete);
            delete = tmp;
        }
    }
    return true;
}

void test(void)
{
    int n_test;
    // only accept an int 1-10, and "-verbose", these can be in any order
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


    // the space is not safe if there is already a queen on the diagonal
    n_test = 9;
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

    Board *start = NULL;
    Board one = {0};
    Board two = {0}; 
    Board three = {0};
    add_new_board(&start,one);
    add_new_board(&start,two);
    add_new_board(&start,three);
    
    int counter = 0;
    Board *current = start;
    while (current != NULL)
    {
        counter++;
        current = current->next;
    }
    bool list_freed = free_list(start);
    assert(counter == 3);
    assert(list_freed);
}
