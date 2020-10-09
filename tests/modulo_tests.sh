
#!/bin/bash

source ./func.sh

run_mod_test() {
    f_ret=$(./op_test -r $1 $2 | head -1)
}

run_mod_test 32 31
match $f_ret 1 "mod1 test"

run_mod_test 1024 32
match $f_ret 0 "mod2 test"

run_mod_test 12345678900000000000000000 123456789
match $f_ret 0 "mod3 test"

