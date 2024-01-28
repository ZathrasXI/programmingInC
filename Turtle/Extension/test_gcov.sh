gcc -fprofile-arcs -ftest-coverage -o interp interp.c -lm
./interp > /dev/null
gcov interp.c

rm *.gc* interp
