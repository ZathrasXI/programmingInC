make > /dev/null
./interpreter > /dev/null 
if [ $? -ne 1 ]; then
    echo "fail: exit code != 1; when running ./interp without args"
    exit 1
fi

./interpreter "../TTLs/fail_parse_ok_interp.ttl" 2> /dev/null
if [ $? -ne 0 ]; then
    echo "fail: `./interpreter ../TTLs/fail_parse_ok_interp.ttl` should exit 0"
fi
make clean > /dev/null
