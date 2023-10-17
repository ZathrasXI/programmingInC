#include "ms.h"
#include <ctype.h>

#define QUESTION_MARK 63
#define ASCII_X 88
#define ASCII_CONVERSION 48
#define MAX_DIGIT 8 

bool mines_of_adjacent_cells(int row, int column, int grid[MAXSQ][MAXSQ]);
int adjacent_mines(int row, int column, int grid[MAXSQ][MAXSQ]);
bool is_valid_number(char c);

board solve_board(board b)
{
    int mines = 0;
    for (int i = 0; i < b.h; i++)
    {
        for (int j = 0; j < b.w; j++)
        {
            if (b.grid[i][j] == QUESTION_MARK)
            {
                // bool mines_need = mines_of_adjacent_cells(i,j,b.grid);
                mines = adjacent_mines(i,j,b.grid);
                b.grid[i][j] = mines;
            }
        }
    }
    return b;
}

void board2str(char s[MAXSQ*MAXSQ+1], board b)
{
    int row_in_string = 0;
    for(int i = 0; i < b.h; i++)
    {
        for(int j = 0; j < b.w; j++)
        {
            if(b.grid[i][j] >= 0 && b.grid[i][j] <= 9)
            {
                s[row_in_string + j] = b.grid[i][j] + ASCII_CONVERSION;    
            } 
            else
            {
                s[row_in_string + j] = b.grid[i][j];    
            }
        }
        row_in_string += b.w;
    }
}

bool syntax_check(unsigned totmines, unsigned width, unsigned height, char inp[MAXSQ*MAXSQ+1])
{
    unsigned length = strlen(inp);
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
            if(isdigit(inp[i + j]))
            {
                int converted_successfully = sscanf((&inp[i + j]), "%1d", &new_board.grid[board_row][j]);
                if(!converted_successfully)
                {
                    printf("error converting %c to type int\n", inp[i+j]);
                }
            } 
            else
            {
                new_board.grid[board_row][j] = inp[i + j];
            }
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

    if(num < 0 || num > 8)
    {
        return false;
    }
    return true;
}

bool mines_of_adjacent_cells(int row, int column, int grid[MAXSQ][MAXSQ])
{
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            if (i >= 0 && i < MAXSQ && j >= 0 && j < MAXSQ && grid[i][j] < MAX_DIGIT)
            {
                printf(".");
            }
        }
    }
    return false;
}

int adjacent_mines(int row, int column, int grid[MAXSQ][MAXSQ])
{
    int mines = 0;
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            if (i >= 0 && i < MAXSQ && j >= 0 && j < MAXSQ)
            {
                if (grid[i][j] == ASCII_X && &grid[i][j] != &grid[row][column]) 
                {
                    mines++;
                }
            }
        }
    }
    return mines;
}

void test(void)
{
    assert(is_valid_number('0') == true);
    assert(is_valid_number('8') == true);
    assert(is_valid_number('9') == false);
    assert(is_valid_number('X') == false);
    assert(is_valid_number('?') == false);

    //assets for tests
    int test_grid[MAXSQ][MAXSQ] = {
        {QUESTION_MARK,1,1,0,QUESTION_MARK},
        {1,QUESTION_MARK,2,1,0},
        {1,QUESTION_MARK,ASCII_X,QUESTION_MARK,1},
        {0,1,2,QUESTION_MARK,1},
        {QUESTION_MARK,0,1,1,1}
    };
    int test_adjacent_mines[] = {0,0,0,0,0,  0,1,1,1,0,  0,1,0,1,0,  0,1,1,1,0, 0,0,0,0,0};

    // make_board()
    board test_board = make_board(5,5,5,"?110?1?2101?X?1012?1?0111");

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            assert(test_board.grid[i][j] == test_grid[i][j]);
        }
    }

    // adjacent_mines()
    int mines = adjacent_mines(0,0,test_grid);
    int index = 0;
    assert(mines == 0);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            assert(adjacent_mines(i,j,test_grid) == test_adjacent_mines[index]);
            index++;
        }
    }
}
