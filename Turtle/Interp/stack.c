#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define EMPTY -1

typedef struct {
    double data[MAX_STACK_SIZE];
    int top;
} Stack;

Stack *stack_init(void);
bool is_empty(int top);
bool is_full(int top);
bool push(Stack *stack, double value);
double pop(Stack *stack);
double evaluate(char **exp, int len);

// int main(void)
// {
//     Stack *s = stack_init();
//     char postfix[] = "68*9+";
//     int result = evaluate(postfix);
//     printf("result %d\n", result);
//     return 0;
// }

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
            //get index
            //TODO check if is num or char type in use
            int index = get_var_index(exp[i][1]);
            bool pushed = push(s, ttl.vars[index].num);
            if (!pushed)
            {
                panic_msg("pushing value to stack, possible stack overflow");
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
            else if (strcmp(exp[i], "/") == 0 && fabs(op2 - 0) < 0.0001)
            {
                //TODO replace 0 with #define NO_DIVISION_BY_ZERO
                push(s, op1 / op2);
            }
            else
            {
                panic_msg("invalid character in postfix expression");
            }




            // switch (exp[i])
            // {
            // case '+':
            //     push(s, op1+op2);
            //     break;
            // case '-':
            //     push(s, op1-op2);
            //     break;
            // case '*':
            //     push(s, op1*op2);
            //     break;
            // case '/':
            //     if (op2 == 0)
            //     {
            //         fprintf(stderr, "division by 0 not possible\n");
            //         exit(EXIT_FAILURE);
            //     }
            //     push(s, op1/op2);
            //     break;
            // default:
            // printf("%c\n", exp[i]);
            //     fprintf(stderr, "invalid character\n");
            //     exit(EXIT_FAILURE);
            // }
        
        }
    }
    int answer = pop(s);
    free(s);
    return answer;
}