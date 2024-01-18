
void test(void)
{
    test_stack();
    test_ttl();
    test_new_token();
    test_is_num();
    test_is_letter();
    test_is_op();
    test_is_var();
    test_is_varnum();
    test_deg_2_rad();
    test_next_row_index();
    test_next_col_index();
    test_is_fwd();
    test_is_rgt();
    test_get_var_index();
    test_is_word();
    test_is_item();
    test_items();
    test_is_lst();
    test_is_col();
    test_is_pfix();
    test_is_set();
    test_update_var();
    test_is_loop();
    test_loop_closed();
    // test_is_inslst();
    // test_is_prog();
}

void test_stack(void)
{
    //can init
    Stack *s_test = stack_init();
    assert(s_test);
    //teardown
    free(s_test);

    //is empty on init
    Stack *empty = stack_init();
    assert(!is_full(empty->top));
    assert(is_empty(empty->top));
    //teardown
    free(empty);

    //is full
    Stack *full = stack_init();
    full->top = MAX_STACK_SIZE - 1;
    assert(is_full(full->top));
    free(full);

    //push
    Stack *push_test = stack_init();
    assert(push(push_test, 123.123));
    assert(fabs(push_test->data[push_test->top] - 123.123) < TOLERANCE);
    //false when stack is full
    push_test->top = MAX_STACK_SIZE -1;
    assert(!push(push_test, 23.0));
    free(push_test);

    /*
    pop()
    */
    //can pop an item
    Stack *pop_test = stack_init();
    assert(push(pop_test, 123));
    assert(fabs(pop(pop_test) - 123) < TOLERANCE);
    free(pop_test);

    //when stack is already empty
    //program will exit with message
    //"can't pop when stack is empty"
    // Stack *pop_test1 = stack_init();
    // pop(pop_test1);
    // free(pop_test1);

    /*
    evaluate()
    */
    // can evaluate expression of numbers...
    // addition
    char *exp[] = {"2", "2", "+", "2", "+"};
    int len = 5;
    assert(fabs(evaluate(exp, len) - 6) < TOLERANCE);
    //multiplication
    char *exp2[] = {"2", "2", "*", "2", "*"};
    assert(fabs(evaluate(exp2, len) - 8) < TOLERANCE);
    //division
    char *exp3[] = {"2", "2", "/", "2", "/"};
    assert(fabs(evaluate(exp3, len) - 0.5) < TOLERANCE);
    //subtraction
    char *exp4[] = {"2", "2", "-", "2", "-"};
    assert(fabs(evaluate(exp4, len) + 2) < TOLERANCE);

    //can evaluate an expression of variables that contain numbers
    // 2 2 + 3 * 2 / 6 - = 0
    ttl.vars[2].num = 2; //$C
    ttl.type_in_use[2] = union_double;
    ttl.vars[3].num = 3; //$D
    ttl.type_in_use[3] = union_double;
    ttl.vars[6].num = 6; //$G
    ttl.type_in_use[6] = union_double;
    char *exp5[] = {"$C", "$C", "+", "$D", "*", "$C", "/", "$G", "-"};
    assert(fabs(evaluate(exp5, 9) - 0) < TOLERANCE);

    //program exits when variable contains a word
    // char *stack_word = "STACK";
    // ttl.vars[2].word = calloc(strlen(stack_word) + 1, sizeof(char));
    // ttl.type_in_use[2] = union_char;
    // if (!ttl.vars[2].word)
    // {
    //     panic_msg("allocating space for word in stack test");
    // }
    // ttl.type_in_use[2] = union_char;
    // char *exp6[] = {"2", "$C", "+"};
    // evaluate(exp6, strlen(stack_word) + 1);

    //program exits when type_in_use not yet set
    // char *type_not_in_use[] = {"$C", "3", "-"};
    // ttl.type_in_use[2] = not_set;
    // evaluate(type_not_in_use, 3);

}

void test_ttl(void)
{
    /*
    TTL is initialised with correct values
    */
    assert(ttl.len == 0);
    assert((int) ttl.direction == 0);
    assert(ttl.capacity == PATH);
    assert(ttl.colour == 'W');
    assert(ttl.path != NULL);
}

void test_new_token(void)
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
}

void test_is_num(void)
{
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
}

void test_is_letter(void)
{
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
}

void test_is_op(void)
{
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
}

void test_is_var(void)
{
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
}

