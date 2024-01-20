#include "interp.h"
#include "stack.c"
#include "test_interp.c"


int main(int argc, char **argv)
{
    test();
    init_ttl();

    if (argc == ONE_ARG)
    {
        printf("Usage: ./interpreter <turtle file>\n");
        exit(EXIT_FAILURE);
    }
    
    FILE *turtle_file = fopen(argv[1], "r");
    if (!turtle_file)
    {
        panic_msg("opening file");
    }

    Token *head = tokenise(turtle_file);

    exit(EXIT_FAILURE);

    if (!is_prog(head))
    {
        panic_msg("file not parsed");
    }

    free_tokens(head);
    free_ttl();
    return 0;
}

Token *new_token(char *c)
{
    int len = strlen(c) + 1;
    Token *new = (Token *) calloc(INIT_SIZE, sizeof(Token));
    if (!new)
    {
        panic_msg("allocating memory for Token");
    }
    new->str = calloc(len, sizeof(char));
    if (!new->str)
    {
        panic_msg("allocating memory a Token's string");
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
        panic_msg("failed to compile regex pattern");
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
        panic_msg("failed to compile regex pattern");
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
        panic_msg("failed to compile regex pattern");
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
        int steps;
        if (is_number(t->next->str))
        {
            sscanf(t->next->str, "%d", &steps);
        }
        else if (is_var(t->next->str))
        {
            steps = ttl.vars[get_var_index(t->next->str[1])].num;
        }
        if (steps > 0)
        {
            //TODO: put this in init()
            if (ttl.len == 0)
            {
                ttl.path[ttl.len].col = COL_START;
                ttl.path[ttl.len].row = ROW_START;
                ttl.len++;
            }
            // int row_line_start = next_row(ttl.path[ttl.len-1].row, 1);
            // int col_line_start = next_col(ttl.path[ttl.len-1].col, 1);
            // for (int i = 0; i < steps; i++)
            // {
            //     ttl.path[ttl.len].row = next_row(row_line_start, i);
            //     ttl.path[ttl.len].col = next_col(col_line_start, i);
            //     ttl.len++;
            // }
            int x1_y1[2];
            find_end_points(ttl.path[ttl.len-1].col, ttl.path[ttl.len-1].row, steps, x1_y1);
            calulcate_line_coords(ttl.path[ttl.len-1].col, ttl.path[ttl.len-1].row, x1_y1[0], x1_y1[1]);

        }
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
        double degrees;
        if (is_number(t->next->str))
        {
            sscanf(t->next->str, "%lf", &degrees);
        }
        else 
        {
            degrees = ttl.vars[get_var_index(t->next->str[1])].num;
        }
        ttl.direction += degrees_to_radians(degrees);
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
        //is valid colour?
        char *str;
        if (is_var(t->next->str))
        {
            int index = get_var_index(t->next->str[1]);
            int len = strlen(ttl.vars[index].word) + 1;
            str = calloc(len, sizeof(char));
            if (!str)
            {
                panic_msg("allocating memory for string (colour)");
            }
            strcpy(str, ttl.vars[index].word);
        }
        else
        {
            int len = strlen(t->next->str) + 1;
            str = calloc(len, sizeof(char));
            if (!str)
            {
                panic_msg("allocating memory for string (colour)");
            }
            strcpy(str, t->next->str);
        }
        /*
        % Valid colours include "BLACK", "RED", "GREEN", "BLUE",
        % "YELLOW", "CYAN", "MAGENTA", "WHITE"
        */
       //TODO check that letters are correct
       //TODO enumerate letters
        if (strcmp(str, "\"BLACK\"") == 0)
        {
            ttl.colour = 'K';
        }
        else if (strcmp(str, "\"RED\"") == 0)
        {
            ttl.colour = 'R';
        }
        else if (strcmp(str, "\"GREEN\"") == 0)
        {
            ttl.colour = 'G';
        }
        else if (strcmp(str, "\"BLUE\"") == 0)
        {
            ttl.colour = 'B';
        }
        else if (strcmp(str, "\"YELLOW\"") == 0)
        {
            ttl.colour = 'Y';
        }
        else if (strcmp(str, "\"CYAN\"") == 0)
        {
            ttl.colour = 'C';
        }
        else if (strcmp(str, "\"MAGENTA\"") == 0)
        {
            ttl.colour = 'M';
        }
        else if (strcmp(str, "\"WHITE\"") == 0)
        {
            ttl.colour = 'W';
        }
        else
        {
            ttl.colour = 'W';
        }
        free(str);
        return true;
    }

    return false;
}

