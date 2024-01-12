CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

parse: parse.h parse.c
	$(CC) parse.c $(SANITIZE) -o parser

val: parse.c parse.h
	$(CC) parse.c $(VALGRIND) -o val_parser

cov: parse.c 
	gcc -fprofile-arcs -ftest-coverage -o parse parse.c
	
clean_cov:
	rm *.gc* parse

clean:
	rm parser 