void test_is_varnum(void)
{
    /*
    is_varnum() <VARNUM> ::= <VAR> | <NUM>
    */
    assert(is_varnum("$A"));
    assert(is_varnum("1"));
    assert(!is_varnum("$1"));
    assert(!is_varnum("asdf$1fsafasdf"));    
}

void test_deg_2_rad(void)
{
    /*
    can convert degrees to radians
    */
    assert(fabs(0 * (PI/180) - degrees_to_radians(0)) < TOLERANCE);
    assert(fabs(359 * (PI/180) - degrees_to_radians(359)) < TOLERANCE);
    assert(fabs(62 * (PI/180) - degrees_to_radians(62)) < TOLERANCE);
    assert(fabs(PI - degrees_to_radians(180)) < TOLERANCE);
}

void test_next_row_index(void)
{
    /*
    get index for row
    */
    //1 step facing north
    ttl.direction = 0;
    assert(next_row(ROW_START, 1) == ROW_START - 1);
    //1 step east
    ttl.direction = degrees_to_radians(90);
    assert(next_row(ROW_START, 1) == ROW_START);
    //1 step south
    ttl.direction = degrees_to_radians(180);
    assert(next_row(ROW_START, 1) == ROW_START + 1);
    //1 step west
    ttl.direction = degrees_to_radians(270);
    assert(next_row(ROW_START, 1) == ROW_START);
    //1 step NE
    ttl.direction = degrees_to_radians(45);
    assert(next_row(ROW_START, 1) == ROW_START - 1);
    //teardown
    ttl.direction = 0;    
}

void test_next_col_index(void)
{
    /*
    get index for column
    */
    // 1 step north
    ttl.direction = 0;
    assert(next_col(COL_START, 1) == COL_START);
    // 1 step NE
    ttl.direction = degrees_to_radians(45);
    assert(next_col(COL_START, 1) == COL_START + 1);
    //1 step south
    ttl.direction = degrees_to_radians(180);
    assert(next_col(COL_START, 1) == COL_START);
    //1 step west
    ttl.direction = degrees_to_radians(270);
    assert(next_col(COL_START, 1) == COL_START -1 );
    ttl.direction = 0;    
}

