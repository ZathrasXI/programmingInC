#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.14159265358979323846

typedef struct step
{
    int row;
    int col;
} Step;

typedef struct route
{
    Step s[25];
    int len;
    float angle;
} Route;

void setPixel(int x, int y, Route *r);
void calulcate_line_coords(int x0, int y0, int x1, int y1, Route *r);
void find_end_points(float x0, float y0, float input_length, int x_y[2], Route r);

void setPixel(int x, int y, Route *r)
{
    r->s[r->len].row = y;
    r->s[r->len].col = x;
    r->len++;
}

void find_end_points(float x0, float y0, float input_length, int x_y[2], Route r)
{
    // x1 should be 0 when drawing a straight line, but it's 1
    float x1 = round(x0 + (input_length * sin(r.angle * PI/180)));
    float y1 = round(y0 + (input_length * cos(r.angle * PI/180)));
    
    printf("start x %lf start y %lf end x %lf end y %lf\n", x0, y0, x1, y1);
    x_y[0] = (int) round(x1);
    x_y[1] = (int) round(y1);
    // calulcate_line_coords((int) round(x0), (int) round(y0), (int) x1, (int) y1, r);
}

void calulcate_line_coords(int x0, int y0, int x1, int y1, Route *r)
{
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; /* error value e_xy */
    //TODO: while loop
    for (;;)
    { /* loop */
        //TODO: test left shift `err<<2` should have the same effect, but be quicker
        e2 = 2*err;
        if (e2 >= dy) 
        { /* e_xy+e_x > 0 */
            if (x0 == x1) break;
            err += dy; x0 += sx;
        }
        if (e2 <= dx) 
        { /* e_xy+e_y < 0 */
            if (y0 == y1) break;
            err += dx; y0 += sy;
        }
        // setPixel(x0,y0,r);
        r->s[r->len].col = x0;
        r->s[r->len].row = y0;
        r->len++;
    }
}



// void drawLine(int rows, int cols, int array[rows][cols], int startX, int startY, int length, double angleDegrees) {
//     double angleRadians = angleDegrees * (PI / 180.0);
//     	printf("radians %lf\n", angleRadians);
//     for (int i = 0; i < length + 1; i++) {
//         int row = startX - (int)round(sqrt(2) * i * cos(angleRadians));
//         int col = startY + (int)round(sqrt(2) * i * sin(angleRadians));
//         printf("\nbefore round\n row %lf  col %lf\n", i * cos(angleRadians), i * sin(angleRadians));
// 	printf("row %d col %d\n", row,col);

//         if (row >= 0 && row < rows && col >= 0 && col < cols) {
//             array[row][col] = 1; // Set the element to 1 to represent the line
//         }
//     }
// }

void printArray(int rows, int cols, int array[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}

int main() {
    Route r;
    r.s[0].col = 5;
    r.s[0].row = 5;
    r.len = 1;
    r.angle = 90;
    int x1_y1[2];
    find_end_points(0, 0, 8, x1_y1, r);
    printf("%d %d\n\n", x1_y1[0], x1_y1[1]);
    calulcate_line_coords(0, 0, x1_y1[0], x1_y1[1], &r);
    // drawLine(r.s[r.len-1].row, r.s[r.len-1].col, 8, &r);
    // printf("end angle %lf steps taken %d\n\n\n", r.angle, r.len);

    for (int i = 0; i < r.len; i++)
    {
        printf("row %d col %d\n", r.s[i].row, r.s[i].col);
    }

    printf("correct len? %d\n", r.len == 9);



    // int arr[50][50] = {0}; // Initialize the array with zeros
    // drawLine(10, 20, arr, 10, 0, 8, 45); // Draw a line starting from (5, 5) with length 8 and angle 62 degrees
    // printArray(10, 20, arr); // Display the resulting array

    return 0;
}

