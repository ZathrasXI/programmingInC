#include "interp.h"
#include "stack.c"
#include "test_interp.c"

int main(int argc, char **argv)
{
    test();
    Turtle *ttl = init_ttl();

    if (argc == ONE_ARG)
    {
        printf("Usage: ./interpreter <turtle file>\n");
        exit(EXIT_FAILURE);
    }
    else if (argc == OUTPUT_FILE)
    {
        ttl->ps_mode = ps_mode(argv[2]);
    }
    
    FILE *turtle_file = fopen(argv[1], "r");
    if (!turtle_file)
    {
        panic_msg("opening file");
    }
    Token *head = tokenise(turtle_file);

    if (!is_prog(head, ttl))
    {
        panic_msg("file not parsed");
    }

    if (argc == OUTPUT_FILE)
    {
        if (ttl->ps_mode)
        {
            char *filepath = create_ps_file(ttl->ps_start, argv[2]);
            if (!ps2pdf_cmd(filepath))
            {
                panic_msg("executing psd2pdf command");
            }
        }
        else
        {
            if (!path_to_txt_file(argv[2], ttl))
            {
                panic_msg("creating txt file");
            }
        }
    }
    else if (argc == PRINT_TERMINAL)
    {
        print_to_terminal(ttl);
    }

    free_tokens(head);
    free_ttl(ttl);
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

bool is_forward(Token *t, Turtle *ttl)
{
    char *fwd = "FORWARD";
    if (strcmp(fwd, t->str) == 0 &&
        is_varnum(t->next->str)
    )
    {
        int steps = 0;
        if (is_number(t->next->str))
        {
            sscanf(t->next->str, "%d", &steps);
        }
        else if (is_var(t->next->str))
        {
            steps = ttl->vars[get_var_index(t->next->str[1])].num;
        }
        if (steps > 0)
        {
            if (ttl->ps_mode)
            {  
                update_postscript_ins(ttl, steps);
            }
            else
            {   
                update_txt_ins(ttl,steps);
            }
        }
        return true;
    }
    return false;
}

bool is_rgt(Token *t, Turtle *ttl)
{
    char *rgt = "RIGHT";
    if (strcmp(rgt, t->str) == 0 &&
        is_varnum(t->next->str))
    {
        double degrees;
        if (is_number(t->next->str))
        {
            sscanf(t->next->str, "%lf", &degrees);
        }
        else 
        {
            degrees = ttl->vars[get_var_index(t->next->str[1])].num;
        }
        ttl->direction += degrees_to_radians(degrees);
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

bool is_col(Token *t, Turtle *ttl)
{
    char *col = "COLOUR";
    if (strcmp(col, t->str) == 0 &&
        (
            is_var(t->next->str) ||
            is_word(t->next->str)
        )
    )
    {
        char *str;
        if (is_var(t->next->str))
        {
            int index = get_var_index(t->next->str[1]);
            int len = strlen(ttl->vars[index].word) + 1;
            str = calloc(len, sizeof(char));
            if (!str)
            {
                panic_msg("allocating memory for string (colour)");
            }
            strcpy(str, ttl->vars[index].word);
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
        update_colour(ttl, str);
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

bool is_set(Token *t, Turtle *ttl)
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

            Token *pfix_head = t->next->next->next;
            int src_index = get_var_index(pfix_head->str[1]);
            int dest_index = get_var_index(t->next->str[0]);
            if (token_count == INIT_SIZE && is_var(pfix_head->str) && ttl->type_in_use[src_index] == union_char)
            {
                copy_word_var_to_var(ttl, src_index, dest_index);
                return true;
            }
            evaluate_postfix_expression(ttl, pfix_head, token_count, dest_index);
            return true;
        }
    return false;
}

bool is_loop(Token *t, Turtle *ttl)
{
    if (strcmp(t->str, "LOOP") == 0 &&
        loop_closed(t) &&
        is_letter(t->next->str) &&
        strcmp(t->next->next->str, "OVER") == 0 &&
        is_lst(t->next->next->next)
        )
        {
            Token *start_of_lst = t->next->next->next->next;
            Token *iter = start_of_lst;
            while (strcmp(start_of_lst->str, "}") != 0)
            {
                start_of_lst = start_of_lst->next;
            }
            Token *start_of_ins = start_of_lst->next;
            int var_index = get_var_index(t->next->str[0]);
            while (is_item(iter->str))
            {
                update_var(iter->str, var_index, ttl);
                is_inslst(start_of_ins, ttl);
                iter = iter->next;
            }
            return true;
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

bool is_ins(Token *t, Turtle *ttl)
{
    if (
        is_forward(t, ttl) ||
        is_rgt(t, ttl) ||
        is_col(t, ttl) ||
        is_loop(t, ttl) ||
        is_set(t, ttl)
    )
    {
        return true;
    }
    return false;
}

bool is_inslst(Token *t, Turtle *ttl)
{
    if (!t)
    {
        return false;
    }
    Token *next_ins = t;
    if (strcmp("END", t->str) == 0)
    {
        return true;
    }
    
    if (is_forward(t, ttl))
    {
        return is_inslst(t->next->next, ttl);
    }
    else if (is_rgt(t, ttl))
    {
        return is_inslst(t->next->next, ttl);
    }
    else if (is_col(t, ttl))
    {
        return is_inslst(t->next->next, ttl);
    }
    else if (is_loop(t, ttl))
    {
        do
        {
            next_ins = next_ins->next;
        } 
        while (strcmp(next_ins->str, "END") != 0);
        next_ins = next_ins->next;
        return is_inslst(next_ins, ttl);
    }
    else if (is_set(t, ttl))
    {
        do
        {
            next_ins = next_ins->next;
        } 
        while (strcmp(next_ins->str, ")") != 0);
        next_ins = next_ins->next;
        return is_inslst(next_ins, ttl);
    }
    return false;
}

bool is_prog(Token *t, Turtle *ttl)   
{
    if (strcmp(t->str, "START") == 0 && 
        t->next &&
        is_inslst(t->next, ttl))
    {
        return true;
    }
    return false;
}

double degrees_to_radians(double degrees)
{
    return degrees * (PI / 180);
}

int get_var_index(char var_name)
{
    return var_name - ASCII_TO_NUM;
}

void update_var(char *token_str, int dest_index, Turtle *ttl)
{
    if (ttl->type_in_use[dest_index] == union_char)
        {
            free(ttl->vars[dest_index].word);
        }
    if (is_number(token_str))
    {
        ttl->vars[dest_index].num = strtod(token_str, NULL);
        ttl->type_in_use[dest_index] = union_double;
    }
    else if (is_var(token_str))
    {
        int src_indx = get_var_index(token_str[1]);
        if (ttl->type_in_use[src_indx] == union_char)
        {
            int s_len = strlen(ttl->vars[src_indx].word) + NULL_CHAR;
            ttl->vars[dest_index].word = calloc(s_len, sizeof(char));
            if (!ttl->vars[dest_index].word)
            {
                panic_msg("allocating memory for word");
            }
            strcpy(ttl->vars[dest_index].word, ttl->vars[src_indx].word);
            ttl->type_in_use[dest_index] = union_char;
        }
        else if (ttl->type_in_use[src_indx] == union_double)
        {
            ttl->vars[dest_index].num = ttl->vars[src_indx].num;
            ttl->type_in_use[dest_index] = union_double;
        }
    }
    else if (is_word(token_str))
    {
        ttl->vars[dest_index].word = calloc(strlen(token_str) + NULL_CHAR, sizeof(char));
        if (!ttl->vars[dest_index].word)
        {
            panic_msg("allocating memory for word");
        }
        strcpy(ttl->vars[dest_index].word, token_str);
        ttl->type_in_use[dest_index] = union_char;
    }
}

Turtle *init_ttl(void)
{
    Turtle *t = calloc(INIT_SIZE, sizeof(Turtle));
    if (!t)
    {
        panic_msg("allocating memory for Turtle");
    }
    t->direction = 0;
    t->colour = 'W';
    t->ps_start = NULL;
    t->ps_last = NULL;
    t->ps_mode = false;
    return t;
}

void panic_msg(char *msg)
{
    fprintf(stderr, "error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void find_end_points(float x0, float y0, int input_length, float x1_y1[X_Y], Turtle *ttl)
{
    if (!ttl->ps_mode)
    {
        x1_y1[0] = x0 + (input_length * sin(ttl->direction));
        x1_y1[1] = y0 - (input_length * cos(ttl->direction));
    }
    else
    {
        x1_y1[0] = x0 + (input_length * sin(ttl->direction));
        x1_y1[1] = y0 + (input_length * cos(ttl->direction));
    }
}

int get_ansi_colour(char c)
{
    switch (c)
    {
    case 'K':
        return BLACK;
    case 'R':
        return RED;
    case 'G':
        return GREEN;
    case 'Y':
        return YELLOW;
    case 'B':
        return BLUE;
    case 'M':
        return MAGENTA;
    case 'C':
        return CYAN;
    case 'W':
        return WHITE;
    default:
        return WHITE;
    }
}

void print_to_terminal(Turtle *ttl)
{
    printf("\033[2J");
    Loc *head = ttl->path_start;
    while (head)
    {
        if (coords_within_ansi_boundaries(head))
        {
            if (head->fwd_ins)
            {
                printf("\033[%d;%dH", head->row + ANSI_ROW_START, head->col + ANSI_COL_START);
                printf("\033[%dm%c\033[%dm", get_ansi_colour(head->colour), head->colour, RESET); 
                head = head->next;
            }
            while (head && !head->fwd_ins)
            {
                if (coords_within_ansi_boundaries(head))
                {
                    printf("\033[%d;%dH", head->row + ANSI_ROW_START, head->col + ANSI_COL_START);
                    printf("\033[%dm%c\033[%dm", get_ansi_colour(head->colour), head->colour, RESET); 
                }
                head = head->next;
            }
            fflush(stdout);
            sleep(1);
        }
        else
        {
            head = head->next;
        }
    }
    printf("\033[%d;1H", HEIGHT + PADDING);
}

bool ps_mode(char *filename)
{
    regex_t regex;
    char *pattern = "\\w+\\.ps$";

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
    {
        panic_msg("failed to compile regex pattern");
    }

    if (regexec(&regex, filename, 0, NULL, 0) == 0)
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

Line *new_line(float x0, float y0, float x1, float y1)
{
    Line *new = (Line *) calloc(INIT_SIZE, sizeof(Line));
    if (!new)
    {
        panic_msg("allocating memory for Line");
    }
    new->next = NULL;
    new->x0 = x0;
    new->y0 = y0;
    new->x1 = x1;
    new->y1 = y1;
    return new;
}

char *set_postscript_colour(char c)
{
    char *red = "1 0 0";
    char *green = "0 1 0";
    char *blue = "0 0 1";
    char *yellow = "1 1 0";
    char *magenta = "1 0 1";
    char *cyan = "0 1 1";
    char *black = "0 0 0";
    char *white_grey = "0.8 0.8 0.8";

    char *colour;
    switch (c)
    {
    case 'R':
        colour = calloc(strlen(red) + NULL_CHAR, sizeof(char));
        strcpy(colour, red);
        break;
    case 'G':
        colour = calloc(strlen(green) + NULL_CHAR, sizeof(char));
        strcpy(colour, green);
        break;
    case 'B':
        colour = calloc(strlen(blue) + NULL_CHAR, sizeof(char));
        strcpy(colour, blue);
        break;
    case 'Y':
        colour = calloc(strlen(yellow) + NULL_CHAR, sizeof(char));
        strcpy(colour, yellow);
        break;
    case 'M':
        colour = calloc(strlen(magenta) + NULL_CHAR, sizeof(char));
        strcpy(colour, magenta);
        break;
    case 'C':
        colour = calloc(strlen(cyan) + NULL_CHAR, sizeof(char));
        strcpy(colour, cyan);
        break;
    case 'K':
        colour = calloc(strlen(black) + NULL_CHAR, sizeof(char));
        strcpy(colour, black);
        break;
    default:
        colour = calloc(strlen(white_grey) + NULL_CHAR, sizeof(char));
        strcpy(colour, white_grey);
        break;
    }
    return colour;
}

char *create_ps_file(Line *l, char *filename)
{
    char *file_path = create_file_path(filename);
    FILE *f = fopen(file_path, "w");
    if (!f)
    {
        panic_msg("creating .ps file");
    }

    fprintf(f, "0.2 setlinewidth\n10 10 scale\n");

    if (l)
    {
        Line *current = l;
        while (current)
        {   
            fprintf(f, "newpath\n");
            fprintf(f,"%f %f moveto\n", current->x0,current->y0);
            fprintf(f,"%f %f lineto\n", current->x1, current->y1);
            fprintf(f, "%s setrgbcolor\n", current->colour);
            fprintf(f,"stroke\n");
            current = current->next;
        }
    }
    fprintf(f, "showpage\n");
    fclose(f);
    return file_path;
}

char *create_file_path(char *filename)
{
    const char *dir = "./Results/";
    int len = strlen(dir) + strlen(filename) + NULL_CHAR;
    char *path = calloc(len, sizeof(char));
    if (!path)
    {
        panic_msg("creating string for filename");
    }
    // Does dir exist?
    if (access(dir, F_OK) == -1) 
    {
        //If not, create it with appropriate permissions
        if (mkdir(dir, 0777) != 0) 
        {
            panic_msg("folder not created");
        } 
    } 
    strcpy(path, dir);
    strcat(path, filename);

    return path;
}

bool ps2pdf_cmd(char *filepath)
{
    char *pdf = calloc(strlen(filepath) + strlen("f") + NULL_CHAR, sizeof(char));
    if (!pdf)
    {
        panic_msg("allocating space for filename of PDF");
    }
    strcpy(pdf, filepath);
    pdf[strlen(pdf)-1] = 'd';
    pdf[strlen(pdf)] = 'f';
    int len = strlen("ps2pdf ") + strlen(filepath) + strlen(" ") + strlen(pdf) + NULL_CHAR; 
    char *cmd = calloc(len, sizeof(char));
    if (!cmd)
    {
        panic_msg("allocating space for ps2pdf command");
    }
    sprintf(cmd, "ps2pdf %s %s", filepath, pdf);
    int return_code = system(cmd);
    free(filepath);
    free(cmd);
    free(pdf);
    if (return_code == 0)
    {
        return true;
    }
    return false;
}

void update_postscript_ins(Turtle *ttl, int steps)
{
    float x1_y1[X_Y];
    if (!ttl->ps_start)
    {
        find_end_points(PS_START_X, PS_START_Y, steps, x1_y1, ttl);
        ttl->ps_start = new_line(PS_START_X, PS_START_Y, x1_y1[0], x1_y1[1]);
        ttl->ps_start->colour = set_postscript_colour(ttl->colour);
        ttl->ps_last = ttl->ps_start;
    }
    else 
    {
        find_end_points(ttl->ps_last->x1, ttl->ps_last->y1, steps, x1_y1, ttl);
        ttl->ps_last->next = new_line(ttl->ps_last->x1, ttl->ps_last->y1, x1_y1[0], x1_y1[1]);
        ttl->ps_last->next->colour = set_postscript_colour(ttl->colour);
        ttl->ps_last = ttl->ps_last->next;
    }
}

void update_txt_ins(Turtle *ttl, int steps)
{
    if (!ttl->path_start)
    {
        ttl->path_start = new_loc(COL_START, ROW_START, ttl->colour, true);
        ttl->path_end = ttl->path_start;
        ttl->path_len++;
    }
    float px1_py1[X_Y];
    find_end_points(ttl->path_end->col, ttl->path_end->row, steps, px1_py1, ttl);
    calculate_loc_coords(ttl->path_end->col, ttl->path_end->row, round(px1_py1[0]), round(px1_py1[1]), ttl);
}

void copy_word_var_to_var(Turtle *ttl, int src_index, int dest_index)
{
    int len = strlen(ttl->vars[src_index].word) + NULL_CHAR;
    ttl->vars[dest_index].word = calloc(len, sizeof(char));
    if (!ttl->vars[dest_index].word)
    {
        panic_msg("allocating space for word");
    }
    strcpy(ttl->vars[dest_index].word, ttl->vars[src_index].word);
    ttl->type_in_use[dest_index] = union_char;
}

void evaluate_postfix_expression(Turtle *ttl, Token *pfix_head, int token_count, int dest_index)
{
    char **postfix_expr = calloc(token_count, sizeof(char*));
    if (!postfix_expr)
    {
        panic_msg("allocating memory for postfix expression\n");
    }
    int i = 0;
    while (strcmp(pfix_head->str, ")") != 0)
    {
        int len = strlen(pfix_head->str);
        postfix_expr[i] = calloc(len + NULL_CHAR, sizeof(char));
        if (!postfix_expr[i])
        {
            panic_msg("allocating space for postfix expression");
        }
        strcat(postfix_expr[i], pfix_head->str);
        pfix_head = pfix_head->next;
        i++;
    }
    double answer = evaluate(postfix_expr, token_count, ttl);
    ttl->vars[dest_index].num = answer;
    ttl->type_in_use[dest_index] = union_double;
    for (int i = 0; i < token_count; i++)
    {
        free(postfix_expr[i]);
    }
    free(postfix_expr);
}

void update_colour(Turtle *ttl, char *colour)
{
        /*
        % Valid colours include "BLACK", "RED", "GREEN", "BLUE",
        % "YELLOW", "CYAN", "MAGENTA", "WHITE"
        */
        if (strcmp(colour, "\"BLACK\"") == 0)
        {
            if (ttl->path_start)
            {
                ttl->path_end->colour = 'K';
            }
            ttl->colour = 'K';
        }
        else if (strcmp(colour, "\"RED\"") == 0)
        {
            if (ttl->path_start)
            {
                ttl->path_end->colour = 'R';
            }
            ttl->colour = 'R';
        }
        else if (strcmp(colour, "\"GREEN\"") == 0)
        {
            if (ttl->path_start)
            {
                ttl->path_end->colour = 'G';
            }
            ttl->colour = 'G';
        }
        else if (strcmp(colour, "\"BLUE\"") == 0)
        {
            if (ttl->path_start)
            {
                ttl->path_end->colour = 'B';
            }
            ttl->colour = 'B';
        }
        else if (strcmp(colour, "\"YELLOW\"") == 0)
        {
            if (ttl->path_start)
            {
                ttl->path_end->colour = 'Y';
            }
            ttl->colour = 'Y';
        }
        else if (strcmp(colour, "\"CYAN\"") == 0)
        {
            if (ttl->path_start)
            {
                ttl->path_end->colour = 'C';
            }
            ttl->colour = 'C';
        }
        else if (strcmp(colour, "\"MAGENTA\"") == 0)
        {
            if (ttl->path_start)
            {
                ttl->path_end->colour = 'M';
            }
            ttl->colour = 'M';
        }
        else if (strcmp(colour, "\"WHITE\"") == 0)
        {
            if (ttl->path_start)
            {
                ttl->path_end->colour = 'W';
            }
            ttl->colour = 'W';
        }
        else
        {
            if (ttl->path_start)
            {
                ttl->path_end->colour = 'W';
            }
            ttl->colour = 'W';
        }
        free(colour);
}

Loc *new_loc(int col, int row, char c, bool fwd_ins)
{
    Loc *new = calloc(INIT_SIZE, sizeof(Loc));
    if (!new)
    {
        panic_msg("allocating space for new Loc type");
    }

    new->row = row;
    new->col = col;
    new->colour = c;
    new->fwd_ins = fwd_ins;
    new->next = NULL;

    return new;
}

void calculate_loc_coords(int x0, int y0, int x1, int y1, Turtle *ttl)
{
    int dx = abs(x1-x0); 
    int dy = -abs(y1-y0); 
    int err = dx+dy, e2; 
    int sx, sy;
    if (x0 < x1)
    {
        sx = 1;
    }
    else
    {
        sx = -1;
    }
    if (y0 < y1)
    {
        sy = 1;
    }
    else
    {
        sy = -1;
    }
    bool end_x = false;
    bool end_y = false;
    bool end_of_line = false;
    int locs_added = 0;
    while(!end_of_line)
    { 
        e2 = err * 2;
        if (e2 >= dy) 
        { 
            if (!end_y && x0 == x1)
            {
                end_x = true;
            }
            else
            {
                err += dy; x0 += sx;
            }
        }
        if (!end_x && e2 <= dx) 
        { 
            if (y0 == y1)
            {
                end_y = true;
            }
            else
            {
                err += dx; y0 += sy;
            }
        }
        end_of_line = end_x || end_y;
        if (!end_of_line)
        {
            if (x0 < WIDTH && y0 < HEIGHT)
            {
                ttl->path_end->next = new_loc(x0,y0,ttl->colour, false);
                ttl->path_end = ttl->path_end->next;
                if (locs_added == 0 && ttl->path_len > 1)
                {
                    ttl->path_end->fwd_ins = true;
                }
                ttl->path_len++;
                locs_added++;
            }
        }
    } 
}

bool coords_within_ansi_boundaries(Loc *head)
{
    return head->row >= 0 && head->row < HEIGHT && head->col >= 0 && head->col < WIDTH;
}



bool path_to_txt_file(char *name, Turtle *ttl)
{
    char *filepath = create_file_path(name);
    FILE *f = fopen(filepath, "w");
    if (!f)
    {
        free(filepath);
        return false;
    }
    char arr[HEIGHT][WIDTH];
    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            arr[row][col] = ' ';
        }
    }
    Loc *head = ttl->path_start;
    while (head)
    {
        if (head->row >= 0 &&
            head->row < HEIGHT &&
            head->col >= 0 &&
            head->col < WIDTH)
            {
                arr[head->row][head->col] = head->colour;
            }
        head = head->next;
    }

    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            fprintf(f, "%c", arr[row][col]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    free(filepath);
    return true;
}
