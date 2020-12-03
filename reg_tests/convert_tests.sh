#!/bin/bash

match (){
    let "total=$total+1"
    if [ $1 != $2 ]; then
        let "mf=$mf+1"
        echo "$3 has failed"
    fi
}

run_bc() {
    bc_val=$(echo $1 | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
}

echo "shift display tests"
total=0
i=0
mf=0
while [ $i -le 70 ]
do
    b10num=$(echo 2^$i | bc)
    b2_num=$(./conv_drv -x $b10num)
    run_bc "obase=2;2^$i"
    match $b2_num $bc_val "test $i"
    let "i=$i+1"
done
let "tot_fail=$tot_fail + $mf"

echo "large number conversion test"
num="2^10000+7^89"
run_bc $num
b10num=$bc_val
b2num=$(./conv_drv -x $b10num)
run_bc "obase=2;$num"
match $bc_val $b2num "large number conversion test"

echo "vector conversion tests"
run_bc "obase=2;2^126 + 2^63 + 2^62"
b2_num=$(./conv_drv -v 126 63 62)
match $bc_val $b2_num "b63 vector test"

echo "total tests run:$total"
echo "number of failed tests:$mf"
