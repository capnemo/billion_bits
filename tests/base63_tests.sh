#!/bin/bash
source ./func.sh

i=0
f=0
while [ $i -le 70 ]
do
    b10num=$(echo 2^$i | bc)
    b2_num=$(./b63_test -x $b10num)
    run_bc "obase=2;2^$i"
    if [ $b2_num != $bc_val ]; then
        echo "test $i has failed"
        let "f=$f + 1"
    fi
    let "i=$i+1"
done
check_fail $f "b63 display"

run_bc "obase=2;2^126 + 2^63 + 2^62"
b2_num=$(./b63_test -v 126 63 62)
match $bc_val $b2_num "b63 vector test"

run_bc 2^10000
b10num=$bc_val
b2num=$(./b63_test -x $b10num)
run_bc "obase=2;2^10000"
match $bc_val $b2num "b63 display large number test"

run_bc 2^1000
b2_num=$(./b63_test -a $bc_val $bc_val)
run_bc "obase=2;2^1001"
match $bc_val $b2_num "large number add test"

a=16383
b=31
run_bc "obase=2;$a + $b"
b63_sum=$(./b63_test -a $a $b)
match $bc_val $b63_sum "basic add test 1"

b63_sum=$(./b63_test -a $b $a)
match $bc_val $b63_sum "basic add test 2"

run_bc "obase=2;2^101"
sum=$bc_val
i=0
f=0
while [ $i -le 20 ]
do
    run_bc "2^100 + $i"
    a=$bc_val
    run_bc "2^100 - $i"
    b=$bc_val
    b63_sum=$(./b63_test -a $a $b)
    if [ $sum != $b63_sum ]; then
        echo "$a + $b failed"
        let "f=$f + 1"
    fi
    let "i=$i+1"
done
check_fail $f "add"
    
run_bc "2^126 + 2^63 + 2^62"
arg=$bc_val
b2_num=$(./b63_test -a $arg $arg)
run_bc "obase=2;$arg*2"
match $bc_val $b2_num "b63 vector add test"

i=1
x62=$(echo 2^62+2^61 | bc)

while [ $i -le 30 ]
do
    x=$(echo 2^$i | bc)
    bc_num=$(echo "$x62 * $x" | bc)
    b63_num=$(./b63_test -m $x62 $x)
    if [ $bc_num != $b63_num ]; then
        echo "multiply shift test $i failed"
        let "f=$f+1"
    fi

    let "i=$i+1"
done
check_fail $f "multiply shift"

run_bc "2^100 + 3^3 + 4^4 + 5^5 + 6^6"
arg=$bc_val
b63_num=$(./b63_test -m $arg $arg)
run_bc $arg*$arg
match $bc_val $b63_num "large number multiply"

i=59
f=0
while [ $i -le 70 ]
do
    bc_num=$(echo "2^$i * 32" | bc)
    b63_num=$(./b63_test -m `echo 2^$i | bc` 32)
    if [ $bc_num != $b63_num ]; then
        echo "multiply shift test $i failed"
        let "f=$f+1"
    fi
    let "i=$i+1"
done
check_fail $f "multiply"