void test_is_fwd(void)
{
    /*
    is_forward() <FWD> ::= "FORWARD" <VARNUM>
    */
    //can store forward step
    Token *fwd = new_token("FORWARD");
    Token *fwd1 = new_token("1");
    fwd->next=fwd1;
    assert(is_forward(fwd));
    assert(ttl.len == 2);
    assert(ttl.path[0].col == COL_START);
    assert(ttl.path[0].row == ROW_START);
    
    //teardown
    ttl.len = 0;
    free_tokens(fwd);

    // 4 steps northward
    int steps = 4;
    Token *fwd_many = new_token("FORWARD");
    Token *fwd_many1 = new_token("4");
    fwd_many->next=fwd_many1;
    assert(is_forward(fwd_many));
    assert(ttl.len == steps + 1);
    for (int i = 0; i < steps; i++)
    {
        assert(ttl.path[i].row == ROW_START - i);
        assert(ttl.path[i].col == COL_START);
    }
    //teardown
    ttl.len = 0;
    free_tokens(fwd_many);
    
    //forward, turn 90, forward
    Token *fwd_rgt_fwd = new_token("FORWARD");
    Token *fwd_rgt_fwd1 = new_token("1");
    Token *fwd_rgt_fwd2 = new_token("RIGHT");
    Token *fwd_rgt_fwd3 = new_token("90");
    Token *fwd_rgt_fwd4 = new_token("FORWARD");
    Token *fwd_rgt_fwd5 = new_token("1");
    fwd_rgt_fwd->next = fwd_rgt_fwd1;
    fwd_rgt_fwd1->next = fwd_rgt_fwd2;
    fwd_rgt_fwd2->next = fwd_rgt_fwd3;
    fwd_rgt_fwd3->next = fwd_rgt_fwd4;
    fwd_rgt_fwd4->next = fwd_rgt_fwd5;
    assert(is_forward(fwd_rgt_fwd));
    assert(is_rgt(fwd_rgt_fwd2));
    assert(is_forward(fwd_rgt_fwd4));
    assert(ttl.len == 3);
    assert(ttl.path[0].col == COL_START);
    assert(ttl.path[0].row == ROW_START);
    assert(ttl.path[1].col == COL_START);
    assert(ttl.path[1].row == ROW_START - 1);
    assert(ttl.path[2].col == COL_START + 1);
    assert(ttl.path[2].row == ROW_START - 1);
    //teardown
    ttl.len = 0;
    ttl.direction = 0;
    free_tokens(fwd_rgt_fwd);

    //awkward angle
    //forward 1, right 62, forward 4
    Token *fwd_rgt_fwd6 = new_token("FORWARD");
    Token *fwd_rgt_fwd7 = new_token("1");
    Token *fwd_rgt_fwd8 = new_token("RIGHT");
    Token *fwd_rgt_fwd9 = new_token("62");
    Token *fwd_rgt_fwd10 = new_token("FORWARD");
    Token *fwd_rgt_fwd11 = new_token("4");
    fwd_rgt_fwd6->next = fwd_rgt_fwd7;
    fwd_rgt_fwd7->next = fwd_rgt_fwd8;
    fwd_rgt_fwd8->next = fwd_rgt_fwd9;
    fwd_rgt_fwd9->next = fwd_rgt_fwd10;
    fwd_rgt_fwd10->next = fwd_rgt_fwd11;
    assert(is_forward(fwd_rgt_fwd6));
    assert(is_rgt(fwd_rgt_fwd8));
    assert(is_forward(fwd_rgt_fwd10));
    assert(ttl.len == 6);
    assert(ttl.path[0].row == ROW_START);
    assert(ttl.path[0].col == COL_START);
    assert(ttl.path[1].row == ROW_START - 1);
    assert(ttl.path[1].col == COL_START);
    assert(ttl.path[2].row == ROW_START - 1);
    assert(ttl.path[2].col == COL_START + 1);
    assert(ttl.path[3].row == ROW_START - 1);
    assert(ttl.path[3].col == COL_START + 2);
    assert(ttl.path[4].row == ROW_START - 2);
    assert(ttl.path[4].col == COL_START + 3);
    assert(ttl.path[5].row == ROW_START - 2);
    assert(ttl.path[5].col == COL_START + 4);
    //teardown
    ttl.len = 0;
    ttl.direction = 0;
    free_tokens(fwd_rgt_fwd6);

    //turtle doesn't move when var == 0
    Token *fwd2 = new_token("FORWARD");
    Token *fwd3 = new_token("$A");
    fwd2->next = fwd3;
    assert(is_forward(fwd2));
    assert(ttl.len == 0);
    //teardown
    free_tokens(fwd2);
    
    Token *fwd4 = new_token("FORWARD");
    Token *fwd5 = new_token("FOWARD");
    fwd4->next = fwd5;
    assert(!is_forward(fwd4));
    free_tokens(fwd4);    
}

void test_is_rgt(void)
{
/*
    is_rgt() <RGT> ::= "RIGHT" <VARNUM>
    */
    ttl.direction = 0;
    double one_degree_in_rads = 0.017453;
    Token *rgt_test = new_token("RIGHT");
    Token *rgt_test1 = new_token("1");
    rgt_test->next=rgt_test1;
    assert(is_rgt(rgt_test));
    assert(fabs(ttl.direction - one_degree_in_rads) < TOLERANCE);
    //teardown
    ttl.direction = 0;
    free_tokens(rgt_test);
    
    // no rotation when variable == 0
    double zero_direction = 0.0;
    Token *rgt_test2 = new_token("RIGHT");
    Token *rgt_test3 = new_token("$A");
    rgt_test2->next=rgt_test3;
    assert(is_rgt(rgt_test2));
    assert(fabs(zero_direction - ttl.direction) < TOLERANCE);
    //teardown
    free_tokens(rgt_test2);

    //direction == $var converted to rads
    ttl.vars[0].num = 359;
    double rads = degrees_to_radians(ttl.vars[0].num);
    Token *rgt_test4 = new_token("RIGHT");
    Token *rgt_test5 = new_token("$A");
    rgt_test4->next=rgt_test5;
    assert(is_rgt(rgt_test4));
    assert(fabs(ttl.direction - rads) < TOLERANCE);
    //teardown
    ttl.direction = 0;
    free_tokens(rgt_test4);


    Token *rgt_test6 = new_token("RIGHT");
    Token *rgt_test7 = new_token("\"WORD\"");
    rgt_test6->next=rgt_test7;
    assert(!is_rgt(rgt_test6));
    free_tokens(rgt_test6);    
}

