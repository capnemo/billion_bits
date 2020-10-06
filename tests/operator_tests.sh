#!/bin/bash

match (){
    
    if [ $1 == $2 ]; then
        echo "$3 has passed"
    else
        echo "$3 has failed"
    fi
}

check_fail() {

    if [ $1 -gt 0 ]; then
        echo "Some $2 test(s) failed" 
    else
        echo "all $2 tests have passed"
    fi

}

a=0
b=12
bc_sum=$(echo "$a + $b" | bc)
b2_sum=$(./op_test -a $a $b | head -1)
match $bc_sum $b2_sum "add1 test"

i=0
x=75
let "sum=2*x"
f=0
while [ $i -le 10 ]
do
    let "a=$x+$i"
    let "b=$x-$i"
    b2_sum=$(./op_test -a $a $b | head -1)
    if [ $sum != $b2_sum ]; then
        echo "$a + $b failed"
        let "f=$f+1"
    fi
    let "i=$i+1"
done

check_fail $f "add2"

i=0
a=1123
b=-1123
f=0
while [ $i -le 10 ]
do 
    b2_sum=$(./op_test -a $a $b | head -1)
    if [ $b2_sum != 0 ]; then
        echo "$a + $b failed"
        let "f=$f+1"
    fi
    let "i=$i+1"
    let "a=$a-1"
    let "b=$b+1"
done
check_fail $f "add3"

big_num=$(echo "2^120" | bc)
big_num_sum=$(echo "2^121" | bc)
b2_sum=$(./op_test -a $big_num $big_num | head -1)
match $big_num_sum $b2_sum "add4 test"


