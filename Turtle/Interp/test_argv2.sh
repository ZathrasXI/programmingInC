#!/bin/bash

# test output of .txt
files=("octagon1" "octagon2" "spiral" "tunnel" "turn" "donothing" "downarrow" "empty" "labyrinth")
mkdir ./Results/output_tmp
make > /dev/null

for file in "${files[@]}"; do
    ./interpreter "../TTLs/$file.ttl" "./output_tmp/$file.txt"
    cmp "./test_files/txt/$file.txt" "./Results/output_tmp/$file.txt"
    if [[ $? != 0 ]]; then 
        echo "FAIL: files are not identical - "../test_files/text/$file.txt" and "./Results/output_tmp/$file.txt""
        rm -r ./Results/output_tmp/ > /dev/null
        make clean > /dev/null
        exit 1
    fi
done

rm ./Results/output_tmp/*
names=$(ls ../TTLs/ | grep -o '^[^.]*')

# test output of .ps
for name in $names; do
    ./interpreter ../TTLs/${name}.ttl ./output_tmp/${name}.ps
    cmp ./test_files/ps/${name}.ps ./Results/output_tmp/${name}.ps
    if [[ $? != 0 ]]; then
        echo "FAIL: files not identical - ./test_files/${name}.ps ./Results/output_tmp/${name}.ps"
        rm -r ./Results/output_tmp/ > /dev/null
        make clean > /dev/null
        exit 1
    fi
done


rm -r ./Results/output_tmp > /dev/null
make clean > /dev/null