#include "parse.h"

//TODO usage
//TODO error message and EXIT for alloc()
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

    Instructions *inst = calloc(INIT_SIZE, sizeof(Instructions));
    if (inst == NULL)
    {
        fprintf(stderr, "error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    char buffer[TOKEN_LEN];
    Token *current;
    while (fscanf(turtle_file, "%s", buffer) == 1)
    {
        if (!inst->head)
        {
            inst->head = new_token(buffer, (int) strlen(buffer));
            current = inst->head;
        }
        else
        {
            current->next = new_token(buffer, (int) strlen(buffer));
            current = current->next;
        }
    }

    free_tokens(inst->head);
    fclose(turtle_file);
    free(inst);
    return 0;
}

Token *new_token(char *c, int len)
{
    Token *new = (Token *) calloc(INIT_SIZE, sizeof(Token));
    if (!new)
    {
        fprintf(stderr, "error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    new->t = (char *) calloc(len + 1, sizeof(char));
    if (!new->t)
    {
        fprintf(stderr, "error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    strcpy(new->t, c);
    new->next = NULL;
    new->length = len;
    return new;
}

void free_tokens(Token* head)
{
    Token *tmp;
    while (head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp->t);
        free(tmp);
    }
}

bool is_number(Token *t)
{
    regex_t regex;
    char *pattern = "^[+-]?([0-9]+(\\.?[0-9]*))$";

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
    {
        fprintf(stderr, "failed to compile regex pattern\n");
        exit(EXIT_FAILURE);
    }

    if (regexec(&regex, t->t, 0, NULL, 0) == 0)
    {   
        regfree(&regex);
        return true;
    }
    else
    {
        regfree(&regex);
        return false;
    }
}


// check grammar is correct
// The Formal Grammar is made up of lots of small parts
// the smallest units of the grammar are <NUM> <LTR> <OP> - they aren't made of other grammars
void test(void)
{
    /*
    can store a token in a node
    */
    Token *n = new_token("abc", 3);
    assert(n->length == 3);
    assert(strcmp(n->t, "abc") == 0);
    assert(n->next == NULL);

    n->next = new_token("defg", 4);
    assert(n->next->length == 4);
    assert(strcmp(n->next->t, "defg") == 0);
    assert(n->next->next == NULL);
    free_tokens(n);

    /*
    is_number() identifies decimal and float
    */
    Token *number_test = new_token("123", 3);
    assert(is_number(number_test));
    free_tokens(number_test);

    Token *number_test_1 = new_token("-123.1", 6);
    assert(is_number(number_test_1));
    free_tokens(number_test_1);

    Token *number_test_2 = new_token("abc", 3);
    assert(!is_number(number_test_2));
    free_tokens(number_test_2);

    Token *number_test_3 = new_token("abc.abc", 7);
    assert(!is_number(number_test_3));
    free_tokens(number_test_3);

    Token *number_test_4 = new_token("1.abc", 5);
    assert(!is_number(number_test_4));
    free_tokens(number_test_4);

    Token *number_test_5 = new_token("-abc", 6);
    assert(!is_number(number_test_5));
    free_tokens(number_test_5);

    Token *number_test_6 = new_token("-abc", 6);
    assert(!is_number(number_test_6));
    free_tokens(number_test_6);
}

