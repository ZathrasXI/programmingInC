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
    new->str = (char *) calloc(len + 1, sizeof(char));
    if (!new->str)
    {
        fprintf(stderr, "error allocating memory\n");
        exit(EXIT_FAILURE);
    }
    strcpy(new->str, c);
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
        free(tmp->str);
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

bool is_word(char *c)
{   
    int len = strlen(c);
    for (int i = 0; i < len; i++)
    {
        if (c[i] == SPACE_ASCII)
        {
            return false;
        }
    }

    char tmp[TOKEN_LEN];
    int final_char = len - 1;
    if (c[0] == '\"' &&
        c[final_char] == '\"' &&
        len > STR_ONLY_QUOTES &&
        sscanf(c, "%s", tmp) == 1)
    {
        return true;
    }
    return false;
}

bool is_item(char *c)
{
    if (is_varnum(c) || is_word(c))
    {
        return true;
    }
    return false;
}

bool is_items(Token *t)
{
    if (strcmp(t->str, "}") == 0)
    {
        return true;
    }
    else if (t->next)
    {
        if (is_item(t->str) && is_items(t->next))
        {
            return true;
        }
    }
    return false;
}

bool is_lst(Token *t)
{
    if (t->next &&
        strcmp(t->str, "{") == 0 &&
        is_items(t->next)
    )
    {
        return true;
    }
    return false;
}

bool is_col(char *c)
{
    char *col = "COLOUR";
    int len = strlen(col);
    int i_next_str = len + 1;
    if (strncmp(col, c, len) == 0 &&
        (
            is_var(c + i_next_str) ||
            is_word(c + i_next_str)
        )
    )
    {
        return true;
    }

    return false;
}

