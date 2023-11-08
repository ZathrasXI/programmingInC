#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_HEIGHT 20
#define MAX_WIDTH 20
#define COORD_STR 5

void maze_size(char input[COORD_STR], int width_height[2]);
void create_maze_array(FILE* maze_file, char maze[MAX_HEIGHT][MAX_WIDTH], int width_height[2]);
bool index_is_on_boundary(int row, int col, int width_height[2]);
void test(void);


int main(int argc, char* argv[])
{
    test();
    char maze[MAX_HEIGHT][MAX_WIDTH];

    if (argc != 2)
    {
        fprintf(stderr,"useage: ./maze <maze_file>\n");
        exit(EXIT_FAILURE);
    }

    FILE* maze_file = fopen(argv[1], "r");
    if (!maze_file)
    {
        fprintf(stderr, "file not opened\n");
        exit(EXIT_FAILURE);
    }

    char co_ords[COORD_STR];
    if (fgets(co_ords, 5, maze_file) == NULL)
    {
        fprintf(stderr, "Could not read 1st line of %s\n", co_ords);
        exit(EXIT_FAILURE);
    }

    int width_height[2];
    maze_size(co_ords, width_height);

    create_maze_array(maze_file, maze, width_height);

    // print maze
    // for (int row = 0; row < width_height[1]; row++)
    // {
    //     for (int col = 0; col < width_height[0]; col++)
    //     {
    //         printf("%c", maze[row][col]);
    //     }
    // }

    // find entrance


    fclose(maze_file);

}

void maze_size(char input[COORD_STR], int width_height[2])
{
    int i = 0;
    for (int c = 0; c < COORD_STR; c++)
    {
        if (isdigit(input[c]))
        {
            width_height[i] = input[c] - '0';
            i++;
        }
    }
}

void create_maze_array(FILE* maze_file, char maze[MAX_HEIGHT][MAX_WIDTH], int width_height[2])
{
    char c;
    int row = 0, col = 0;
    while(((c = fgetc(maze_file)) != EOF))
    {
        maze[row][col] = (char) c;
        col++;
        if (col == width_height[0])
        {
            row++;
            col = 0;
        }
    }
}

bool index_is_on_boundary(int row, int col, int width_height[2])
{
    if (row == 0 && col == 0)
    {
        return true;
    }
    if (row == 0 && col == width_height[0] - 1)
    {
        return true;
    }
    if (row == width_height[1] - 1 && col == 0)
    {
        return true;
    }
    if (row == width_height[1] - 1 && col == width_height[1] - 1 )
    {
        return true;
    }
    return false;
}

void test(void)
{
    int xy[] = {8,8};
    assert(index_is_on_boundary(0,0,xy));
    assert(index_is_on_boundary(7,7,xy));
    assert(index_is_on_boundary(0,7,xy));
    assert(index_is_on_boundary(7,0,xy));
    assert(index_is_on_boundary(8,8,xy) == false);
}
