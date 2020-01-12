#!/bin/bash

i=1
sum=0
while [ $i -le 100 ]; do
    sum=$[sum+i]
    i=$[i+1]
done
echo "sum(1-100)=$sum"
