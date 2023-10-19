#include "ms.h"
#include <ctype.h>
#include <stdlib.h>

#define ASCII_CONVERSION 48
#define MAX_DIGIT 8
#define MAX_ATTEMPTS 5

int adjacent_mines(int row, int column, int grid[MAXSQ][MAXSQ]);
bool is_valid_number(char c);
bool indexes_within_boundary(int row, int column);
int unknowns_in_neighbourhood(int row, int column, int grid[MAXSQ][MAXSQ]);
int unknowns_to_mines(int row, int column, int grid[MAXSQ][MAXSQ], int amount);

board solve_board(board b)
{
    int mines_found = 0, attempts = 0;
    while (mines_found <= b.totmines && attempts < MAX_ATTEMPTS)
    {
        for (int i = 0; i < b.h; i++)
        {
            for (int j = 0; j < b.w; j++)
            {
                if (b.grid[i][j] == MINE)
                {
                    mines_found++;
                }
                if (b.grid[i][j] <= MAX_DIGIT)
                {
                    if (b.grid[i][j] - unknowns_in_neighbourhood(i,j,b.grid) == adjacent_mines(i,j,b.grid))
                    {
                        mines_found += unknowns_to_mines(i, j, b.grid, unknowns_in_neighbourhood(i,j,b.grid));
                    }
                }
            }
        }
        attempts++;
    }

    for (int i = 0; i < b.h; i++)
    {
        for (int j = 0; j < b.w; j++)
        {
            if (b.grid[i][j] == UNK)
            {
                b.grid[i][j] = adjacent_mines(i,j,b.grid);
            }
        }
    }
    return b;
}

void board2str(char s[MAXSQ*MAXSQ+1], board b)
{
    int row_in_string = 0;
    for(int j = 0; j < b.h; j++)
    {
        for(int i = 0; i < b.w; i++)
        {
            if(b.grid[j][i] >= 0 && b.grid[j][i] <= MAX_DIGIT)
            {
                s[row_in_string + i] = b.grid[j][i] + ASCII_CONVERSION;    
            } 
            else
            {
                s[row_in_string + i] = b.grid[j][i];    
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
        if (!is_valid_number(inp[i]) && inp[i] != MINE && inp[i] != UNK){
            return false;
        }
        if (inp[i] == MINE){
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
    for (int i = 0; i < height * width; i += width)
    {
        for (int j = 0; j < width; j++)
        {
            if(isdigit(inp[i + j]))
            {
                if(!sscanf((&inp[i + j]), "%1d", &new_board.grid[board_row][j]))
                {
                    fprintf(stderr,"error converting %c to type int\n", inp[i+j]);
                    exit(EXIT_FAILURE);
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

    int num = c - ASCII_CONVERSION;
    if(num < 0 || num > MAX_DIGIT)
    {
        return false;
    }
    return true;
}

int adjacent_mines(int row, int column, int grid[MAXSQ][MAXSQ])
{
    int mines = 0;
    for (int r = row - 1; r <= row + 1; r++)
    {
        for (int c = column - 1; c <= column + 1; c++)
        {
            if (indexes_within_boundary(r,c))
            {
                if (grid[r][c] == MINE && (r != row || c != column)) 
                {
                    mines++;
                }
            }
        }
    }
    return mines;
}

bool indexes_within_boundary(int row, int column)
{
    //TODO replace MAXSQ with b.h and b.w
    return row >= 0 && row < MAXSQ && column >= 0 && column < MAXSQ;
}

int unknowns_in_neighbourhood(int row, int column, int grid[MAXSQ][MAXSQ])
{
    int counter = 0;
    for (int r = row - 1; r <= row + 1; r++)
    {
        for (int c = column - 1; c <= row + 1; c++)
        {
            if (indexes_within_boundary(r,c) && grid[r][c] == UNK)
            {
                counter++;
            }
        }
    }
    return counter;
}

int unknowns_to_mines(int row, int column, int grid[MAXSQ][MAXSQ], int amount)
{
    int cells_changed = 0;
    while (cells_changed < amount)
    {
        for (int r = row - 1; r <= row + 1; r++)
        {
            for (int c = column - 1; c <= column + 1; c++)
            {
                if (indexes_within_boundary(r,c) && grid[r][c] == UNK)
                {
                    grid[r][c] = MINE;
                    cells_changed++;
                }
            }
        }
    }
    return cells_changed;
}

void test(void)
{
    assert(is_valid_number('0') == true);
    assert(is_valid_number('8') == true);
    assert(is_valid_number('9') == false);
    assert(is_valid_number(MINE) == false);
    assert(is_valid_number(UNK) == false);

    //assets for tests
    int test_grid[MAXSQ][MAXSQ] = {
        {UNK,1,1,0,UNK},
        {1,2,3,2,1},
        {1,MINE,MINE,MINE,2},
        {1,2,4,MINE,2},
        {UNK,0,1,1,1}
    };
    int test_adjacent_mines[] = {0,0,0,0,0,  1,2,3,2,1,  1,1,3,2,2,  1,2,4,2,2, 0,0,1,1,1};

    // make_board()
    board test_board = make_board(5,5,5,"?110?123211XXX2124X2?0111");

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
    index = 0;

    // indexes_within_boundary()
    assert(indexes_within_boundary(-1,0) == false);
    assert(indexes_within_boundary(0,-1) == false);
    assert(indexes_within_boundary(MAXSQ,MAXSQ) == false);
    assert(indexes_within_boundary(0,0) == true);
    assert(indexes_within_boundary(MAXSQ-1,MAXSQ-1) == true);
}
