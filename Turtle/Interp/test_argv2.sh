#!/bin/bash

files=("octagon1" "octagon2" "spiral" "tunnel" "turn" "donothing" "downarrow" "empty" "labyrinth")

mkdir testing
make

for file in "${files[@]}"; do
    ./interpreter "../TTLs/$file.ttl" "./testing/$file.out"
    cmp "./test_files/txt/$file.out" "./testing/$file.out"
    if [[ $? != 0 ]]; then 
        echo "FAIL: files are not identical - "../TTLs/$file.ttl" and "./testing/$file.out""
        rm -r testing/
        exit 1
    fi
done

rm -r testing/