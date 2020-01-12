# sed
sed处理文件内容的行数据
## vim 末行模式
%s/echo/printf/g echo替换成printf /g表示替换多次
## sed
### 格式
sed 参数 '脚本语句' 带操作文件
sed 参数 -f '脚本文件' 待操作文件
#### 参数
1. a 追加
2. i 插入
3. d 删除
4. s 替换
5. -i 更改到文件 **慎用**
6. & 匹配到字符串本身
7. /n 匹配到字符串的第n个字符
```shell
sed '/echo/s/echo/printf/g' case.sh
sed 's/printf/echo/g' case.sh #替换
sed '4a 追加到第4行' case.sh
sed '5d' case.sh   #删除第5行
sed '2,7d' case.sh #删除2到7行
sed -i '2,7d' case.sh #删除2到7行 结果返回给文件
sed -n '/echo/p' case.sh #打印匹配到echo的行
sed '/echo/d' case.sh #删除匹配到echo的行
sed 's/echo/--&--/g' case.sh #将echo前加上-- 后加上--
sed 's/([0-9][0-9])/--\1--~~/2~~/g' case.sh
sed 's/<[^<>]*>//g' t.html #[^<>]除了<>以外的元素
sed 's/\(\(\([0-9]\)\{1,3\}\.\)\{3\}\([0-9]\)\{1,3\}\)/ip:&/g' case.sh #匹配到的ip前面加上ip:
sed -r 's/((([0-9]){1,3}\.){3}([0-9]){1,3})/ip:&/g' case.sh #使用扩展正则表达式
```
