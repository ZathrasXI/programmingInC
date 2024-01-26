gcc -fprofile-arcs -ftest-coverage -o interp interp.c -lm
./interp 
gcov interp.c

rm *.gc* interp
