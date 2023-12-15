#include "parse.h"

//TODO usage
//TODO only given input file 
//TODO given input and output file

int main(int argc, char **argv)
{
    test();
    if (argc == ONE_ARG)
    {
        return 1;
    }

    char instruction_file_name[FILE_NAME_LEN];
    strcpy(instruction_file_name, argv[1]);

    
    FILE *turtle_file = fopen(instruction_file_name, "r");
    if (!turtle_file)
    {
        fprintf(stderr, "error opening file\n");
        exit(EXIT_FAILURE);
    }

    Instructions *turtle = calloc(1, sizeof(Instructions));
    if (turtle == NULL)
    {
        fprintf(stderr, "error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    int token = 0;
    while (fscanf(turtle_file, "%s", turtle->words[token]) == 1)
    {
        printf("%s\n", turtle->words[token]);
        token++;
    }
    fclose(turtle_file);
    free(turtle);
    return 0;
}


// check grammar is correct
// The Formal Grammar is made up of lots of small parts
// the smallest units of the grammar are <NUM> <LTR> <OP> - they aren't made of other grammars
void test(void)
{

}

