#include "ms.h"
#include <ctype.h>

#define QUESTION_MARK 63
#define ASCII_X 88

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

board make_board(int totmines, int width, int height, char inp[MAXSQ*MAXSQ+1])
{   
    board new_board = {
        .w = width,
        .h = height,
        .totmines = totmines,
    };
    
    int board_row = 0;
    for (int i = 0; i < height * width; i+=width)
    {
        for (int j = 0; j < width; j++)
        {
            new_board.grid[board_row][j] = inp[i + j];
        }
        board_row++;
    }

    return new_board;
}

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

    int test_grid[5][5] = {
        {QUESTION_MARK,49,49,48,QUESTION_MARK},
        {49,QUESTION_MARK,50,49,48},
        {49,QUESTION_MARK,ASCII_X,QUESTION_MARK,49},
        {48,49,50,QUESTION_MARK,49},
        {QUESTION_MARK,48,49,49,49}
    };
    board test_board = make_board(5,5,5,"?110?1?2101?X?1012?1?0111");

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            printf("i:%d j:%d\n", i, j);
            printf("board:%d array:%d\n",test_board.grid[i][j],test_grid[i][j]);
            assert(test_board.grid[i][j] == test_grid[i][j]);
        }
    }

}
