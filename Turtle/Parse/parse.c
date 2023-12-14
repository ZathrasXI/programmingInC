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
// the smallest units of the grammar are <NUM> <LTR> <OP> - they aren't made of other grammars
void test(void)
{
    assert(!is_number("a"));
    assert(is_number("0"));
}

bool is_number(char s[TOKEN_LEN])
{
    

    return false;
}