void test_get_var_index(void)
{
    /*
    get index of a variable,e.g. index of $A
    */
    char *vars[] = {"$A", "$B", "$C", "$D", "$E", "$F", "$G", "$H", "$I", "$J", 
    "$K", "$L", "$M", "$N", "$O", "$P", "$Q", "$R", "$S", "$T", "$U", "$V", "$W", "$X", "$Y", "$Z"};
    for (int i = 0; i < 26; i++)
    {
        assert(get_var_index(vars[i][1]) == i);
    }    
}

void test_is_word(void)
{
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
}

void test_is_item(void)
{
    /*
    is_item() <ITEM> ::= <VARNUM> | <WORD>
    */
    assert(is_item("$A"));
    assert(is_item("\"a\""));
    assert(is_item("99"));
    assert(!is_item("$1"));
    assert(!is_item("a"));
    assert(!is_item("9A"));    
}

void test_items(void)
{
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
}

void test_is_lst(void)
{
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
}

void test_is_col(void)
{
/*
    is_col() <COL> ::= "COLOUR" <VAR> | "COLOUR" <WORD>
    */
    //ttl.colour is updated when given valid colour via variable
    ttl.len = 0;
    char *magenta = "\"MAGENTA\"";
    int len = strlen(magenta) + 1;
    ttl.vars[25].word = calloc(len, sizeof(char));
    if (!ttl.vars[25].word)
    {
        panic_msg("allocating memory for test");
    }
    strcpy(ttl.vars[25].word, magenta);
    Token *col_test = new_token("COLOUR");
    Token *col_test1 = new_token("$Z");
    col_test->next=col_test1;
    assert(is_col(col_test));
    assert(ttl.colour == 'M');
    free(ttl.vars[25].word);
    free_tokens(col_test);

    //ttl.colour not updated when given invalid colour via variable
    ttl.len = 0;
    char *rust = "\"RUST\"";
    len = strlen(rust) + 1;
    ttl.vars[0].word = calloc(len, sizeof(char));
    if (!ttl.vars[0].word)
    {
        panic_msg("allocating memory for test");
    }
    strcpy(ttl.vars[0].word, rust);
    Token *col_test8 = new_token("COLOUR");
    Token *col_test9 = new_token("$A");
    col_test8->next=col_test9;
    assert(is_col(col_test8));
    assert(ttl.colour == 'W');
    free(ttl.vars[0].word);
    free_tokens(col_test8);


    //ttl.colour is updated when given valid colour directly
    ttl.len = 0;
    Token *col_test6 = new_token("COLOUR");
    Token *col_test7 = new_token("\"CYAN\"");
    col_test6->next=col_test7;
    assert(is_col(col_test6));
    assert(ttl.colour == 'C');
    free_tokens(col_test6);

    //ttl.colour not updated when given invalid colour directly
    Token *col_test2 = new_token("COLOUR");
    Token *col_test3 = new_token("\"FOX\"");
    col_test2->next=col_test3;
    assert(is_col(col_test2));
    assert(ttl.colour == 'W');
    free_tokens(col_test2);
    
    // invalid syntax
    Token *col_test4 = new_token("COLOUR");
    Token *col_test5 = new_token("END");
    col_test4->next=col_test5;
    assert(!is_col(col_test4));
    free_tokens(col_test4);    
}

void test_is_pfix(void)
{
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
}

