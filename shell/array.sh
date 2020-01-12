#!/bin/bash

:<<EOF
注释内容...
注释内容...
注释内容...
EOF

# :<<A
array=(1 2 3 4 5 6 7)
echo "${array[@]}"
echo "length=${#array[@]}"
# A

hqf=" Mr.hu"
readonly hqf
str1='hello'${hqf}'.'
str2="hello${hqf}."
echo -e "${str1} ${str2}"
# hqf="${hqf}."
# unset hqf
