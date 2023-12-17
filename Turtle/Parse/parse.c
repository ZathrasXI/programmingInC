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

bool is_number(char *c)
{
    regex_t regex;
    char *pattern = "^[+-]?([0-9]+(\\.?[0-9]*))$";

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
    {
        fprintf(stderr, "failed to compile regex pattern\n");
        exit(EXIT_FAILURE);
    }

    if (regexec(&regex, c, 0, NULL, 0) == 0)
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

bool is_letter(char *c)
{
    regex_t regex;
    char *pattern = "^([A-Z])$";

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
    {
        fprintf(stderr, "failed to compile regex pattern\n");
        exit(EXIT_FAILURE);
    }

    if (regexec(&regex, c, 0, NULL, 0) == 0)
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

bool is_op(char *c)
{
    regex_t regex;
    char *pattern = "^(-|\\+|\\/|\\*){1}$";

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
    {
        fprintf(stderr, "failed to compile regex pattern\n");
        exit(EXIT_FAILURE);
    }

    if (regexec(&regex, c, 0, NULL, 0) == 0)
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

bool is_var(char *c)
{
    if (c[0] == '$' && is_letter(c+1))
    {
        return true;
    }
    return false;
}

bool is_varnum(char *c)
{
    if (is_var(c) || is_number(c))
    {
        return true;
    }
    return false;
}

bool is_forward(char *c)
{
    char *fwd = "FORWARD";
    int len = strlen(fwd);
    int len_inc_space = len + 1;
    if (strncmp(fwd, c, len) == 0
        && is_varnum(c + len_inc_space)
    )
    {
        return true;
    }
    return false;
}

bool is_rgt(char *c)
{
    char *rgt = "RIGHT";
    int len = strlen(rgt);
    int len_inc_space = len + 1;
    if (strncmp(rgt, c, len) == 0
        && is_varnum(c + len_inc_space)
    )
    {
        return true;
    }
    return false;
}

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
    assert(is_number("123"));
    assert(is_number("-17.99"));
    assert(is_number("-123.1"));
    assert(!is_number("abc"));
    assert(!is_number("abc.abc"));
    assert(!is_number("1.abc"));
    assert(!is_number("-abc"));

    /*
    is_letter() identifies single upper case letter
    */
    assert(is_letter("A"));
    assert(is_letter("Z"));
    assert(!is_letter("a"));
    assert(!is_letter("z"));
    assert(!is_letter("123"));
    assert(!is_letter("A.1"));
    assert(!is_letter("1.A"));
    assert(!is_letter("-A"));

    /*
    is_op() <OP>::= + - / *
    */
    assert(is_op("+"));
    assert(is_op("-"));
    assert(is_op("/"));
    assert(is_op("*"));
    assert(!is_op("++"));
    assert(!is_op("abc-"));
    assert(!is_op("1/"));
    assert(!is_op("*2"));

    /*
    is_var() <VAR>::= $<LTR>
    */
    assert(is_var("$A"));
    assert(is_var("$Z"));
    assert(!is_var("$a"));
    assert(!is_var("A$"));
    assert(!is_var("$1"));
    assert(!is_var("$$"));
    assert(!is_var("A"));

    /*
    is_varnum() <VARNUM> ::= <VAR> | <NUM>
    */
    assert(is_varnum("$A"));
    assert(is_varnum("1"));
    assert(!is_varnum("$1"));
    assert(!is_varnum("asdf$1fsafasdf"));

    /*
    is_forward() <FWD> ::= "FORWARD" <VARNUM>
    */
    assert(is_forward("FORWARD 1"));
    assert(is_forward("FORWARD $A"));
    assert(!is_forward("FORWARD $1"));
    assert(!is_forward("FORWARD$$"));
    assert(!is_forward(" FORWARD$$"));
    assert(!is_forward("FORWRD $A"));

    /*
    is_rgt() <RGT> ::= "RIGHT" <VARNUM>
    */
    assert(is_rgt("RIGHT 1"));
    assert(is_rgt("RIGHT $A"));
    assert(!is_rgt("RIGHT $1"));
    assert(!is_rgt("RIGHT$$"));
    assert(!is_rgt(" RIGHT$$"));
    assert(!is_rgt("RGT $A"));
    
}

