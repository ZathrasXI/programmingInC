#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define PATH 10
#define MAX_VARS 10


typedef struct turtle 
{
    int x0;
    int y0;
    int x1;
    int y1;
} Turtle;

void *init_ttl()
{
    Turtle *t = calloc(1, sizeof(Turtle));
    t->x0 = 1;
    t->x1 = 10;
    t->y0 = 2;
    t->y1 = 20;
    return (void *) t;
}

int main(void)
{
    int t = 5;
    Turtle **ttl = calloc(t, sizeof(Turtle));
    pthread_t *th = calloc(t, sizeof(pthread_t));
    for (int i = 0; i < t; i ++)
    {
        if (pthread_create(th + i, NULL, &init_ttl, NULL) != 0)
        {
            printf("error\n");
        }
    }
    for (int i = 0; i < t; i++)
    {
        if (pthread_join(th[i], (void **) &ttl[i]) != 0)
        {
            printf("join error\n");
        }
    }
    
    for (int i = 0; i < t; i++)
    {
        printf("%d %d %d %d %d\n", i, ttl[i]->x0, ttl[i]->y0, ttl[i]->x1, ttl[i]->y1);
    }


}