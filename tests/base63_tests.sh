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
run_bc "2^1001"
match $bc_val $b2_num "large number add test"

a=16383
b=31
run_bc "$a + $b"
b63_sum=$(./b63_test -a $a $b)
match $bc_val $b63_sum "basic add test 1"

b63_sum=$(./b63_test -a $b $a)
match $bc_val $b63_sum "basic add test 2"

run_bc "2^101"
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
run_bc "$arg*2"
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

run_sub_test() {
    run_bc "$1 - $2"
    b63_num=$(./b63_test -s $1 $2)
    match $bc_val $b63_num "$3"
}

sub_test_count() {
    run_bc "$1 - $2"
    b63_num=$(./b63_test -s $1 $2)
    if [ $bc_val != $b63_num ]; then
        echo "series sub test $3 failed"
        let "f_count=$f_count+1"
    fi
}

a=123456789
b=6789
run_sub_test $a $b "sub test 1a"
run_sub_test $b $a "sub test 1b"
run_sub_test $a $a "sub test 1c"
run_sub_test $b $b "sub test 1d"

run_bc "2^300 + 3^200 + 5^100"
a=$bc_val
run_bc "2^123"
b=$bc_val
run_sub_test $a $b "large number sub test a"
run_sub_test $b $a "large number sub test b"
run_sub_test $a $a "zero test a"
run_sub_test $b $b "zero test b"

i=1
run_bc "7^156"
x=$bc_val
run_bc "11^23"
b=$bc_val
f_count=0
while [ $i -le 70 ]
do
    run_bc "$x + 3^$i"
    a=$bc_val
    sub_test_count $a $b "sub test $i.a"
    echo -en "\b|"
    sub_test_count $b $a "sub test $i.b"
    echo -en "\b/"
    sub_test_count $a $a "sub test $i.c"
    echo -en "\b-"
    sub_test_count $b $b "sub test $i.d"
    echo -en "\b\\"
    let "i=$i+1"
done
echo -en "\b"
if [ $f_count -gt 0 ]; then
    echo "$f_count sub series tests have failed"
else
    echo "all sub series tests have passed"
fi

run_bc "2^360 + 3^234 + 5^67"
a=$bc_val
run_bc "2^100 + 3^67 + 7^9"
b=$bc_val

run_bc "$a/$b"
b63_num=$(./b63_test -d $a $b)
match $bc_val $b63_num "large number division test"

run_bc "$b/$a"
b63_num=$(./b63_test -d $b $a)
match $bc_val $b63_num "reverse large number division test"

c=0
run_bc "$c/$a"
b63_num=$(./b63_test -d $c $a)
match $bc_val $b63_num "zero division test"

d=1
run_bc "$a/$d"
b63_num=$(./b63_test -d $a $d)
match $bc_val $b63_num "unity division test"

i=40
f=0
while [ $i -le 70 ]
do
    run_bc "$b*2^i"
    x=$bc_val
    run_bc "$a/$x"
    b63_num=$(./b63_test -d $a $x)
    if [ $bc_val != $b63_num ]; then
        echo "divide shift test $i failed"
        let "f=$f+1"
    fi
    let "i=$i+1"
    echo -n "."
done
echo ""
check_fail $f "divide shift"

run_mod_test() {
    run_bc $1%$2
    b63_num=$(./b63_test -r $1 $2)
    match $bc_val $b63_num $3
}

run_bc "2^1234 + 2^123 + 2^65"
a=$bc_val
b=32
run_mod_test $a $b "mod_test"
run_mod_test $b $a "rev_mod_test"

run_bc "7^234 + 9^123"
a=$bc_val
run_bc "13^23 + 17^12"
b=$bc_val
run_mod_test $a $b "large_mod_test"

run_sign_test() {
    b63_val=$(./b63_test $1 $3 $4)
    run_bc "$3 $2 $4"
    if [ $bc_val != $b63_val ]; then
        let "f=f+1"
        echo "$5 failed"
    else
        let "tot=$tot+1"
    fi
}

bc_multiply() {
    bc_val=$(echo "$1*$2" | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
}

bc_divide() {
    bc_val=$(echo "$1/$2" | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
}

multiply_sign_test() {
    b63_val=$(./b63_test -m $1 $2)
    bc_multiply $1 $2
    if [ $bc_val != $b63_val ]; then
        let "f=f+1"
        echo "$3 failed"
    else
        let "tot=$tot+1"
    fi
}

divide_sign_test() {
    b63_val=$(./b63_test -d $1 $2)
    bc_divide $1 $2
    if [ $bc_val != $b63_val ]; then
        let "f=f+1"
        echo "$3 failed"
    else
        let "tot=$tot+1"
    fi
}

run_bc "2^34 + 3^56 + 7^78"
a=$bc_val

run_bc "5^12 + 6^45"
b=$bc_val
neg_a=-$a
neg_b=-$b

tot=0
f=0
run_sign_test -a + $a $b "add_sign_test_1"
run_sign_test -a + $b $a "add_sign_test_2"
run_sign_test -a + $a $neg_b "add_sign_test_3"
run_sign_test -a + $neg_a $b "add_sign_test_4"
run_sign_test -a + $neg_a $neg_b "add_sign_test_5"

run_sign_test -s - $a $b "subtract_sign_test_1"
run_sign_test -s - $b $a "subtract_sign_test_2"
run_sign_test -s - $b $b "subtract_sign_test_3"
run_sign_test -s - $a $neg_b "subtract_sign_test_4"
run_sign_test -s - $neg_a $b "subtract_sign_test_5"
run_sign_test -s - $neg_a $neg_b "subtract_sign_test_6"
run_sign_test -s - $neg_a $neg_a "subtract_sign_test_7"

multiply_sign_test $a $b "multiply_test_1"
multiply_sign_test $neg_a $b "multiply_test_2"
multiply_sign_test $a $neg_b "multiply_test_3"
multiply_sign_test $neg_a $neg_b "multiply_test_4"

run_bc 31^33
a=$bc_val
run_bc 31^20
b=$bc_val
neg_a=-$a
neg_b=-$b
divide_sign_test $a $b "divide_test_1"
divide_sign_test $neg_a $b "divide_test_2"
divide_sign_test $a $neg_b "divide_test_3"
divide_sign_test $neg_a $neg_b "divide_test_4"
echo "$tot sign tests passed"
echo "$f sign tests failed"


