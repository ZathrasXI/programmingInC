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
            inst->head = new_token(buffer);
            current = inst->head;
        }
        else
        {
            current->next = new_token(buffer);
            current = current->next;
        }
    }

    free_tokens(inst->head);
    fclose(turtle_file);
    free(inst);
    return 0;
}

Token *new_token(char *c)
{
    //TODO can I calculate len inside this funciton instead of passing it in
    int len = strlen(c);
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

bool is_forward(Token *t)
{
    char *fwd = "FORWARD";
    if (strcmp(fwd, t->str) == 0 &&
        is_varnum(t->next->str)
    )
    {
        return true;
    }
    return false;
}

bool is_rgt(Token *t)
{
    char *rgt = "RIGHT";
    if (strcmp(rgt, t->str) == 0 &&
        is_varnum(t->next->str)
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
    if (strcmp(t->str, "}") == 0 ||
        (t->next && 
        is_item(t->str) && 
        is_items(t->next)))
    {
        return true;
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

bool is_col(Token *t)
{
    //TODO do I need to check if valid colour?
    char *col = "COLOUR";
    if (strcmp(col, t->str) == 0 &&
        (
            is_var(t->next->str) ||
            is_word(t->next->str)
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

bool is_set(Token *t)
{
    if (!t->next)
    {
        return false;
    }
    if (
        strcmp(t->str, "SET") == 0 &&
        is_letter(t->next->str) &&
        strcmp(t->next->next->str, "(") == 0 &&
        is_pfix(t->next->next->next)
        )
        {
            return true;
        }
    return false;
}

bool is_loop(Token *t)
{
    if (strcmp(t->str, "LOOP") == 0 &&
        is_letter(t->next->str) &&
        strcmp(t->next->next->str, "OVER") == 0 &&
        is_lst(t->next->next->next)
        )
        {
            Token *end_of_lst = t->next->next->next;
            while (strcmp(end_of_lst->str, "}") != 0)
            {
                end_of_lst = end_of_lst->next;
            }
            if (is_inslst(end_of_lst->next))
            {
                return true;
            }
        }
    return false;
}

bool is_ins(Token *t)
{
    if (
        is_forward(t) ||
        is_rgt(t) ||
        is_col(t) ||
        is_loop(t) ||
        is_set(t)
    )
    {
        return true;
    }
    return false;
}

bool is_inslst(Token *t)
{
    if (strcmp("END", t->str) == 0)
    {
        return true;
    }

    Token *next_ins = t;
    if (is_forward(t))
    {
        printf("1\n");
        do
        {
            next_ins = next_ins->next;
        } 
        while (strcmp(next_ins->str, "FOWARD") == 0 || 
            is_varnum(next_ins->str));
    }
    else if (is_rgt(t))
    {
        printf("2\n");
        do
        {
            next_ins = next_ins->next;
        }
        while (strcmp(next_ins->str, "RIGHT") == 0 || 
            is_varnum(next_ins->str));
    }
    else if (is_col(t))
    {
        printf("3\n");
        do
        {
            next_ins = next_ins->next;
        } 
        while (strcmp(next_ins->str, "COLOUR") == 0 || 
            is_var(next_ins->str) || 
            is_word(next_ins->str));
    }
    else if (is_loop(t))
    {
        printf("4\n");
        do
        {
            next_ins = next_ins->next;
        } 
        while (strcmp(next_ins->str, "LOOP") == 0 || 
            is_letter(next_ins->str) || 
            strcmp(next_ins->str, "OVER") == 0 || 
            is_lst(next_ins) || 
            is_inslst(t));
    }
    else if (is_set(t))
    {
        printf("5\n");
        do
        {
            next_ins = next_ins->next;
        } 
        while (strcmp(next_ins->str, "SET") == 0 || 
            is_letter(next_ins->str) || 
            strcmp(next_ins->str, "(") || 
            is_pfix(next_ins));
    }

    if (is_ins(t) && is_inslst(next_ins))
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
    Token *n = new_token("abc");
    assert(n->length == 3);
    assert(strcmp(n->str, "abc") == 0);
    assert(n->next == NULL);

    n->next = new_token("defg");
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
    Token *fwd = new_token("FORWARD");
    Token *fwd1 = new_token("1");
    fwd->next=fwd1;
    assert(is_forward(fwd));
    free_tokens(fwd);

    Token *fwd2 = new_token("FORWARD");
    Token *fwd3 = new_token("$A");
    fwd2->next = fwd3;
    assert(is_forward(fwd2));
    free_tokens(fwd2);

    Token *fwd4 = new_token("FORWARD");
    Token *fwd5 = new_token("FOWARD");
    fwd4->next = fwd5;
    assert(!is_forward(fwd4));
    free_tokens(fwd4);

    /*
    is_rgt() <RGT> ::= "RIGHT" <VARNUM>
    */
    Token *rgt_test = new_token("RIGHT");
    Token *rgt_test1 = new_token("1");
    rgt_test->next=rgt_test1;
    assert(is_rgt(rgt_test));
    free_tokens(rgt_test);

    Token *rgt_test2 = new_token("RIGHT");
    Token *rgt_test3 = new_token("$A");
    rgt_test2->next=rgt_test3;
    assert(is_rgt(rgt_test2));
    free_tokens(rgt_test2);

    Token *rgt_test4 = new_token("RIGHT");
    Token *rgt_test5 = new_token("\"WORD\"");
    rgt_test4->next=rgt_test5;
    assert(!is_rgt(rgt_test4));
    free_tokens(rgt_test4);

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
    Token *items_test = new_token("}");
    assert(is_items(items_test));
    free_tokens(items_test);

    //false when next item == NULL
    Token *items_test1 = new_token("$A");
    assert(!is_items(items_test1));
    free_tokens(items_test1);

    // false next item is not an item or "}"
    Token *items_test2 = new_token("$A");
    Token *items_test3 = new_token("$A");
    Token *items_test4 = new_token("$A");
    Token *items_test5 = new_token("!");
    items_test2->next = items_test3;
    items_test3->next = items_test4;
    items_test4->next = items_test5;
    assert(!is_items(items_test2));
    free_tokens(items_test2);

    // true when "}" can be found
    Token *items_test6 = new_token("$A");
    Token *items_test7 = new_token("\"MAGENTA\"");
    items_test6->next = items_test7;
    Token *items_test8 = new_token("$C");
    items_test7->next = items_test8;
    Token *items_test9 = new_token("\"$D\"");
    items_test8->next = items_test9;
    Token *items_test10 = new_token("$E");
    items_test9->next = items_test10;
    Token *items_test11 = new_token("\"ONE\"");
    items_test10->next = items_test11;
    Token *items_test12 = new_token("}");
    items_test11->next = items_test12;
    assert(is_items(items_test6));
    free_tokens(items_test6);

    /*
    is_lst() <LST> ::= "{" <ITEMS>
    */
    // false - token with no nodes proceeding
    Token *lst_test = new_token("{");
    assert(!is_lst(lst_test));
    free_tokens(lst_test);

    // false when no closing "}"
    Token *lst_test1 = new_token("{");
    Token *lst_test2 = new_token("$A");
    Token *lst_test3= new_token("\"WORD\"");
    Token *lst_test4 = new_token("$B");
    Token *lst_test5 = new_token("\"178\"");
    Token *lst_test6 = new_token("$C");
    Token *lst_test7 = new_token("\"ONE\"");
    lst_test1->next = lst_test2;
    lst_test2->next = lst_test3;
    lst_test3->next = lst_test4;
    lst_test4->next = lst_test5;
    lst_test5->next = lst_test6;
    lst_test6->next = lst_test7;
    assert(!is_lst(lst_test1));
    free_tokens(lst_test1);

    // "{" followed by "}" is allowed
    Token *lst_test8 = new_token("{");
    Token *lst_test9 = new_token("}");
    lst_test8->next = lst_test9;
    assert(is_lst(lst_test8));
    free_tokens(lst_test8);

    // large list, encapsulated with "{" and "}"
    Token *lst_test10 = new_token("{");
    Token *lst_test11 = new_token("$A");
    Token *lst_test12 = new_token("\"WORD\"");
    Token *lst_test13 = new_token("$B");
    Token *lst_test14 = new_token("\"178\"");
    Token *lst_test15 = new_token("$C");
    Token *lst_test16 = new_token("}");
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
    Token *col_test = new_token("COLOUR");
    Token *col_test1 = new_token("$A");
    col_test->next=col_test1;
    assert(is_col(col_test));
    free_tokens(col_test);

    Token *col_test2 = new_token("COLOUR");
    Token *col_test3 = new_token("\"WORD\"");
    col_test2->next=col_test3;
    assert(is_col(col_test2));
    free_tokens(col_test2);

    Token *col_test4 = new_token("COLOUR");
    Token *col_test5 = new_token("END");
    col_test4->next=col_test5;
    assert(!is_col(col_test4));
    free_tokens(col_test4);

    /*
    is_pfix() <PFIX> ::= ")" | <OP> <PFIX> | <VARNUM> <PFIX>
    */
   // true when given ")"
    Token *pfix_test = new_token(")");
    assert(is_pfix(pfix_test));
    free_tokens(pfix_test);

    // true when <OP> ")"
    Token *pfix_test1 = new_token("+");
    Token *pfix_test2 = new_token(")");
    pfix_test1->next=pfix_test2;
    assert(is_pfix(pfix_test1));
    free_tokens(pfix_test1);

    // true when <VARNUM> and <OP> in list")"
    Token *pfix_test3 = new_token("$A");
    Token *pfix_test4 = new_token("10");
    Token *pfix_test5 = new_token("+");
    Token *pfix_test6 = new_token(")");
    pfix_test3->next = pfix_test4;
    pfix_test4->next = pfix_test5;
    pfix_test5->next = pfix_test6;
    assert(is_pfix(pfix_test3));
    free_tokens(pfix_test3);

    // false when no ")" at end
    Token *pfix_test7 = new_token("$A");
    Token *pfix_test8 = new_token("10");
    Token *pfix_test9 = new_token("+");
    pfix_test7->next = pfix_test8;
    pfix_test8->next = pfix_test9;
    assert(!is_pfix(pfix_test7));
    free_tokens(pfix_test7);

    /*
    is_set() <SET> ::= "SET" <LTR> "(" <PFIX>
    */
    Token *set_test = new_token("SET");
    Token *set_test1 = new_token("A");
    Token *set_test2 = new_token("(");
    Token *set_test3 = new_token("4");
    Token *set_test4 = new_token(")");
    set_test->next = set_test1;
    set_test1->next = set_test2;
    set_test2->next = set_test3;
    set_test3->next = set_test4;
    assert(is_set(set_test));
    free_tokens(set_test);

    Token *set_test5 = new_token("DESET");
    Token *set_test6 = new_token("A");
    Token *set_test7 = new_token("(");
    Token *set_test8 = new_token("4");
    Token *set_test9 = new_token(")");
    set_test5->next = set_test6;
    set_test6->next = set_test7;
    set_test7->next = set_test8;
    set_test8->next = set_test9;
    assert(!is_set(set_test5));
    free_tokens(set_test5);

    Token *set_test10 = new_token("SET");
    Token *set_test11 = new_token("A");
    Token *set_test12 = new_token("(");
    Token *set_test13 = new_token("4");
    Token *set_test14 = new_token("!");
    set_test10->next = set_test11;
    set_test11->next = set_test12;
    set_test12->next = set_test13;
    set_test13->next = set_test14;
    assert(!is_set(set_test10));
    free_tokens(set_test10);

    /*
    is_loop() <LOOP> ::= "LOOP" <LTR> "OVER" <LST> <INSLST>
    */
    //loop has list and list of instructions and end - true
    // Token *loop_test = new_token("LOOP");
    // Token *loop_test1 = new_token("A");
    // Token *loop_test2 = new_token("OVER");
    // Token *loop_test3 = new_token("{");
    // Token *loop_test4 = new_token("$A");
    // Token *loop_test5 = new_token("10");
    // Token *loop_test6 = new_token("\"PURPLE\"");
    // Token *loop_test7 = new_token("}");
    // Token *loop_test8 = new_token("FOWARD");
    // Token *loop_test9 = new_token("1");
    // // Token *loop_test10 = new_token("RIGHT");
    // // Token *loop_test11 = new_token("2");
    // // Token *loop_test12 = new_token("COLOUR");
    // // Token *loop_test13 = new_token("\"PURPLE\"");
    // // Token *loop_test14 = new_token("SET");
    // // Token *loop_test15 = new_token("A");
    // // Token *loop_test16 = new_token("(");
    // // Token *loop_test17 = new_token("10");
    // // Token *loop_test18 = new_token("$A");
    // // Token *loop_test19 = new_token("+");
    // // Token *loop_test20 = new_token(")");
    // Token *loop_test21 = new_token("END");
    // loop_test->next = loop_test1;
    // loop_test1->next = loop_test2;
    // loop_test2->next = loop_test3;
    // loop_test3->next = loop_test4;
    // loop_test4->next = loop_test5;
    // loop_test5->next = loop_test6;
    // loop_test6->next = loop_test7;
    // loop_test7->next = loop_test8;
    // loop_test8->next = loop_test9;
    // loop_test9->next = loop_test21;
    // // loop_test10->next = loop_test11;
    // // loop_test11->next = loop_test12;
    // // loop_test12->next = loop_test13;
    // // loop_test13->next = loop_test21;
    // // loop_test14->next = loop_test15;
    // // loop_test15->next = loop_test16;
    // // loop_test16->next = loop_test17;
    // // loop_test17->next = loop_test18;
    // // loop_test18->next = loop_test19;
    // // loop_test19->next = loop_test20;
    // // loop_test20->next = loop_test21;
    // // assert(is_loop(loop_test));
    // free_tokens(loop_test);

    //false when no "END" found
    // Token *loop_test9 = new_token("LOOP");
    // Token *loop_test10 = new_token("A");
    // Token *loop_test11 = new_token("OVER");
    // Token *loop_test12 = new_token("{");
    // Token *loop_test13 = new_token("$A");
    // Token *loop_test14 = new_token("10");
    // Token *loop_test15 = new_token("\"PURPLE\"");
    // Token *loop_test16 = new_token("}");
    // Token *loop_test17 = new_token("NED");
    // loop_test9->next = loop_test10;
    // loop_test10->next = loop_test11;
    // loop_test11->next = loop_test12;
    // loop_test12->next = loop_test13;
    // loop_test13->next = loop_test14;
    // loop_test14->next = loop_test15;
    // loop_test15->next = loop_test16;
    // loop_test16->next = loop_test17;
    // assert(!is_loop(loop_test9));
    // free_tokens(loop_test9);

    /*
    is_inslst() <INSLST> ::= "END" | <INS> <INSLST>
    */
    Token *inslst_test = new_token("END");
    assert(is_inslst(inslst_test));
    free_tokens(inslst_test);

    Token *inslst_test1 = new_token("RIGHT");
    Token *inslst_test2 = new_token("61");
    Token *inslst_test3 = new_token("FOWARD");
    Token *inslst_test4 = new_token("1");
    Token *inslst_test5 = new_token("COLOUR");
    Token *inslst_test6 = new_token("$A");
    Token *inslst_test7 = new_token("LOOP");
    Token *inslst_test8 = new_token("C");
    Token *inslst_test9 = new_token("OVER");
    Token *inslst_test10 = new_token("{");
    Token *inslst_test11 = new_token("1");
    Token *inslst_test12 = new_token("2");
    Token *inslst_test13 = new_token("3");
    Token *inslst_test14 = new_token("}");
    Token *inslst_test15 = new_token("SET");
    Token *inslst_test16 = new_token("Z");
    Token *inslst_test17 = new_token("(");
    Token *inslst_test18 = new_token("$A");
    Token *inslst_test19 = new_token("1");
    Token *inslst_test20 = new_token("/");
    Token *inslst_test21 = new_token(")");
    Token *inslst_test22 = new_token("END");
    inslst_test1->next=inslst_test2;
    inslst_test2->next=inslst_test3;
    inslst_test3->next=inslst_test4;
    inslst_test4->next=inslst_test5;
    inslst_test5->next=inslst_test6;
    inslst_test6->next=inslst_test7;
    inslst_test7->next=inslst_test8;
    inslst_test8->next=inslst_test9;
    inslst_test9->next=inslst_test10;
    inslst_test10->next=inslst_test11;
    inslst_test11->next=inslst_test12;
    inslst_test12->next=inslst_test13;
    inslst_test13->next=inslst_test14;
    inslst_test14->next=inslst_test15;
    inslst_test15->next=inslst_test16;
    inslst_test16->next=inslst_test17;
    inslst_test17->next=inslst_test18;
    inslst_test18->next=inslst_test19;
    inslst_test19->next=inslst_test20;
    inslst_test20->next=inslst_test21;
    inslst_test21->next=inslst_test22;
    assert(is_inslst(inslst_test1));
    free_tokens(inslst_test1);
}

