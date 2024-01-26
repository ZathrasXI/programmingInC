#!/bin/bash


TTLs="../TTLs"

# test output of .txt
files=("octagon1" "octagon2" "spiral" "tunnel" "turn" "donothing" "downarrow" "empty" "labyrinth")
mkdir output_tmp
make 

for file in "${files[@]}"; do
    ./interpreter "../TTLs/$file.ttl" "./output_tmp/$file.txt"
    cmp "./test_files/txt/$file.txt" "./output_tmp/$file.txt"
    if [[ $? != 0 ]]; then 
        echo "FAIL: files are not identical - "../test_files/text/$file.txt" and "./output_tmp/$file.txt""
        rm -r output_tmp/
        exit 1
    fi
done

rm output_tmp/*
names=$(ls ../TTLs/ | grep -o '^[^.]*')

# test output of .ps
for name in $names; do
    ./interpreter ../TTLs/${name}.ttl ./output_tmp/${name}.ps
    cmp ./output_tmp/${name}.ps ./test_files/ps/${name}.ps
    if [[ $? != 0 ]]; then
        echo "FAIL: files not identical - ./output_tmp/${name}.ps ./test_files/${name}.ps"
        rm -r output_tmp/
        make clean
        exit 1
    fi
done


rm -r output_tmp 
make clean