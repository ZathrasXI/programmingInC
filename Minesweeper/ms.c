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
    printf("%u %u %u\n\n\n", totmines, width, height);

    if (strlen(inp) != width * height){
        return false;
    }
    // Ensure only characters are from the set:   0123456789?X
    
    printf("%s\n", inp);
    return true;
}

// board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
// {
// }

// void test(void)
// {
// }
