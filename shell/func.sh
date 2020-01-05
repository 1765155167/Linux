#!/bin/bash

echo $sum
echo "----------------------------"
echo $0
num=$#
while [ $num -gt 0 ]; do
    echo $1
    shift
    num=$[num-1]
done
echo "----------------------------"

foo()
{
    sum=10
    echo $0
    num=$#
    while [ $num -gt 0 ]; do
        echo $1
        shift
        num=$[num-1]
    done
    echo "Hello foo."
}

foo aa bb cc dd ee ff .
