#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define INITIAL_SIZE 1

typedef uint8_t boolarr[INITIAL_SIZE];

boolarr *boolarr_init(void);
boolarr *boolarr_initstr(const char* str);
void test(void);


int main(void)
{
    test();
    uint8_t c = 2;
    printf("%d\n",c );
    return 0;
}

boolarr *boolarr_init(void)
{
    int *b = calloc(8,sizeof(int));
    if (b == NULL)
    {
        fprintf(stderr, "failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 8; i++)
    {
        if (b[i] == 0)
        {
            printf("0 ");
        }
    }
    boolarr *c = NULL;
    return c;
}

boolarr* boolarr_initstr(const char* str)
{
    //TODO handle strings with more than 8 chars
    
    boolarr *b = boolarr_init();
    int len = strlen(str);
    for (int i = len; i > 0; i--)
    {
        if (str[i] == '1')
        {
            *b[0] |= (1 << i);
        }
    }
    return b;
}


void test(void)
{
    // boolarr *b = boolarr_init();
    // assert(b[0]);
    // free(b);

    boolarr *s = boolarr_initstr("00000001");
    free(s);
}




