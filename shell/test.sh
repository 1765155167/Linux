#!/bin/bash

echo $0 #argv[0]
echo $1 #argv[1]
echo $2
echo $3
echo $4

echo '$#:'$# #argc - 1
echo '$@:'$@ #argv
echo '$$:'$$ #pid

echo "---------------------------"

num=$#
while [ $num -gt 0 ]; do
    echo $1
    shift
    num=$[num-1]
done
