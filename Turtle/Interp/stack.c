#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define EMPTY -1

typedef struct {
    int data[MAX_STACK_SIZE];
    int top;
} Stack;

Stack *stack_init(void);
bool is_empty(int top);
bool is_full(int top);
bool push(Stack *stack, int value);
int pop(Stack *stack);
int evaluate(char *exp);

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

bool push(Stack *stack, int value) 
{
    if (is_full(stack->top)) 
    {
        return false;
    }
    stack->top++;
    stack->data[stack->top] = value;
    return true;
}

int pop(Stack *stack)
{
    if (is_empty(stack->top))
    {
        exit(EXIT_FAILURE);
    }
    return stack->data[stack->top--];
}

int evaluate(char *exp)
{
    Stack *s = stack_init();
    int i = 0;
    
    while (exp[i])
    {
        if (isdigit(exp[i]))
        {
            bool pushed = push(s, exp[i] - '0');
            if (!pushed)
            {
                fprintf(stderr,"stack overflow\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            int op2 = pop(s);
            int op1 = pop(s);
            switch (exp[i])
            {
            case '+':
                push(s, op1+op2);
                break;
            case '-':
                push(s, op1-op2);
                break;
            case '*':
                push(s, op1*op2);
                break;
            case '/':
                if (op2 == 0)
                {
                    fprintf(stderr, "division by 0 not possible\n");
                    exit(EXIT_FAILURE);
                }
                push(s, op1/op2);
                break;
            default:
            printf("%c\n", exp[i]);
                fprintf(stderr, "invalid character\n");
                exit(EXIT_FAILURE);
            }
        
        }
        i++;
    }
    int answer = pop(s);
    free(s);
    return answer;
}