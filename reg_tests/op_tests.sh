#!/bin/bash

run_bc_op() {
    bc_op_val=$(echo "$1 $bc_op $2" | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
}

run_bc() {
    bc_val=$(echo $1 | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
}

run_test_and_compare() {
    let "total+=1"
    run_bc_op $1 $2
    t_val=$(./operator_drv $base_arg $op $1 $2)
    if [ $t_val != $bc_op_val ]; then
        let "local_fail+=1"
        let "fail_count+=1"
        echo "test has failed -- $1 $op $2"
    fi
}

base_arg=-b2
if [[ "$1" == "-b2" ]] || [[ "$1" == "-b63" ]]; then
    base_arg=$1
fi

declare -A op_array
op_array[-a]=+
op_array[-s]=-
op_array[-m]=*
op_array[-d]=/
op_array[-r]=%

declare -A stub_array
stub_array[-a]=add
stub_array[-s]=subtract
stub_array[-m]=multiply
stub_array[-d]=divide
stub_array[-r]=modulo

op=-a
for i in -a -s -m -d -r
do
    if [[ "$i" == "$2" ]]; then
        op=$i
    fi
done
bc_op="${op_array[$op]}"

local_fail=0
echo "sign tests"
a=16383
b=31
run_test_and_compare $a $b
run_test_and_compare -$a $b
run_test_and_compare $a -$b
run_test_and_compare -$a -$b

echo "unity tests"
local_fail=0
run_bc "3^123 + 5^45 + 7^6"
a=$bc_val
run_test_and_compare $a 1
run_test_and_compare 1 $a
run_test_and_compare 1  1


if [[ "$op" != "-d" ]] && [[ "$op" != "-r" ]]; then
    echo "zero tests"
    run_test_and_compare $a 0
    run_test_and_compare 0 $a
    run_test_and_compare 0  0
fi

echo "shift tests"
local_fail=0
run_bc "7^156"
x=$bc_val
run_bc "11^23"
b=$bc_val
i=100
while [ $i -le 130 ]
do
    run_bc "$x + 3^$i"
    a=$bc_val
    run_test_and_compare $a $b 
    echo -en "\b|"
    run_test_and_compare $b $a 
    echo -en "\b/"
    run_test_and_compare $a $a 
    echo -en "\b-"
    run_test_and_compare $b $b 
    echo -en "\b\\"
    let "i=$i+1"
done
echo -en "\b"

echo "large number tests"
local_fail=0
run_bc "2^360 + 3^234 + 5^67"
a=$bc_val
run_bc "2^100 + 3^67 + 7^9"
b=$bc_val
run_test_and_compare $a $b
run_test_and_compare $b $a

echo -n "base is $base_arg "
echo -n "operation is "${stub_array[$op]}" "
echo -n "tests run:$total "
echo "tests failed:$local_fail"
