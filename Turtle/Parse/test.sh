#!/bin/bash

#compile parser using gcc and clang,
#run both files against all of the test files in ../TTL/
#assert that the exit status is correct

#define flags
COMMON="-Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror"
DEBUG="-g3"
SANITIZE="$COMMON -fsanitize=undefined -fsanitize=address $DEBUG"

compilers=("gcc" "clang")
for comp in "${compilers[@]}"; do

    # Compile parser using gcc
    ${comp} parse.c ${SANITIZE} -o test_${comp}_parser
    if [ $? -ne 0 ]; then 
        echo "Error compiling"
        exit 1
    fi

    # Run parser against every file
    TTLs="../TTLs"
    expected_failure="fail_parse_ok_interp.ttl"

    for file in "$TTLs"/*; do
        if [ -f "$file" ]; then
            ./test_${comp}_parser "$file" 2> /dev/null
            if [[ "$file" == "$expected_failure" && $? == 0  ]]; then
                echo "FAIL: compiler: ${comp} - $file" 
            fi
            if [[ "$file" != "$expected_failure" && $? != 0 ]]; then
                echo "FAIL: compiler: ${comp} - $file" 
            fi
        fi
    done

    #rm compiled file 
    rm test_${comp}_parser
done