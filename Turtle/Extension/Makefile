CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

extension: extension.h extension.c stack.c test_extension.c
	$(CC) extension.c $(COMMON) -o extension -lm -pthread

val: extension.c extension.h
	$(CC) extension.c $(VALGRIND) -o val_extension -lm -pthread

cov: extension.c 
	gcc -fprofile-arcs -ftest-coverage -o extension extension.c -pthread -lm
	
clean_cov:
	rm *.gc* extension

clean:
	rm extension
