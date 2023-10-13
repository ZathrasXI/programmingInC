#include "ms.h"
#include <ctype.h>

// Maybe some of your own function prototypes here

bool is_valid_number(char c);

// board solve_board(board b)
// {
// }

// void board2str(char s[MAXSQ*MAXSQ+1], board b)
// {
// }


bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
    unsigned length = strlen(inp);
    printf("%u %u\n\n\n", width, height);

    if (length != width * height)
    {
        return false;
    }

    unsigned mine_counter = 0;
    for (unsigned i = 0; i < length; i++)
    {
        if (!is_valid_number(inp[i]) && inp[i] != 'X' && inp[i] != '?'){
            return false;
        }
        if (inp[i] == 'X'){
            mine_counter++;
        }
    }

    if (mine_counter > totmines)
    {
        return false;
    }

    return true;
}

// board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
// {
// }

bool is_valid_number(char c)
{
    if(!isdigit(c))
    {
        return false;
    }

    int num;
    int convert_successfully = sscanf(&c, "%d", &num);
    if(!convert_successfully)
    {  
        return false;
    }

    if(num < 0 || num > 9)
    {
        return false;
    }
    return true;
}



void test(void)
{
    assert(is_valid_number('0') == true);
    assert(is_valid_number('9') == true);
    assert(is_valid_number('X') == false);
    assert(is_valid_number('?') == false);

}
