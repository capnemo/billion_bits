#!/bin/bash

check_args() {

    if [ $# == 1 ]; then
        b_arg=$1
    else
        b_arg=-b2
    fi
    echo "base is $b_arg"
}

match (){
    
    if [ $1 == $2 ]; then
        echo "$3 has passed"
    else
        echo "$3 has failed"
        let "mf=$mf+1"
    fi
}

check_fail() {

    if [ $1 -gt 0 ]; then
        echo "Some $2 test(s) failed" 
    else
        echo "all $2 tests have passed"
    fi

}

run_bc() {
    bc_val=$(echo $1 | bc | tr -d "\n" | tr -d "\\" 2>/dev/null)
}


