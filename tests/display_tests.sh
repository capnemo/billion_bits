#!/bin/bash

match (){
    
    if [ $1 == $2 ]; then
        echo "$3 test passed"
    else
        echo "$3 test failed"
    fi
}

num=31
bc_bits=$(echo "obase=2;$num" | bc)
ds_bits=$(./disp_test $num)
match $bc_bits $ds_bits display

large_num=$(echo "2^100" | bc)
bc_bits=$(echo "obase=2;$large_num" | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
ds_bits=$(./disp_test $large_num)
match $bc_bits $ds_bits "large number"

num=-56
bc_bits=$(echo "obase=2;$num" | bc)
ds_bits=$(./disp_test $num)
match $bc_bits $ds_bits sign

