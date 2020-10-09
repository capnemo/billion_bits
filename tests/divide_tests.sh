
#!/bin/bash

source ./func.sh

run_div_test() {
    f_ret=$(./op_test -d $1 $2 | head -1)
}

run_div_test 100 10
match $f_ret 10 "div1 test"

run_div_test 123456789123456789 123456789123456789
match $f_ret 1 "div2 test"

run_div_test 0 1234
match $f_ret 0 "div3 test"

run_bc 2^200
a=$bc_val
run_bc 2^199
b=$bc_val
run_div_test $a 2
match $f_ret $b "div4 test"

run_div_test 1 1
match $f_ret 1 "div5 test"

run_div_test 12340 12345
match $f_ret 0 "div6 test"

run_div_test -223344 -223344
match $f_ret 1 "div7 test"

run_div_test 223344 -223344
match $f_ret -1 "div8 test"

run_div_test -223344 223344
match $f_ret -1 "div9 test"
