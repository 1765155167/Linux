#!/bin/bash

MYHOME=/home/hqf/Linux

echo "hello i'am mr.hu."

echo $PATH

la -al

echo $MYHOME

ls *.sh

ls ??????.sh

ls [a-z]?????.sh

echo "----------------------------"

VAR=$(date)
echo $VAR

VAR=`date`
echo $VAR

echo "-----------------------------"

NUM=76
echo $[$NUM+12]
echo $[NUM+12]
echo $(($NUM+12))
echo $((NUM+12))

echo "----------------------------"
echo $[2#1000100+8#777]

echo "----------------------------"
ls \
    -a

echo "----------------------------"
echo "\$[hello]"

# echo "----------------------------"
# touch ./-----xyz.md

printf "hello %d \n" $NUM

echo 'hello "xiaoming" $[NUM] '
echo "hello "xiaoming" $[NUM] "

est $NUM -eq 76
echo '$NUM -eq 76' $?

test $NUM -lt 76
echo '$NUM -lt 76' $?


echo "--------------------------------"
VAR1=hello
VAR2=hello
[ "$VAR1" = "$VAR2" ]
echo $?

echo "--------------------------------"
VAR1=15
VAR2=15
[ "$VAR1" -eq "$VAR2" ]
echo '-eq:'$?
[ "$VAR1" -lt "$VAR2" ]
echo '-lt:'$?
[ "$VAR1" -gt "$VAR2" ]
echo '-gt:'$?
[ "$VAR1" -le "$VAR2" ]
echo '-le:'$?
[ "$VAR1" -ge "$VAR2" ]
echo '-ge:'$?
