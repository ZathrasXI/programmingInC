#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

void test(void);
bool parse_input(int *n, char* argv[], int argc, bool *verbose);

int main(int argc, char* argv[])
{
    test();
    int n;
    bool verbose = false;
    bool acceptable_input = parse_input(&n, argv, argc, &verbose);


    if (verbose)
    {
        printf("%d", verbose);
    }

}

bool parse_input(int *n, char* argv[], int argc, bool *verbose)
{
    bool verbose = false;
    for (int arg = 0; arg < argc; arg++)
    {
        if (strcmp(argv[arg], "-verbose") == 0)
        {
            verbose = true;
        }
        if (sscanf(argv[arg], "%d", &n) == 1)
        {
            *n = argv[arg] - '0';
        }
    }

    if (argc < 2 || argc > 3 || (argc == 3 && !verbose))
    {
        fprintf(stderr, "usage: ./8q <1-10> <optional: -verbose>\n");
        exit(EXIT_FAILURE);
        verbose = false;
    }
    return verbose;
}

void test(void)
{
    int n;
    char *test_argv[] = {"./8q", "-verbose", "3"};
    assert(parse_input(&n, test_argv, 3) == true);
    assert(n == 3);

    char *test_argv1[] = {"./8q", "10", "-verbose"};
    assert(parse_input(&n, test_argv1, 3) == true);
    printf("n == %d\n", n);
    assert(n == 10);
}
