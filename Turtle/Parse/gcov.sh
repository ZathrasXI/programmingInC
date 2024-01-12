# generate report of test coverage

gcc -fprofile-arcs -ftest-coverage -o parse parse.c
./parse
gcov parse.c
