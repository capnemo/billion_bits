#!/bin/bash

source ./func.sh

run_sub_test() {
    f_ret=$(./op_test -s $1 $2 | head -1)
}

a=2323
b=23
run_sub_test $a $b
match $f_ret 2300 "sub1 test"

a=100000000000000000000000000000000000000000000000000000000000000000000000000
run_sub_test $a $a
match $f_ret 0 "sub2 test"

a=-1221
run_sub_test $a $a
match $f_ret 0 "sub3 test"

a=10000000000000000000000000000000000000000000000000
b=1
d=9999999999999999999999999999999999999999999999999
run_sub_test $a $b
match $f_ret $d "sub4 test"

