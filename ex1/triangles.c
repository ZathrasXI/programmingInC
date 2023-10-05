#include "stdio.h"
#include "assert.h"
#include "math.h"
#include "stdbool.h"

enum triangle { equalateral, scalene, isosceles, not_triangle };
typedef enum triangle triangle;


bool is_right_angle(int a, int b, int c);
bool is_triangle(int a, int b, int c);
triangle which_triangle(int a, int b, int c);


int main(void){
	assert(is_right_angle(1,2,3) == false);
	assert(is_right_angle(3,4,5) == true);
	
	assert(is_triangle(-1,4,5) == false);
	assert(is_triangle(1,1,10) == false);
	assert(is_triangle(3,4,5) == true);

	assert(which_triangle(3,4,5) == scalene);
	assert(which_triangle(2,2,4) == isosceles);
	assert(which_triangle(3,3,3) == equalateral);
	
	int a, b, c, longest_side, input;
	printf("Enter 3 numbers: ");
	if (scanf("%d %d %d", &a, &b, &c) != 3) {
		printf("Enter 3 numbers");
		return 1;
	}

	while (is_triangle(a,b,c))
	{
		if (is_right_angle(a,b,c)) {
			printf("Right angle ");
		}

		triangle current_triangle = which_triangle(a, b, c);
		switch (current_triangle)
		{
		case equalateral:
			printf("equalateral\n");
			break;
		case scalene:
			printf("scalene\n");
			break;
		case isosceles:
			printf("isosceles\n");
			break;
		default:
			printf("not sure....\n");
			break;
		}
		printf("Enter 3 numbers: ");
		scanf("%d %d %d", &a, &b, &c);
	}
}


bool is_triangle(int a, int b, int c){
	if (a <= 0 || b <= 0 || c <= 0) {
		return false;
	}
	if (a + b < c || a + c < b || b + c < a ) {
		return false;
	}
	return true;
}


triangle which_triangle(int a, int b, int c) {
	if(a != b && a != c){
		return scalene;
	}

	if(a == b && a == c){
		return equalateral;
	}

	if(a == b || a == c || b == c){
		return isosceles;
	}
	return not_triangle;
}


bool is_right_angle(int a, int b, int c){
	int longest_side;
	if(a > b && a > c){
		longest_side = a;
		if(a == sqrt(c * c + b * b)){
		    return true;
		}
	}

	if(b > c && b > a){
		longest_side = b;
		if(b == sqrt(a * a + c * c)){
		    return true;
		}
	}

	longest_side = c;
	if(c == sqrt(b * b + a * a)){
	    return true;
	}
	return false;
}

