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


// check grammar is correct
// The Formal Grammar is made up of lots of small parts
// the smallest units of the grammar are <NUM> <LTR> <OP> - they aren't made of other grammars
void test(void)
{
    Token *n = new_token("abc", 3);
    assert(n->length == 3);
    assert(strcmp(n->t, "abc") == 0);
    assert(n->next == NULL);

    n->next = new_token("defg", 4);
    assert(n->next->length == 4);
    assert(strcmp(n->next->t, "defg") == 0);
    assert(n->next->next == NULL);
    free_tokens(n);
}

