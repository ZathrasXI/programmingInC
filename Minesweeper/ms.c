#include "ms.h"
#include <ctype.h>
#include <stdlib.h>

#define ASCII_CONVERSION 48
#define MAX_DIGIT 8
#define MAX_ATTEMPTS 5

int adjacent_mines(int row, int column, board* b);
bool is_valid_number(char c);
bool indexes_within_boundary(int row, int column, board* b);
int unknowns_in_neighbourhood(int row, int column, board* b);
int unknowns_to_mines(int row, int column, board* b, int amount);

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
                    if (b.grid[i][j] - unknowns_in_neighbourhood(i,j,&b) == adjacent_mines(i,j,&b))
                    {
                        mines_found += unknowns_to_mines(i, j, &b, unknowns_in_neighbourhood(i,j,&b));
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
                b.grid[i][j] = adjacent_mines(i,j,&b);
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

int adjacent_mines(int row, int column, board* b)
{
    int mines = 0;
    for (int r = row - 1; r <= row + 1; r++)
    {
        for (int c = column - 1; c <= column + 1; c++)
        {
            if (indexes_within_boundary(r,c,b))
            {
                if (b->grid[r][c] == MINE && (r != row || c != column)) 
                {
                    mines++;
                }
            }
        }
    }
    return mines;
}

bool indexes_within_boundary(int row, int column, board* b)
{
    return row >= 0 && row < b->h && column >= 0 && column < b->w;
}

int unknowns_in_neighbourhood(int row, int column, board* b)
{
    int counter = 0;
    for (int r = row - 1; r <= row + 1; r++)
    {
        for (int c = column - 1; c <= row + 1; c++)
        {
            if (indexes_within_boundary(r,c,b) && b->grid[r][c] == UNK)
            {
                counter++;
            }
        }
    }
    return counter;
}

int unknowns_to_mines(int row, int column, board* b, int amount)
{
    int cells_changed = 0;
    while (cells_changed < amount)
    {
        for (int r = row - 1; r <= row + 1; r++)
        {
            for (int c = column - 1; c <= column + 1; c++)
            {
                if (indexes_within_boundary(r,c,b) && b->grid[r][c] == UNK)
                {
                    b->grid[r][c] = MINE;
                    cells_changed++;
                }
            }
        }
    }
    return cells_changed;
}

void test(void)
{
    //assets for tests
    int test_grid[5][5] = {
        {UNK,1,1,0,UNK},
        {1,2,3,2,1},
        {1,MINE,MINE,MINE,2},
        {1,2,4,MINE,2},
        {UNK,0,1,1,1}
    };
    int test_adjacent_mines[] = {0,0,0,0,0,  1,2,3,2,1,  1,1,3,2,2,  1,2,4,2,2, 0,0,1,1,1};
    board test_unk = make_board(0,5,5,"???000???0??0?00???0???0?");
    board test_no_unk = make_board(0,5,5,"1234512345123451234512345");

    // is_valid_number()
    assert(is_valid_number('0') == true);
    assert(is_valid_number('8') == true);
    assert(is_valid_number('9') == false);
    assert(is_valid_number(MINE) == false);
    assert(is_valid_number(UNK) == false);


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
    int mines = adjacent_mines(0,0,&test_board);
    int index = 0;
    assert(mines == 0);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            assert(adjacent_mines(i,j,&test_board) == test_adjacent_mines[index]);
            index++;
        }
    }
    index = 0;

    // indexes_within_boundary()
    assert(indexes_within_boundary(-1, 0, &test_board) == false);
    assert(indexes_within_boundary(0, -1, &test_board) == false);
    assert(indexes_within_boundary(MAXSQ, MAXSQ, &test_board) == false);
    assert(indexes_within_boundary(0, 0, &test_board) == true);
    assert(indexes_within_boundary(4, 4, &test_board) == true);

    // unknowns_in_neighbourhood()
    assert(unknowns_in_neighbourhood(0,1,&test_board) == 1);
    assert(unknowns_in_neighbourhood(2,2,&test_unk) == 8);
    assert(unknowns_in_neighbourhood(2,2,&test_no_unk) == 0);

    // unknowns_to_mines()
    assert(unknowns_to_mines(0,1,&test_board,1) == 1);
    assert(unknowns_to_mines(2,2,&test_unk,8) == 8);
    assert(unknowns_to_mines(2,2,&test_no_unk,0) == 0);

    // solve_board() && board2str()
    char s[MAXSQ*MAXSQ+1];
    board b1;

    strcpy(s, "?100?232101XX21123X1?0111");
    assert(syntax_check(3, 5, 5, s)==true);
    b1 = make_board(5,5,5,s);
    b1 = solve_board(b1);
    board2str(s, b1);
    assert(strcmp(s, "X1000232101XX21123X100111") == 0);

    strcpy(s, "XXXXXXXXXXXX");
    assert(syntax_check(12, 3, 4, s)==true);
    b1 = make_board(12,3,4,s);
    b1 = solve_board(b1);
    board2str(s, b1);
    assert(strcmp(s, "XXXXXXXXXXXX") == 0);

    strcpy(s, "?");
    assert(syntax_check(0, 1, 1, s)==true);
    b1 = make_board(0,1,1,s);
    b1 = solve_board(b1);
    board2str(s, b1);
    assert(strcmp(s, "0") == 0);

    strcpy(s, "X?X?X?X?");
    assert(syntax_check(4, 2, 4, s)==true);
    b1 = make_board(4,2,4,s);
    b1 = solve_board(b1);
    board2str(s, b1);
    assert(strcmp(s, "X2X3X3X2") == 0);

    strcpy(s, "???????X???????");
    assert(syntax_check(1,3,5,s)==true);
    b1 = make_board(1,3,5,s);
    b1 = solve_board(b1);
    board2str(s, b1);
    assert(strcmp(s, "0001111X1111000") == 0);

    strcpy(s, "???????????????");
    assert(syntax_check(0,3,5,s)==true);
    b1 = make_board(0,3,5,s);
    b1 = solve_board(b1);
    board2str(s, b1);
    assert(strcmp(s, "000000000000000") == 0);

    strcpy(s, "X??????????????");
    assert(syntax_check(1,3,5,s)==true);
    b1 = make_board(1,3,5,s);
    b1 = solve_board(b1);
    board2str(s, b1);
    assert(strcmp(s, "X10110000000000") == 0);

    strcpy(s, "XXX?????????232");
    assert(syntax_check(6,3,5,s)==true);
    b1 = make_board(6,3,5,s);
    b1 = solve_board(b1);
    board2str(s, b1);
    assert(strcmp(s, "XXX232232XXX232") == 0);

    strcpy(s, "???????8???????");
    assert(syntax_check(6,3,5,s)==true);
    b1 = make_board(6,3,5,s);
    b1 = solve_board(b1);
    board2str(s, b1);
    assert(strcmp(s, "232XXXX8XXXX232") == 0);
}
