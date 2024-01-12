#!/bin/bash

# Check if the Makefile exists
if [ ! -e "../Parse/Makefile" ]; then
    echo "No Makefile found"
    exit 1
fi

# Compile parser
cd ../Parse
make test >> ../Testing/results.parser
if [ $? -ne 0 ]; then 
    echo "Error compiling"
    exit 1
fi

# Run parser against every file
TTLs="../TTLs"
expected_failure="fail_parse_ok_interp.ttl"

for file in "$TTLs"/*; do
    if [ -f "$file" ]; then
        ./test_parser "$file" >> ../Testing/results.parser
        if [[ "$file" == "$expected_failure" && $? == 0 ]]; then
            echo "test $file failed" >> ../Testing/results.parser
        fi
        if [[ "$file" != "$expected_failure" && $? != 0 ]]; then
            echo "test $file failed" >> ../Testing/results.parser
        fi
    fi
done