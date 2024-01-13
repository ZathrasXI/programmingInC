CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

interp: interp.h interp.c
	$(CC) interp.c $(SANITIZE) -o interpr

val: interp.c interp.h
	$(CC) interp.c $(VALGRIND) -o val_interpr

cov: interp.c 
	gcc -fprofile-arcs -ftest-coverage -o interp interp.c
	
clean_cov:
	rm *.gc* interp

clean:
	rm interpr 