#!/bin/bash

COUNT=3

printf "Please input passwd:"
read TRY

while [ "$TRY" != "hqf666123" -a $COUNT -gt 0 ]; do
    printf "passwd err Please Enter Again:"
    read TRY
    if [ "$TRY" = "break" ]; then
        break 1
    fi
    COUNT=$[COUNT-1]
done

if [ $COUNT -eq 0 ]; then
    echo "Sorry Passwd error."
else
    echo "Wecome."
fi

