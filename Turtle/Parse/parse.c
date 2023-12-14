#include "parse.h"

//TODO usage
//TODO only given input file 
//TODO given input and output file

int main(int argc, char **argv)
{
    if (argc == ONE_ARG)
    {
        return 1;
    }

    char instruction_file_name[FILE_NAME_LEN];
    strcpy(instruction_file_name, argv[1]);
    
    FILE * turtle_file = fopen(instruction_file_name, "r");
    if (!turtle_file)
    {
        fprintf(stderr, "error opening file\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}


// check grammar is correct
// The Formal Grammar is made up of lots of small parts
//