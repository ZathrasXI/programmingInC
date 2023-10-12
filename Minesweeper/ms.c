#include "ms.h"

// Maybe some of your own function prototypes here

// board solve_board(board b)
// {
// }

// void board2str(char s[MAXSQ*MAXSQ+1], board b)
// {
// }

bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
    printf("%u %u %u \n", totmines, width, height);
    int length = MAXSQ*MAXSQ+1;
    for (int i = 0; i < length; i++) {
        printf("%s", inp);
    }
    printf("\n");
    return true;
}

// board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
// {
// }

// void test(void)
// {
// }
