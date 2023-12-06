CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

alloc: alloc.c specific.h
	$(CC) $(SANITIZE) alloc.c specific.h -o alloc