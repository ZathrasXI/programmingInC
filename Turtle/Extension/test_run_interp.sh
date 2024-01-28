make > /dev/null
./interpreter > /dev/null 
if [ $? -ne 1 ]; then
    echo "fail: exit code != 1; when running ./interp without args"
    exit 1
fi
make clean > /dev/null