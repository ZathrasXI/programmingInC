
Stack *stack_init(void)
{
    Stack *s = calloc(1, sizeof(Stack));
    if (!s)
    {
        panic_msg("allocating space for stack");
    }
    s->top = EMPTY;
    return s;
}

bool is_empty(int top)
{
    return top == EMPTY;
}

bool is_full(int top) 
{
    return top == MAX_STACK_SIZE - 1;
}

bool push(Stack *stack, double value) 
{
    if (is_full(stack->top)) 
    {
        return false;
    }
    stack->top++;
    stack->data[stack->top] = value;
    return true;
}

double pop(Stack *stack)
{
    if (is_empty(stack->top))
    {
        panic_msg("can't pop when stack is empty");
    }
    return stack->data[stack->top--];
}

double evaluate(char **exp, int len, Turtle *ttl)
{
    Stack *s = stack_init();
    
    for (int i = 0; i < len; i++)
    {
        if (is_number(exp[i]))
        {
            double num; 
            sscanf(exp[i], "%lf", &num);
            bool pushed = push(s, num);
            if (!pushed)
            {
                panic_msg("stack overflow\n");
            }
        }
        else if (is_var(exp[i]))
        {
            int var_index = get_var_index(exp[i][1]);
            if (ttl->type_in_use[var_index] == union_char)
            {
                panic_msg("can't evaluate postfix expression when variable contains a word");
            }
            else if (ttl->type_in_use[var_index] == union_double)
            {
                bool pushed = push(s, ttl->vars[var_index].num);
                if (!pushed)
                {
                    panic_msg("pushing value to stack, possible stack overflow");
                }
            }
            else
            {
                panic_msg("type in use not defined for variable");
            }
        }
        else
        {
            double op2 = pop(s);
            double op1 = pop(s);
            if (strcmp(exp[i], "+") == 0)
            {
                push(s, op1 + op2);
            }
            else if (strcmp(exp[i], "-") == 0)
            {
                push(s, op1 - op2);
            }
            else if (strcmp(exp[i], "*") == 0)
            {
                push(s, op1 * op2);
            }
            else if (strcmp(exp[i], "/") == 0  && fabs(op2 - 0) > TOLERANCE)
            {
                push(s, op1 / op2);
            }
            else
            {
                printf("else %s %lf\n\n", exp[i], op2);
                panic_msg("invalid character in postfix expression");
            }
        }
    }
    double answer = pop(s);
    free(s);
    return answer;
}
