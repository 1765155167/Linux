#awk
处理文件的列数据
默认按照空格或者tab进行差分
## 格式
awk 参数 '脚本语句(/patten/{action})' 待操作文件
awk 参数 -f '脚本文件' 待操作文件
# example
```shell
ps -aux | awk '{print $10}'
//-F: 更改分割符号为:
awk -F: '{print $7}' /etc/passwd 
awk '$2>75 {print $0} $2<=75 {print $0 " 需要进货";}' awk.txt
awk '$2>75 {print $0} $2<=75 {printf("%s %s\n",$0,"需要进货")}' awk.txt
awk '/^ *$/ {x=x+1;} END {print x;}' README.md #统计空行数
```