void test_is_set(void)
{
/*
    is_set() <SET> ::= "SET" <LTR> "(" <PFIX>
    */
    //can directly assign a number to a var
    ttl.vars[0].num = 0.0;
    ttl.type_in_use[0] = union_char;
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
    assert(fabs(ttl.vars[0].num - 4.0) < TOLERANCE);
    assert(ttl.type_in_use[0] == union_double);
    //teardown
    ttl.vars[0].num = 0.0;
    free_tokens(set_test);

    //can evaluate postfix expressions
    ttl.vars[0].num = 0.0;
    ttl.type_in_use[0] = union_char;
    Token *set_test25 = new_token("SET");
    Token *set_test26 = new_token("A");
    Token *set_test27 = new_token("(");
    Token *set_test28 = new_token("4");
    Token *set_test29 = new_token("4");
    Token *set_test30 = new_token("*");
    Token *set_test31 = new_token("4");
    Token *set_test32 = new_token("+");
    Token *set_test33 = new_token(")");
    set_test25->next = set_test26;
    set_test26->next = set_test27;
    set_test27->next = set_test28;
    set_test28->next = set_test29;
    set_test29->next = set_test30;
    set_test30->next = set_test31;
    set_test31->next = set_test32;
    set_test32->next = set_test33;
    assert(is_set(set_test25));
    assert(fabs(ttl.vars[0].num - 20.0) < TOLERANCE);
    assert(ttl.type_in_use[0] == union_double);
    //teardown
    ttl.vars[0].num = 0.0;
    free_tokens(set_test25);

    //can evaluate postfix expression that contains a variable
    //variable has a num value
    int j_index = get_var_index('J');
    ttl.vars[j_index].num = 31.0;
    ttl.type_in_use[j_index] = union_double;
    int s_index = get_var_index('S');
    ttl.type_in_use[s_index] = union_char;

    Token *set_test34 = new_token("SET");
    Token *set_test35 = new_token("S");
    Token *set_test36 = new_token("(");
    Token *set_test37 = new_token("2");
    Token *set_test38 = new_token("2");
    Token *set_test39 = new_token("*");
    Token *set_test40 = new_token("$J");
    Token *set_test41 = new_token("+");
    Token *set_test42 = new_token(")");
    set_test34->next = set_test35;
    set_test35->next = set_test36;
    set_test36->next = set_test37;
    set_test37->next = set_test38;
    set_test38->next = set_test39;
    set_test39->next = set_test40;
    set_test40->next = set_test41;
    set_test41->next = set_test42;
    assert(is_set(set_test34));
    assert(fabs(ttl.vars[s_index].num - 35.0) < TOLERANCE);
    assert(ttl.type_in_use[s_index] == union_double);
    //teardown
    ttl.vars[j_index].num = 0.0;
    ttl.vars[s_index].num = 0.0;
    free_tokens(set_test34);

    //can assign word value of one variable to another variable
    char *test_word = "ZOOBZOOB";
    int d_index = get_var_index('D');
    int z_index = get_var_index('Z');
    ttl.type_in_use[d_index] = union_char;
    ttl.vars[d_index].word = calloc((int) strlen(test_word) + 1, sizeof(char));
    if (!ttl.vars[d_index].word)
    {
        panic_msg("allocating string for test");
    }
    strcpy(ttl.vars[d_index].word, test_word);
    Token *set_test15 = new_token("SET");
    Token *set_test16 = new_token("Z");
    Token *set_test17 = new_token("(");
    Token *set_test18 = new_token("$D");
    Token *set_test19 = new_token(")");
    set_test15->next = set_test16;
    set_test16->next = set_test17;
    set_test17->next = set_test18;
    set_test18->next = set_test19;
    assert(is_set(set_test15));
    assert(ttl.type_in_use[z_index] == union_char);
    assert(strcmp(ttl.vars[z_index].word, ttl.vars[d_index].word) == 0);
    //teardown
    free(ttl.vars[d_index].word);
    free(ttl.vars[z_index].word);
    free_tokens(set_test15);

    //can assign number of one variable to another variable
    int m_index = get_var_index('M');
    ttl.vars[m_index].num = 3.142;
    ttl.type_in_use[m_index] = union_double;
    z_index = get_var_index('Z');
    ttl.vars[z_index].num = 10.0;
    ttl.type_in_use[z_index] = union_double;

    Token *set_test20 = new_token("SET");
    Token *set_test21 = new_token("Z");
    Token *set_test22 = new_token("(");
    Token *set_test23 = new_token("$M");
    Token *set_test24 = new_token(")");
    set_test20->next = set_test21;
    set_test21->next = set_test22;
    set_test22->next = set_test23;
    set_test23->next = set_test24;
    assert(is_set(set_test20));
    assert(fabs(ttl.vars[m_index].num - ttl.vars[z_index].num) < TOLERANCE);
    assert(ttl.type_in_use[m_index] == union_double);
    assert(ttl.type_in_use[m_index] == ttl.type_in_use[z_index]);
    //teardown
    ttl.vars[m_index].num = 0.0;
    ttl.vars[z_index].num = 0.0;
    free_tokens(set_test20);

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
}

