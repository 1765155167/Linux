#!/bin/bash

makedir()
{
    dirname=$1
    if [ ! -d "$dirname" ]; then
        mkdir $dirname
    else
        echo "$dirname have here."
    fi
}

for dir in $@; do
    makedir "$dir"
done
