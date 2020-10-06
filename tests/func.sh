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