void test_update_var(void)
{
   /*
    update_var() 
    can update var with a word, num, value of another variable
    */
    int dest_index = get_var_index('L');
    char *tokens[] = {"\"PURPLE\"", "10", "$C"};
    
    //can update var with literal word
    update_var(tokens[0], dest_index);
    assert(strcmp(ttl.vars[dest_index].word, tokens[0]) == 0);
    assert(ttl.type_in_use[dest_index] == union_char);
    
    //can update var with literal num
    update_var(tokens[1], dest_index);
    assert(fabs(10.0 - ttl.vars[dest_index].num) < TOLERANCE);
    assert(ttl.type_in_use[dest_index] == union_double);
    
    //can update var with num from another var
    int src_index = get_var_index('C');
    ttl.vars[src_index].num = 31.75;
    ttl.type_in_use[src_index] = union_double;
    update_var(tokens[2], dest_index);
    assert(fabs(ttl.vars[src_index].num - ttl.vars[dest_index].num) < TOLERANCE);
    assert(ttl.type_in_use[dest_index] == union_double);

    //can update var with word from another var
    ttl.vars[src_index].word = calloc(strlen("\"BANDANA\"") + NULL_CHAR, sizeof(char));
    if (!ttl.vars[src_index].word)
    {
        panic_msg("allocating memory for word in test");
    }
    ttl.type_in_use[src_index] = union_char;
    update_var(tokens[2], dest_index);
    assert(strcmp(ttl.vars[src_index].word, ttl.vars[src_index].word) == 0);
    assert(ttl.type_in_use[src_index] == union_char); 
    free(ttl.vars[src_index].word);
}

void test_is_loop(void)
{
    /*
    is_loop() <LOOP> ::= "LOOP" <LTR> "OVER" <LST> <INSLST>
    */
    //value of A is updated
    int a_index = get_var_index('A');
    int c_index = get_var_index('C');
    ttl.vars[c_index].num = 100.01;
    ttl.type_in_use[c_index] = union_double;
    char *final_value = "\"PURPLE\"";
    Token *loop_test31 = new_token("LOOP");
    Token *loop_test32 = new_token("A");
    Token *loop_test33 = new_token("OVER");
    Token *loop_test34 = new_token("{");
    Token *loop_test35 = new_token("$C");
    Token *loop_test36 = new_token("10");
    Token *loop_test37 = new_token("\"PURPLE\"");
    Token *loop_test38 = new_token("}");
    Token *loop_test39 = new_token("END");
    loop_test31->next = loop_test32;
    loop_test32->next = loop_test33;
    loop_test33->next = loop_test34;
    loop_test34->next = loop_test35;
    loop_test35->next = loop_test36;
    loop_test36->next = loop_test37;
    loop_test37->next = loop_test38;
    loop_test38->next = loop_test39;
    assert(is_loop(loop_test31));
    assert(strcmp(final_value, ttl.vars[a_index].word) == 0);
    assert(ttl.type_in_use[a_index] == union_char);
    free_tokens(loop_test31);

    //loop has list and list of instructions and end - true
    Token *loop_test = new_token("LOOP");
    Token *loop_test1 = new_token("A");
    Token *loop_test2 = new_token("OVER");
    Token *loop_test3 = new_token("{");
    Token *loop_test4 = new_token("1");
    Token *loop_test5 = new_token("2");
    Token *loop_test6 = new_token("3");
    Token *loop_test7 = new_token("}");
    Token *loop_test8 = new_token("FORWARD");
    Token *loop_test9 = new_token("2");
    Token *loop_test10 = new_token("RIGHT");
    Token *loop_test11 = new_token("45");
    Token *loop_test12 = new_token("COLOUR");
    Token *loop_test13 = new_token("$A");
    Token *loop_test14 = new_token("FORWARD");
    Token *loop_test15 = new_token("2");
    Token *loop_test16 = new_token("FORWARD");
    Token *loop_test17 = new_token("2");
    Token *loop_test18 = new_token("FORWARD");
    Token *loop_test19 = new_token("2");
    Token *loop_test20 = new_token("END");
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
}

void test_loop_closed(void)
{
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
}

void test_is_inslst(void)
{
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
    Token *inslst_test6 = new_token("\"RED\"");
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
    Token *inslst_test18 = new_token("$C");
    Token *inslst_test19 = new_token("1");
    Token *inslst_test20 = new_token("+");
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
}

void test_is_prog(void)
{
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










