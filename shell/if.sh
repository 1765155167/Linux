#!/bin/bash

FILE=test.txt

if [ -f "$FILE" ]; then
    echo "$FILE is file"
elif [ -p "$FILE" ]
then
    echo "$FILE is fifo"
else
    echo "unknown"
#    exit 1 # exit terminal
fi
if : 
then
    printf "every thing is true\n"
fi
#exit 0
