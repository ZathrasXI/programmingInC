#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t eight_bits;

eight_bits *init_array(void);

int main(void)
{
    eight_bits *a = init_array();
}

eight_bits *init_array(void)
{
    eight_bits *a = calloc(1, sizeof(uint8_t));
    if (a == NULL)
    {
        fprintf(stderr, "error allocating memory\n");
    }
    return a;
}
