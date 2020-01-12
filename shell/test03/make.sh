#!/bin/bash

for file in $(ls); do
    if [ -f "$file" ]; then
        gcc -c "$file"
    fi
done
