#!/bin/bash
source ./func.sh

num=31
bc_bits=$(echo "obase=2;$num" | bc)
ds_bits=$(./disp_test $num)
match $bc_bits $ds_bits "display test"

large_num=$(echo "2^1000" | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
bc_bits=$(echo "obase=2;$large_num" | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
ds_bits=$(./disp_test $large_num)
match $bc_bits $ds_bits "large number test"

num=-56
bc_bits=$(echo "obase=2;$num" | bc)
ds_bits=$(./disp_test $num)
match $bc_bits $ds_bits "sign test"

