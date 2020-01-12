#!/bin/bash

isdir()
{
    dirname=$1
    if [ ! -d "$dirname" ]; then
        return 1
    else
        return 0
    fi
}

makedir()
{
    dirname=$1
    if  ! isdir "$dirname"; then
        mkdir $dirname
    else
        echo "$dirname have here."
    fi
}

for dir in $@; do
    makedir "$dir"
done
