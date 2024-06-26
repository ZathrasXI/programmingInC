#include "parse.h"

int main(int argc, char **argv)
{
    test();
    if (argc == ONE_ARG)
    {
        printf("Usage: ./parser <turtle file>\n");
        exit(EXIT_FAILURE);
    }

    FILE *turtle_file = fopen(argv[1], "r");
    if (!turtle_file)
    {
        fprintf(stderr, "error opening file\n");
        exit(EXIT_FAILURE);
    }
    Token *head = tokenise(turtle_file);

    if (!is_prog(head))
    {
        fprintf(stderr, "file not parsed\n");
        exit(EXIT_FAILURE);
    }

    free_tokens(head);
    fclose(turtle_file);
    return 0;
}

Token *new_token(char *c)
{
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
    return new;
}

Token *tokenise(FILE *ttl_file)
{
    char buffer[TOKEN_LEN];
    Token *current = NULL;
    Token *head = NULL;
    while (fscanf(ttl_file, "%s", buffer) == 1)
    {
        if (!head)
        {
            head = new_token(buffer);
            current = head;
        }
        else
        {
            current->next = new_token(buffer);
            current = current->next;
        }
    }
    return head;
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
        loop_closed(t) &&
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

bool loop_closed(Token *t)
{
    int loop_depth = 0;
    while (t)
    {
        if (strcmp(t->str, "LOOP") == 0)
        {
            loop_depth++;
        }
        if (strcmp(t->str, "END") == 0)
        {
            loop_depth--;
            if (loop_depth == 0)
            {
                return true;
            }
        }
        t = t->next;
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
        do
        {
            next_ins = next_ins->next;
        } 
        while (strcmp(next_ins->str, "FOWARD") == 0 || 
            is_varnum(next_ins->str));
    }
    else if (is_rgt(t))
    {
        do
        {
            next_ins = next_ins->next;
        }
        while (strcmp(next_ins->str, "RIGHT") == 0 || 
            is_varnum(next_ins->str));
    }
    else if (is_col(t))
    {
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
        do
        {
            next_ins = next_ins->next;
        } 
        while (strcmp(next_ins->str, "}") != 0);
        next_ins = next_ins->next;
    }
    else if (is_set(t))
    {
        do
        {
            next_ins = next_ins->next;
        } 
        while (strcmp(next_ins->str, ")") != 0);
        next_ins = next_ins->next;
    }
    if (is_ins(t) && is_inslst(next_ins))
    {
        return true;
    }
    return false;
}

bool is_prog(Token *t)   
{
    if (strcmp(t->str, "START") == 0 && 
        t->next &&
        is_inslst(t->next))
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
    assert(strcmp(n->str, "abc") == 0);
    assert(n->next == NULL);

    n->next = new_token("defg");
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
    Token *loop_test = new_token("LOOP");
    Token *loop_test1 = new_token("A");
    Token *loop_test2 = new_token("OVER");
    Token *loop_test3 = new_token("{");
    Token *loop_test4 = new_token("$A");
    Token *loop_test5 = new_token("10");
    Token *loop_test6 = new_token("\"PURPLE\"");
    Token *loop_test7 = new_token("}");
    Token *loop_test8 = new_token("FORWARD");
    Token *loop_test9 = new_token("1");
    Token *loop_test10 = new_token("RIGHT");
    Token *loop_test11 = new_token("2");
    Token *loop_test12 = new_token("COLOUR");
    Token *loop_test13 = new_token("\"PURPLE\"");
    Token *loop_test14 = new_token("SET");
    Token *loop_test15 = new_token("A");
    Token *loop_test16 = new_token("(");
    Token *loop_test17 = new_token("10");
    Token *loop_test18 = new_token("$A");
    Token *loop_test19 = new_token("+");
    Token *loop_test20 = new_token(")");
    Token *loop_test21 = new_token("END");
    loop_test->next = loop_test1;
    loop_test1->next = loop_test2;
    loop_test2->next = loop_test3;
    loop_test3->next = loop_test4;
    loop_test4->next = loop_test5;
    loop_test5->next = loop_test6;
    loop_test6->next = loop_test7;
    loop_test7->next = loop_test8;
    loop_test8->next = loop_test9;
    loop_test9->next = loop_test10;
    loop_test10->next = loop_test11;
    loop_test11->next = loop_test12;
    loop_test12->next = loop_test13;
    loop_test13->next = loop_test14;
    loop_test14->next = loop_test15;
    loop_test15->next = loop_test16;
    loop_test16->next = loop_test17;
    loop_test17->next = loop_test18;
    loop_test18->next = loop_test19;
    loop_test19->next = loop_test20;
    loop_test20->next = loop_test21;
    assert(is_loop(loop_test));
    free_tokens(loop_test);

    //false when no "END" found
    Token *loop_test22 = new_token("LOOP");
    Token *loop_test23 = new_token("A");
    Token *loop_test24 = new_token("OVER");
    Token *loop_test25 = new_token("{");
    Token *loop_test26 = new_token("$A");
    Token *loop_test27 = new_token("10");
    Token *loop_test28 = new_token("\"PURPLE\"");
    Token *loop_test29 = new_token("}");
    Token *loop_test30 = new_token("NED");
    loop_test22->next = loop_test23;
    loop_test23->next = loop_test24;
    loop_test24->next = loop_test25;
    loop_test25->next = loop_test26;
    loop_test26->next = loop_test27;
    loop_test27->next = loop_test28;
    loop_test28->next = loop_test29;
    loop_test29->next = loop_test30;
    assert(!is_loop(loop_test22));
    free_tokens(loop_test22);

    /*
    loop_closed()
    */
    Token *closed_loop = new_token("LOOP");
    Token *closed_loop1 = new_token("END");
    closed_loop->next = closed_loop1;
    assert(loop_closed(closed_loop));
    free_tokens(closed_loop);

    Token *closed_loop2 = new_token("LOOP");
    Token *closed_loop3 = new_token("LOOP");
    Token *closed_loop4 = new_token("LOOP");
    Token *closed_loop5 = new_token("END");
    Token *closed_loop6 = new_token("END");
    Token *closed_loop7 = new_token("NOT_THE_END");
    closed_loop2->next = closed_loop3;
    closed_loop3->next = closed_loop4;
    closed_loop4->next = closed_loop5;
    closed_loop5->next = closed_loop6;
    closed_loop6->next = closed_loop7;
    assert(!loop_closed(closed_loop2));
    free_tokens(closed_loop2);

    Token *closed_loop8 = new_token("LOOP");
    Token *closed_loop9 = new_token("LOOP");
    Token *closed_loop10 = new_token("LOOP");
    Token *closed_loop11 = new_token("END");
    Token *closed_loop12 = new_token("END");
    Token *closed_loop13 = new_token("END");
    closed_loop8->next = closed_loop9;
    closed_loop9->next = closed_loop10;
    closed_loop10->next = closed_loop11;
    closed_loop11->next = closed_loop12;
    closed_loop12->next = closed_loop13;
    assert(loop_closed(closed_loop8));
    assert(loop_closed(closed_loop9));
    assert(loop_closed(closed_loop10));
    free_tokens(closed_loop8);

    /*
    is_inslst() <INSLST> ::= "END" | <INS> <INSLST>
    */
    //true when str == END
    Token *inslst_test = new_token("END");
    assert(is_inslst(inslst_test));
    free_tokens(inslst_test);

    // true when given valid ins and for loop
    Token *inslst_test1 = new_token("FORWARD");
    Token *inslst_test2 = new_token("1");
    Token *inslst_test3 = new_token("RIGHT");
    Token *inslst_test4 = new_token("61.0");
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

    //nested for loop
    Token *nestd_loop = new_token("LOOP");
    Token *nestd_loop2 = new_token("C");
    Token *nestd_loop3 = new_token("OVER");
    Token *nestd_loop4 = new_token("{");
    Token *nestd_loop5 = new_token("\"BLUE\"");
    Token *nestd_loop6 = new_token("\"GREEN\"");
    Token *nestd_loop7 = new_token("}");
    Token *nestd_loop8 = new_token("COLOUR");
    Token *nestd_loop9 = new_token("$C");
    Token *nestd_loop10 = new_token("LOOP");
    Token *nestd_loop11 = new_token("Z");
    Token *nestd_loop12 = new_token("OVER");
    Token *nestd_loop13 = new_token("{");
    Token *nestd_loop14 = new_token("1");
    Token *nestd_loop15 = new_token("2");
    Token *nestd_loop16 = new_token("3");
    Token *nestd_loop17 = new_token("}");
    Token *nestd_loop18 = new_token("FORWARD");
    Token *nestd_loop19 = new_token("$Z");
    Token *nestd_loop20 = new_token("RIGHT");
    Token *nestd_loop21 = new_token("$A");
    Token *nestd_loop22 = new_token("END");
    Token *nestd_loop23 = new_token("END");
    nestd_loop->next=nestd_loop2;
    nestd_loop2->next=nestd_loop3;
    nestd_loop3->next=nestd_loop4;
    nestd_loop4->next=nestd_loop5;
    nestd_loop5->next=nestd_loop6;
    nestd_loop6->next=nestd_loop7;
    nestd_loop7->next=nestd_loop8;
    nestd_loop8->next=nestd_loop9;
    nestd_loop9->next=nestd_loop10;
    nestd_loop10->next=nestd_loop11;
    nestd_loop11->next=nestd_loop12;
    nestd_loop12->next=nestd_loop13;
    nestd_loop13->next=nestd_loop14;
    nestd_loop14->next=nestd_loop15;
    nestd_loop15->next=nestd_loop16;
    nestd_loop16->next=nestd_loop17;
    nestd_loop17->next=nestd_loop18;
    nestd_loop18->next=nestd_loop19;
    nestd_loop19->next=nestd_loop20;
    nestd_loop20->next=nestd_loop21;
    nestd_loop21->next=nestd_loop22;
    nestd_loop22->next=nestd_loop23;
    assert(is_inslst(nestd_loop));
    free_tokens(nestd_loop);

    /*(is_prog) "START" <INSLST>
    */

    Token *start = new_token("START");
    assert(!is_prog(start));
    free_tokens(start);

   //nested for loop with start and end
    Token *prog = new_token("START"); 
    Token *prog1 = new_token("LOOP");
    Token *prog2 = new_token("C");
    Token *prog3 = new_token("OVER");
    Token *prog4 = new_token("{");
    Token *prog5 = new_token("\"BLUE\"");
    Token *prog6 = new_token("\"GREEN\"");
    Token *prog7 = new_token("}");
    Token *prog8 = new_token("COLOUR");
    Token *prog9 = new_token("$C");
    Token *prog10 = new_token("LOOP");
    Token *prog11 = new_token("Z");
    Token *prog12 = new_token("OVER");
    Token *prog13 = new_token("{");
    Token *prog14 = new_token("1");
    Token *prog15 = new_token("2");
    Token *prog16 = new_token("3");
    Token *prog17 = new_token("}");
    Token *prog18 = new_token("FORWARD");
    Token *prog19 = new_token("$Z");
    Token *prog20 = new_token("RIGHT");
    Token *prog21 = new_token("$A");
    Token *prog22 = new_token("END");
    Token *prog23 = new_token("END");
    Token *prog24 = new_token("END");
    prog->next=prog1;
    prog1->next=prog2;
    prog2->next=prog3;
    prog3->next=prog4;
    prog4->next=prog5;
    prog5->next=prog6;
    prog6->next=prog7;
    prog7->next=prog8;
    prog8->next=prog9;
    prog9->next=prog10;
    prog10->next=prog11;
    prog11->next=prog12;
    prog12->next=prog13;
    prog13->next=prog14;
    prog14->next=prog15;
    prog15->next=prog16;
    prog16->next=prog17;
    prog17->next=prog18;
    prog18->next=prog19;
    prog19->next=prog20;
    prog20->next=prog21;
    prog21->next=prog22;
    prog22->next=prog23;
    prog23->next=prog24;
    assert(is_prog(prog));
    free_tokens(prog);


   // instructions before nested for loop
    Token *full_prog1 = new_token("START");
    Token *full_prog2 = new_token("FORWARD");
    Token *full_prog3 = new_token("1");
    Token *full_prog4 = new_token("RIGHT");
    Token *full_prog5 = new_token("10");
    Token *full_prog6 = new_token("LOOP");
    Token *full_prog7 = new_token("C");
    Token *full_prog8 = new_token("OVER");
    Token *full_prog9 = new_token("{");
    Token *full_prog10 = new_token("\"BLUE\"");
    Token *full_prog11 = new_token("\"GREEN\"");
    Token *full_prog12 = new_token("}");
    Token *full_prog13 = new_token("COLOUR");
    Token *full_prog14 = new_token("$C");
    Token *full_prog15 = new_token("LOOP");
    Token *full_prog16 = new_token("Z");
    Token *full_prog17 = new_token("OVER");
    Token *full_prog18 = new_token("{");
    Token *full_prog19 = new_token("1");
    Token *full_prog20 = new_token("2");
    Token *full_prog21 = new_token("3");
    Token *full_prog22 = new_token("}");
    Token *full_prog23 = new_token("FORWARD");
    Token *full_prog24 = new_token("$Z");
    Token *full_prog25 = new_token("RIGHT");
    Token *full_prog26 = new_token("$A");
    Token *full_prog27 = new_token("END");
    Token *full_prog28 = new_token("END");
    Token *full_prog29 = new_token("END");
    full_prog1->next=full_prog2;
    full_prog2->next=full_prog3;
    full_prog3->next=full_prog4;
    full_prog4->next=full_prog5;
    full_prog5->next=full_prog6;
    full_prog6->next=full_prog7;
    full_prog7->next=full_prog8;
    full_prog8->next=full_prog9;
    full_prog9->next=full_prog10;
    full_prog10->next=full_prog11;
    full_prog11->next=full_prog12;
    full_prog12->next=full_prog13;
    full_prog13->next=full_prog14;
    full_prog14->next=full_prog15;
    full_prog15->next=full_prog16;
    full_prog16->next=full_prog17;
    full_prog17->next=full_prog18;
    full_prog18->next=full_prog19;
    full_prog19->next=full_prog20;
    full_prog20->next=full_prog21;
    full_prog21->next=full_prog22;
    full_prog22->next=full_prog23;
    full_prog23->next=full_prog24;
    full_prog24->next=full_prog25;
    full_prog25->next=full_prog26;
    full_prog26->next=full_prog27;
    full_prog27->next=full_prog28;
    full_prog28->next=full_prog29;
    assert(is_prog(full_prog1));
    free_tokens(full_prog1);

}

