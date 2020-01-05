#!/bin/bash

for filename in $(ls); do
    if [ -f "$filename" ]; then
        echo "$filename is file"
    elif [ -p "$filename" ]; then
        echo "$filename is fifo"
    else
        echo "$filename unknow file type"
    fi
done
