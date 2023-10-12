#include "ms.h"
#include <ctype.h>

// Maybe some of your own function prototypes here

// board solve_board(board b)
// {
// }

// void board2str(char s[MAXSQ*MAXSQ+1], board b)
// {
// }

bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
    unsigned length = strlen(inp);
    printf("%u %u %u\n\n\n", totmines, width, height);

    if (length != width * height){
        return false;
    }

    for (unsigned i = 0; i < length; i++){
        if (isdigit(inp[i]) == 0 && inp[i] != 'X' && inp[i] != '?'){
            return false;
        }
    }
    return true;
}

// board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
// {
// }

// void test(void)
// {
// }
