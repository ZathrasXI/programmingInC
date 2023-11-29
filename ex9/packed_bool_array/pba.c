#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define INITIAL_SIZE 1

typedef struct boolarr {
    int size;
    int capacity;
    uint8_t *arr;
} boolarr;


boolarr *boolarr_init(void);
boolarr *boolarr_initstr(const char* str);
boolarr* boolarr_clone(const boolarr* ba);
void test(void);


int main(void)
{
    test();
    // boolarr *b = boolarr_init();
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
    boolarr *b = boolarr_init();
    int len = strlen(str);
    int bit = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        if (str[i] == '1')
        {
            b->arr[0] |= (1 << bit);
        }
        bit++;
    }
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


void test(void)
{
    boolarr *b = boolarr_init();
    assert(b->arr[0] == 0);
    assert(b->capacity == 1);
    assert(b->size == 1);

    boolarr *s1 = boolarr_initstr("00000001");
    assert(s1->arr[0] == 1);

    boolarr *s2 = boolarr_initstr("00000010");
    assert(s2->arr[0] == 2);

    boolarr *s3 = boolarr_initstr("00000011");
    assert(s3->arr[0] == 3);

    boolarr *s129 = boolarr_initstr("10000001");
    assert(s129->arr[0] == 129);

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




