
#!/bin/bash

source ./func.sh

run_mul_test() {
    f_ret=$(./op_test -m $1 $2 | head -1)
}

run_bc() {
    bc_val=$(echo $1 | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
}

run_mul_test 32 32
match $f_ret 1024 "mul1 test"

run_mul_test 31 31
match $f_ret 961 "mul2 test"

run_bc 2^100
a=$bc_val
run_bc 2^50
b=$bc_val
run_bc 2^150
c=$bc_val
run_mul_test $a $b
match $f_ret $c "mul3 test"

run_mul_test 1234 0
match $f_ret 0 "mul4 test"

run_mul_test 0 1234
match $f_ret 0 "mul5 test"

run_mul_test 1 1234
match $f_ret 1234 "mul6 test"

run_mul_test 1234 1
match $f_ret 1234 "mul7 test"

run_mul_test 1234 -1
match $f_ret -1234 "mul8 test"

run_mul_test -1234 1
match $f_ret -1234 "mul9 test"

run_mul_test -1234 -1
match $f_ret 1234 "mul11 test"
