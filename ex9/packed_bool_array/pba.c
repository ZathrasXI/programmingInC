#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define INITIAL_SIZE 1

typedef uint8_t boolarr;

boolarr *boolarr_init(void);
// boolarr *boolarr_initstr(const char* str);
void test(void);


int main(void)
{
    test();
    boolarr *b = boolarr_init();
    printf("%d\n", b[0]);
    return 0;
}

boolarr *boolarr_init(void)
{
    boolarr *b = calloc(INITIAL_SIZE,sizeof(boolarr));
    if (b == NULL)
    {
        fprintf(stderr, "failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    return b;
}

boolarr* boolarr_initstr(const char* str)
{
    boolarr *b = boolarr_init();
    int len = strlen(str);
    int bit = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        if (str[i] == '1')
        {
            b[0] |= (1 << bit);
        }
        bit++;
    }
    return b;
}


void test(void)
{
    boolarr *b = boolarr_init();
    assert(b[0] == 0);
    free(b);

    boolarr *s1 = boolarr_initstr("00000001");
    assert(s1[0] == 1);
    free(s1);

    boolarr *s2 = boolarr_initstr("00000010");
    assert(s2[0] == 2);
    free(s2);

    boolarr *s3 = boolarr_initstr("00000011");
    assert(s3[0] == 3);
    free(s3);

    boolarr *s4 = boolarr_initstr("10000000");
    assert(s4[0] == 128);
    free(s4);
}