bool is_pfix(Token *t)
{
    if (strcmp(")", t->str) == 0 ||
            (t->next &&
            ((is_op(t->str) && is_pfix(t->next)) ||
            (is_varnum(t->str) && is_pfix(t->next))))
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
            Token *start = t->next->next;
            int token_count = 0;
            while (strcmp(start->next->str, ")") != 0)
            {
                token_count++;
                start = start->next;
            }

            Token *ps_head = t->next->next->next;
            int src_index = get_var_index(ps_head->str[1]);
            int dest_index = get_var_index(t->next->str[0]);
            if (token_count == 1 && is_var(ps_head->str) && ttl.type_in_use[src_index] == union_char)
            {
                int len = strlen(ttl.vars[src_index].word) + NULL_CHAR;
                ttl.vars[dest_index].word = calloc(len, sizeof(char));
                if (!ttl.vars[dest_index].word)
                {
                    panic_msg("allocating space for word");
                }
                strcpy(ttl.vars[dest_index].word, ttl.vars[src_index].word);
                ttl.type_in_use[dest_index] = union_char;
                return true;
            }

            char **postfix_expr = calloc(token_count, sizeof(char*));
            if (!postfix_expr)
            {
                panic_msg("allocating memory for postfix expression\n");
            }
            int i = 0;
            while (strcmp(ps_head->str, ")") != 0)
            {
                int len = strlen(ps_head->str);
                postfix_expr[i] = calloc(len + 1, sizeof(char));
                strcat(postfix_expr[i], ps_head->str);
                ps_head = ps_head->next;
                i++;
            }
            double answer = evaluate(postfix_expr, token_count);
            ttl.vars[dest_index].num = answer;
            ttl.type_in_use[dest_index] = union_double;
            for (int i = 0; i < token_count; i++)
            {
                free(postfix_expr[i]);
            }
            free(postfix_expr);
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
            Token *start_of_lst = t->next->next->next->next;
            int list_len = 0;
            while (strcmp(start_of_lst->str, "}") != 0)
            {
                start_of_lst = start_of_lst->next;
                list_len++;
            }
            if (is_inslst(start_of_lst->next))
            {
                Token *iter = t->next->next->next->next;
                int var_index = get_var_index(t->next->str[0]);
                while (strcmp(iter->str, "}") != 0)
                {
                    update_var(iter->str, var_index);

                    iter = iter->next;
                }

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
    if (is_inslst(next_ins))
    {
        return true;
    }
    // if (is_ins(t) && is_inslst(next_ins))
    // {
    //     return true;
    // }
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

int next_row(int line_start, int step_n)
{
    return line_start - (int)round(step_n * cos(ttl.direction));
}

int next_col(int line_start, int step_n)
{
    return line_start + (int)round(step_n * sin(ttl.direction));
}

double degrees_to_radians(double degrees)
{
    return degrees * (PI / 180);
}

int get_var_index(char var_name)
{
    return var_name - ASCII_TO_NUM;
}

void update_var(char *token_str, int dest_index)
{
    if (ttl.type_in_use[dest_index] == union_char)
        {
            free(ttl.vars[dest_index].word);
        }
    if (is_number(token_str))
    {
        ttl.vars[dest_index].num = strtod(token_str, NULL);
        ttl.type_in_use[dest_index] = union_double;
    }
    else if (is_var(token_str))
    {
        int src_indx = get_var_index(token_str[1]);
        if (ttl.type_in_use[src_indx] == union_char)
        {
            int s_len = strlen(ttl.vars[src_indx].word) + NULL_CHAR;
            ttl.vars[dest_index].word = calloc(s_len, sizeof(char));
            if (!ttl.vars[dest_index].word)
            {
                panic_msg("allocating memory for word");
            }
            strcpy(ttl.vars[dest_index].word, ttl.vars[src_indx].word);
            ttl.type_in_use[dest_index] = union_char;
        }
        else if (ttl.type_in_use[src_indx] == union_double)
        {
            ttl.vars[dest_index].num = ttl.vars[src_indx].num;
            ttl.type_in_use[dest_index] = union_double;
        }
    }
    else if (is_word(token_str))
    {
        ttl.vars[dest_index].word = calloc(strlen(token_str) + NULL_CHAR, sizeof(char));
        if (!ttl.vars[dest_index].word)
        {
            panic_msg("allocating memory for word");
        }
        strcpy(ttl.vars[dest_index].word, token_str);
        ttl.type_in_use[dest_index] = union_char;
    }
}

void init_ttl()
{
    ttl.len = 0;
    ttl.capacity = PATH;
    ttl.direction = 0;
    ttl.colour = 'W';
    ttl.path = calloc(PATH, sizeof(Loc));
    if (!ttl.path)
    {
        panic_msg("allocating memory for turtle's path");
    }
    ttl.vars = calloc(MAX_VARS, sizeof(Var));
    if (!ttl.vars)
    {
        panic_msg("allocating memory for turtle's variables");
    }
    ttl.type_in_use = calloc(MAX_VARS, sizeof(type_used));
    if (!ttl.type_in_use)
    {
        panic_msg("allocating memory for type_used array");
    }
}

void panic_msg(char *msg)
{
    fprintf(stderr, "error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void find_end_points(int x0, int y0, int input_length, int x_y[2])
{
    // x1 should be 0 when drawing a straight line, but it's 1
    float x1 = round(x0 + (input_length * sin(ttl.direction)));
    float y1 = round(y0 - (input_length * cos(ttl.direction)));
    
    // printf("start x %d start y %d end x %d end y %d\n", x0, y0, x1, y1);
    x_y[0] = (int) round(x1);
    x_y[1] = (int) round(y1);
    // calulcate_line_coords((int) round(x0), (int) round(y0), (int) x1, (int) y1, r);
}

void calulcate_line_coords(int x0, int y0, int x1, int y1)
{
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */
    //TODO: while loop
    for (;;)
    { /* loop */
        //TODO: test left shift `err<<2` should have the same effect, but be quicker
        e2 = 2*err;
        if (e2 >= dy) 
        { /* e_xy+e_x > 0 */
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) 
        { /* e_xy+e_y < 0 */
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
        ttl.path[ttl.len].col = x0;
        ttl.path[ttl.len].row = y0;
        ttl.len++;
        // r->s[r->len].col = x0;
        // r->s[r->len].row = y0;
        // r->len++;
    }
}