bool is_pfix(Token *t)
{
    if (strcmp(")", t->str) == 0 ||
            (t->next &&
                (
                    (is_op(t->str) && is_pfix(t->next)) ||
                    (is_varnum(t->str) && is_pfix(t->next))
                )
            )
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
    assert(strcmp(n->str, "abc") == 0);
    assert(n->next == NULL);

    n->next = new_token("defg", 4);
    assert(n->next->length == 4);
    assert(strcmp(n->next->str, "defg") == 0);
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
    
    /*
    is_word() a string as defined by scanf("%s"), must be encapsulated by ""
    */
    assert(is_word("\"potatoes\""));
    assert(is_word("\"178\""));
    assert(is_word("\"RED!\""));
    assert(!is_word("\"BIRD IS THE ____\""));
    assert(!is_word("178"));
    assert(!is_word("\"\""));
    assert(!is_word("potatoes"));

    /*
    is_item() <ITEM> ::= <VARNUM> | <WORD>
    */
    assert(is_item("$A"));
    assert(is_item("\"a\""));
    assert(is_item("99"));
    assert(!is_item("$1"));
    assert(!is_item("a"));
    assert(!is_item("9A"));

    /*
    is_items() <ITEMS> ::= '}' | <ITEM> <ITEMS>
    */
    Token *items_test = new_token("}", 1);
    assert(is_items(items_test));
    free_tokens(items_test);

    //false when next item == NULL
    Token *items_test1 = new_token("$A", 2);
    assert(!is_items(items_test1));
    free_tokens(items_test1);

    // false next item is not an item or "}"
    Token *items_test2 = new_token("$A", 2);
    Token *items_test3 = new_token("$A", 2);
    Token *items_test4 = new_token("$A", 2);
    Token *items_test5 = new_token("!", 1);
    items_test2->next = items_test3;
    items_test3->next = items_test4;
    items_test4->next = items_test5;
    assert(!is_items(items_test2));
    free_tokens(items_test2);

    // true when "}" can be found
    Token *items_test6 = new_token("$A", 2);
    Token *items_test7 = new_token("\"MAGENTA\"", 9);
    items_test6->next = items_test7;
    Token *items_test8 = new_token("$C", 2);
    items_test7->next = items_test8;
    Token *items_test9 = new_token("\"$D\"", 4);
    items_test8->next = items_test9;
    Token *items_test10 = new_token("$E", 2);
    items_test9->next = items_test10;
    Token *items_test11 = new_token("\"ONE\"", 5);
    items_test10->next = items_test11;
    Token *items_test12 = new_token("}", 1);
    items_test11->next = items_test12;
    assert(is_items(items_test6));
    free_tokens(items_test6);

    /*
    is_lst() <LST> ::= "{" <ITEMS>
    */
    // false - token with no nodes proceeding
    Token *lst_test = new_token("{", 1);
    assert(!is_lst(lst_test));
    free_tokens(lst_test);

    // false when no closing "}"
    Token *lst_test1 = new_token("{", 1);
    Token *lst_test2 = new_token("$A", 2);
    Token *lst_test3= new_token("\"WORD\"", 6);
    Token *lst_test4 = new_token("$B", 2);
    Token *lst_test5 = new_token("\"178\"", 5);
    Token *lst_test6 = new_token("$C", 2);
    Token *lst_test7 = new_token("\"ONE\"", 5);
    lst_test1->next = lst_test2;
    lst_test2->next = lst_test3;
    lst_test3->next = lst_test4;
    lst_test4->next = lst_test5;
    lst_test5->next = lst_test6;
    lst_test6->next = lst_test7;
    assert(!is_lst(lst_test1));
    free_tokens(lst_test1);

    // "{" followed by "}" is allowed
    Token *lst_test8 = new_token("{", 1);
    Token *lst_test9 = new_token("}", 1);
    lst_test8->next = lst_test9;
    assert(is_lst(lst_test8));
    free_tokens(lst_test8);

    // large list, encapsulated with "{" and "}"
    Token *lst_test10 = new_token("{", 1);
    Token *lst_test11 = new_token("$A", 2);
    Token *lst_test12 = new_token("\"WORD\"", 6);
    Token *lst_test13 = new_token("$B", 2);
    Token *lst_test14 = new_token("\"178\"", 5);
    Token *lst_test15 = new_token("$C", 2);
    Token *lst_test16 = new_token("}", 1);
    lst_test10->next = lst_test11;
    lst_test11->next = lst_test12;
    lst_test12->next = lst_test13;
    lst_test13->next = lst_test14;
    lst_test14->next = lst_test15;
    lst_test15->next = lst_test16;
    assert(is_lst(lst_test10));
    free_tokens(lst_test10);

    /*
    is_col() <COL> ::= "COLOUR" <VAR> | "COLOUR" <WORD>
    */
    assert(is_col("COLOUR $A"));
    assert(is_col("COLOUR \"WOOORD\""));
    assert(!is_col("COLOUR WOOORD"));
    assert(!is_col("COLOUR "));
    assert(!is_col("COLOUR$A"));

    /*
    is_pfix() <PFIX> ::= ")" | <OP> <PFIX> | <VARNUM> <PFIX>
    */
   // true when given ")"
    Token *pfix_test = new_token(")", 1);
    assert(is_pfix(pfix_test));
    free_tokens(pfix_test);

    // true when <OP> ")"
    Token *pfix_test1 = new_token("+", 1);
    Token *pfix_test2 = new_token(")", 1);
    pfix_test1->next=pfix_test2;
    assert(is_pfix(pfix_test1));
    free_tokens(pfix_test1);

    // true when <VARNUM> and <OP> in list")"
    Token *pfix_test3 = new_token("$A", 2);
    Token *pfix_test4 = new_token("10", 2);
    Token *pfix_test5 = new_token("+", 2);
    Token *pfix_test6 = new_token(")", 2);
    pfix_test3->next = pfix_test4;
    pfix_test4->next = pfix_test5;
    pfix_test5->next = pfix_test6;
    assert(is_pfix(pfix_test3));
    free_tokens(pfix_test3);

    // false when no ")" at end
    Token *pfix_test7 = new_token("$A", 2);
    Token *pfix_test8 = new_token("10", 2);
    Token *pfix_test9 = new_token("+", 1);
    pfix_test7->next = pfix_test8;
    pfix_test8->next = pfix_test9;
    assert(!is_pfix(pfix_test7));
    free_tokens(pfix_test7);
}

