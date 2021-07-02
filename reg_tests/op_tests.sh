#!/bin/bash

run_bc_op() {
    echo "$1 $bc_op $2" | bc | tr -d "\n" | tr -d "\\" 2>/dev/null
}

run_bc() {
    echo $1 | bc | tr -d "\n" | tr -d "\\" 2>/dev/null
}

run_test_and_compare() {
    let "total+=1"
    bc_op_val=$(run_bc_op $1 $2)
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

op=-a
for i in -a -s -m -d -r
do
    if [[ "$i" == "$2" ]]; then
        op=$i
    fi
done
#bc_op="${op_array[$op]}"
bc_op=+

if [[ "$op" == "-a" ]]
then
    bc_op=+
    op_name="add"
elif [[ "$op" == "-s" ]]
then
    bc_op=-
    op_name="subtract"
elif [[ "$op" == "-m" ]]
then
    bc_op=*
    op_name="multiply"
elif [[ "$op" == "-d" ]]
then
    bc_op=/
    op_name="divide"
elif [[ "$op" == "-r" ]]
then
    bc_op=%
    op_name="modulo"
fi

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
a=$(run_bc "3^123 + 5^45 + 7^6")
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
x=$(run_bc "7^156")
b=$(run_bc "11^23")
i=100
while [ $i -le 130 ]
do
    a=$(run_bc "$x + 3^$i")
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
a=$(run_bc "2^360 + 3^234 + 5^67")
b=$(run_bc "2^100 + 3^67 + 7^9")
run_test_and_compare $a $b
run_test_and_compare $b $a

echo -n "base is $base_arg "
echo -n "operation is $op_name "
echo -n "tests run:$total "
echo "tests failed:$local_fail"
