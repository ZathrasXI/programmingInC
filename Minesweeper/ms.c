#include "ms.h"
#include <ctype.h>

#define QUESTION_MARK 63
#define ASCII_X 88
#define ASCII_CONVERSION 48
#define MAX_DIGIT 8 

int unknown_cells_in_neighbourhood(int row, int column, int grid[MAXSQ][MAXSQ]);
int adjacent_mines(int row, int column, int grid[MAXSQ][MAXSQ]);
bool is_valid_number(char c);
bool indexes_within_grid(int row, int column);
bool should_be_a_mine(int row, int column, int grid[MAXSQ][MAXSQ]);
int question_marks_in_neighbourhood(int row, int column, int grid[MAXSQ][MAXSQ]);
int question_marks_to_mines(int row, int column, int grid[MAXSQ][MAXSQ], int changes);

board solve_board(board b)
{
    int mines_found = 0;
    while (mines_found <= b.totmines)
    {
        for (int i = 0; i < b.h; i++)
        {
            for (int j = 0; j < b.w; j++)
            {
                if (b.grid[i][j] == ASCII_X)
                {
                    mines_found++;
                }
                if (b.grid[i][j] <= MAX_DIGIT )
                {
                    if (indexes_within_grid(i,j) && b.grid[i][j] - question_marks_in_neighbourhood(i,j,b.grid) == adjacent_mines(i,j,b.grid))
                    {
                        int new_mines = question_marks_to_mines(i, j, b.grid, question_marks_in_neighbourhood(i,j,b.grid));
                        mines_found += new_mines;
                    }
                }

            }
        }
    }

    for (int i = 0; i < b.h; i++)
    {
        for (int j = 0; j < b.h; j++)
        {
            if (b.grid[i][j] == QUESTION_MARK)
            {
                int mines = adjacent_mines(i,j,b.grid);
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

int unknown_cells_in_neighbourhood(int row, int column, int grid[MAXSQ][MAXSQ])
{
    int cell_count = 0;
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            if (indexes_within_grid(i,j) && grid[i][j] == QUESTION_MARK && &grid[i][j] != &grid[row][column])
            {
                cell_count++;
            }
        }
    }
    return cell_count;
}

int adjacent_mines(int row, int column, int grid[MAXSQ][MAXSQ])
{
    int mines = 0;
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= column + 1; j++)
        {
            if (indexes_within_grid(i,j))
            {
                if (grid[i][j] == ASCII_X && (i != row || j != column)) 
                {
                    mines++;
                }
            }
        }
    }
    return mines;
}

bool indexes_within_grid(int row, int column)
{
    return row >= 0 && row < MAXSQ && column >= 0 && column < MAXSQ;
}

bool should_be_a_mine(int row, int column, int grid[MAXSQ][MAXSQ])
{
    for (int r = row - 1; r <= row + 1; r++)
    {
        for (int c = column - 1; c <= column + 1; c++)
        {
            if (indexes_within_grid(r,c) && (r != row || c != column) && grid[r][c] < 9)
            {
                int mines = adjacent_mines(r,c,grid);
                if (grid[r][c] > mines)
                {
                    printf("cell %d:%d value %d mines %d\n",r,c, grid[r][c], mines);
                    return true;
                }
            }
        }
    }
    return false;
}

int question_marks_in_neighbourhood(int row, int column, int grid[MAXSQ][MAXSQ])
{
    int counter = 0;
    for (int i = row - 1; i <= row + 1; i++)
    {
        for (int j = column - 1; j <= row + 1; j++)
        {
            if (grid[i][j] == QUESTION_MARK)
            {
                counter++;
            }
        }
    }
    return counter;
}

int question_marks_to_mines(int row, int column, int grid[MAXSQ][MAXSQ], int changes)
{
    int cells_changed = 0;
    while (cells_changed < changes)
    {
        for (int r = row - 1; r <= row + 1; r++)
        {
            for (int c = column - 1; c <= column + 1; c++)
            {
                if (indexes_within_grid(r,c) && grid[r][c] == QUESTION_MARK)
                {
                    grid[r][c] = ASCII_X;
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
        {QUESTION_MARK,1,1,0,QUESTION_MARK},
        {1,2,3,2,1},
        {1,ASCII_X,ASCII_X,ASCII_X,2},
        {1,2,4,ASCII_X,2},
        {QUESTION_MARK,0,1,1,1}
    };
    int test_adjacent_mines[] = {0,0,0,0,0,  1,2,3,2,1,  1,1,3,2,2,  1,2,4,2,2, 0,0,1,1,1};
    int test_unknown_cells[] = {0,1,0,1,0,  1,1,0,1,1, 0,0,0,0,0, 1,1,0,0,0, 0,1,0,0,0};

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
    
    // unknown_cells_in_neighbourhood()
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            assert(unknown_cells_in_neighbourhood(i,j,test_grid) == test_unknown_cells[index]);
            index++;
        }
    }

    // indexes_within_grid()
    assert(indexes_within_grid(-1,0) == false);
    assert(indexes_within_grid(0,-1) == false);
    assert(indexes_within_grid(MAXSQ,MAXSQ) == false);
    assert(indexes_within_grid(0,0) == true);
    assert(indexes_within_grid(MAXSQ-1,MAXSQ-1) == true);

    // should be a mine()
    // int test_minefield[MAXSQ][MAXSQ] = {
    //     {QUESTION_MARK,1,1,0,QUESTION_MARK},
    //     {1,QUESTION_MARK,2,1,0},
    //     {1,QUESTION_MARK,ASCII_X,QUESTION_MARK,1},
    //     {0,1,2,QUESTION_MARK,1},
    //     {QUESTION_MARK,0,1,1,1}
    // };
    // bool test_mines[] = {false,false,false,false,false,
    //                     false,true,false,false,false,
    //                     false,false,false,false,false,
    //                     false,false,false,true,false,
    //                     false,false,false,false,false};
    
    // int counter = 0;
    // for (int i = 0; i < 5; i++)
    // {
    //     for (int j = 0; j < 5; j++)
    //     {
    //         // assert(should_be_a_mine(i,j,test_minefield) == test_mines[counter]);
    //         if (should_be_a_mine(i,j,test_minefield) != test_mines[counter])
    //         {
    //             printf("test minefinder: %d\n", counter);
    //         }
    //         counter++;
    //     }
    // }
}
