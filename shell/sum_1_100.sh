#!/bin/bash

i=0
sum=0
while [ $i -le 100 ]; do
    i=$[i+1]
    sum=$[sum+i]
done

echo "sum:$sum"
