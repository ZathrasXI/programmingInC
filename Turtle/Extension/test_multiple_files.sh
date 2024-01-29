#!/bin/bash

dir="./Results"
if [ ! -d "$dir" ]; then
    mkdir "$dir"
fi

make > /dev/null

./extension ../TTLs/octagon2.ttl ../TTLs/spiral.ttl ../TTLs/downarrow.ttl ../TTLs/donothing.ttl ../TTLs/labyrinth.ttl -ps -txt

if [[ $? != 0 ]]; then
    echo "FAIL: exit code 1 when running program"
    make clean > /dev/null
    rm ./Results/* > /dev/null
    exit 1
fi

names=("octagon2" "spiral" "downarrow" "donothing" "labyrinth")
for name in "${names[@]}"; do
    cmp "./Results/$name.ps" "./test_files/ps/$name.ps"
    if [[ $? != 0 ]]; then
        echo "FAIL: files not identical $name"
        make clean > /dev/null
        rm ./Results/* > /dev/null
        exit 1
    fi

    cmp "./Results/$name.txt" "./test_files/txt/$name.txt"
    if [[ $? != 0 ]]; then
        echo "FAIL: files not identical $name"
        make clean > /dev/null
        rm ./Results/* > /dev/null
        exit 1
    fi
done

make clean > /dev/null
rm ./Results/* > /dev/null