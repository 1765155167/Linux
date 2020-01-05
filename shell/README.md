# shell
## shell running ways
- . sample.sh
- chmod u+x sample.sh && ./sample.sh
## wildcard
- ? one anything char
- * zero to more char
- [0-9] one num
- [a-z] one low char
### if 
- -eq =
- -lt <
- -le <=
- -gt >
- -ge >=
- -d  is dir
- -s  is file
- -p  is pipe
- -z  len = 0
- -n  not is null
- =   [ var1 = var2 ] if equal
- !=  [ var1 != var2 ] not equal
- !   [ ! EXPR ] 
- -a  [ EXPR1 -a EXPR2 ] and &&
- -o  [ EXPR1 -o EXPR2 ] or  ||
```shell
hqf@hqf-machine:~/Linux/shell$ test -d README.md
hqf@hqf-machine:~/Linux/shell$ echo $?
1
hqf@hqf-machine:~/Linux/shell$ test -s README.md
hqf@hqf-machine:~/Linux/shell$ echo $?
0
hqf@hqf-machine:~/Linux/shell$ [ -s README.md ]
hqf@hqf-machine:~/Linux/shell$ echo $?
0
```
### echo $?
- true:0
- false:1
## if
```shell
if [  ]; then
    echo "hello"
elif [  ]; then
    echo "..."
else
    echo "else"
fi
```
## while
```shell
while [ filename in $(ls) ]; do
    echo "$filename"
done
```
## case
```shell
case $VAL in
YES|y)
    echo "yes"
NO|n)
    echo "No"
*)
    echo "default"
esac
```
## chong ding xiang
- > 
- >> 
- cmd > file 2>&1 
- * 2:STDERR_NO 
- * 1:STDOUT_NO
- * ls dasdsadasd > ls.txt 2>&1
- cmd < file1 > file2
- * cat < README.md > README.md~
## debug
- sh -x mkdir_more.sh aa bb cc dir
