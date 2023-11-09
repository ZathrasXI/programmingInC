#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define BOARDS 1000000

// I am making an array to represent the unique locations a queen can be in. starting with 1 queen, ending on `n` queens


typedef struct board
{
    int queens;
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

    Board unique_locations[BOARDS];
    int initial_boards = add_initial_boards(unique_locations, &n);
    for (int i = 0; i < initial_boards; i++)
    {
        printf("%d\n",unique_locations[i].queens);
    }

    if (verbose)
    {
        printf("n = %d verbose = %d\n", n, verbose);
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
    unique_boards[0].queens = 0;
    counter++;
    for (int i = 1; i <= *boards_to_add; i++)
    {
        unique_boards[i].queens = 1;
        counter++;
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


    Board test_boards[BOARDS];
    n_test = 10;
    assert(add_initial_boards(test_boards, &n_test)== n_test + 1);
    assert(add_initial_boards(test_boards, &n_test)!= n_test);
}
