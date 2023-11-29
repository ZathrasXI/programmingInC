#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#define INITIAL_SIZE 1
#define BYTE 8

typedef struct boolarr {
    int size;
    int capacity;
    uint8_t *arr;
} boolarr;


boolarr *boolarr_init(void);
boolarr *boolarr_initstr(const char* str);
boolarr* boolarr_clone(const boolarr* ba);
unsigned int boolarr_size(const boolarr* ba);
void test(void);


int main(void)
{
    // test();
    boolarr *b = boolarr_initstr("0000000900000001");
    printf("\n%d\n", b->capacity);
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
    b->arr = (uint8_t*) calloc(1, sizeof(uint8_t));
    if (b->arr == NULL)
    {
        fprintf(stderr, "failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    b->size = 1;
    b->capacity = 1;
    return b;
}

boolarr* boolarr_initstr(const char* str)
{
    //for every 8 chars, calloc extra unint_8 to arr
    int len = strlen(str);
    int bytes = len / BYTE;
    if (len % BYTE != 0)
    {
        fprintf(stderr, "str needs to have length multiple of 8\n");
        exit(EXIT_FAILURE);
    }
    printf("bytes %d\n", bytes);

    boolarr *b = (boolarr *) calloc(1, sizeof(boolarr));
    b->arr = (uint8_t *) calloc(bytes, sizeof(uint8_t));
    b->capacity = bytes;
    b->size = bytes;
    int bit = BYTE - 1;
    int byte_n = 0;
    
    while (byte_n != bytes)
    {
        printf("%c", str[bit]);
        bit--;
        if (bit == (byte_n * BYTE - 1) - BYTE) 
        {
            byte_n++;
            bit = bytes * BYTE - 1;
        }
    }
    

    // for (int i = len - 1; i >= 0; i--)
    // {
    //     if (str[i] == '1')
    //     {
    //         b->arr[0] |= (1 << bit);
    //     }
    //     bit++;
    // }
    return b;
}

boolarr* boolarr_clone(const boolarr* ba)
{
    boolarr *deep_copy = boolarr_init();
    deep_copy->capacity = ba->capacity;
    deep_copy->size = ba->size;
    memcpy(deep_copy->arr, ba->arr, sizeof(uint8_t) * ba->size);
    return deep_copy;
}

unsigned int boolarr_size(const boolarr* ba)
{
    return (unsigned int) ba->size;
}



void test(void)
{
    boolarr *b = boolarr_init();
    assert(b->arr[0] == 0);
    assert(b->capacity == 1);
    assert(b->size == 1);

    // can init boolean packed array from string
    boolarr *s1 = boolarr_initstr("00000001");
    assert(s1->arr[0] == 1);

    boolarr *s2 = boolarr_initstr("00000010");
    assert(s2->arr[0] == 2);

    boolarr *s3 = boolarr_initstr("00000011");
    assert(s3->arr[0] == 3);

    boolarr *s129 = boolarr_initstr("10000001");
    assert(s129->arr[0] == 129);

    boolarr *bytes_2 = boolarr_initstr("0000000000000001");
    printf("%d\n",bytes_2->arr[0]);

    // can deep copy
    boolarr *ba = boolarr_initstr("00000001");
    boolarr *deep_copy = boolarr_clone(ba);
    assert(deep_copy->size == ba->size);
    assert(deep_copy->capacity == ba->capacity);
    assert(memcmp(deep_copy->arr, ba->arr, sizeof(uint8_t) * ba->size) == 0);

    boolarr *ba1 = boolarr_initstr("11111111");
    boolarr *deep_copy1 = boolarr_clone(ba1);
    assert(deep_copy1->size == ba1->size);
    assert(deep_copy1->capacity == ba1->capacity);
    assert(memcmp(deep_copy1->arr, ba1->arr, sizeof(uint8_t) * ba1->size) == 0);


}




