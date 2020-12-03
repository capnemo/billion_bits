#!/bin/bash

fail_count=0
total=0
for j in -b2 -b63
do
    for i in -a -s -m -d -r
    do
        source op_tests.sh $j $i
    done
done
echo tests failed:$fail_count
echo tests run:$total
