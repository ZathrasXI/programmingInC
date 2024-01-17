#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define EMPTY -1

typedef struct {
    //TODO could make this a better size
    double data[MAX_STACK_SIZE];
    int top;
} Stack;

Stack *stack_init(void);
bool is_empty(int top);
bool is_full(int top);
bool push(Stack *stack, double value);
double pop(Stack *stack);
double evaluate(char **exp, int len);


Stack *stack_init(void)
{
    Stack *s = calloc(1, sizeof(Stack));
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
        exit(EXIT_FAILURE);
    }
    return stack->data[stack->top--];
}

double evaluate(char **exp, int len)
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
                fprintf(stderr,"stack overflow\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (is_var(exp[i]))
        {
            int var_index = get_var_index(exp[i][1]);
            if (ttl.type_in_use[var_index] == union_char)
            {
                panic_msg("can't evaluate postfix expression when variable contains a word");
            }
            else if (ttl.type_in_use[var_index] == union_double)
            {
                bool pushed = push(s, ttl.vars[var_index].num);
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
            else if (strcmp(exp[i], "/") == 0 && fabs(op2 - 0) < TOLERANCE)
            {
                push(s, op1 / op2);
            }
            else
            {
                panic_msg("invalid character in postfix expression");
            }
        }
    }
    double answer = pop(s);
    free(s);
    return answer;
}