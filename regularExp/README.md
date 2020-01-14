# 正则表达式
## 字符类
|字符|含义|
|--------|----------------------------------------------------|
|.       |匹配任意一个字符|
|[]      |匹配括号中的任意字符|
|-       |在括号内表示范围|
|^       |在括号内的开头，匹配除括号中的字符之外的任意一个字符|
|[[:xxx]]|grep工具预定义的一些命名字符类|
## 数量限定符
|字符|含义|
|-----|-----------|
|?    |0次或1次|
|+    |1次或多次|
|\*   |0次或多次|
|{n}  |n次|
|{,n} |至多n次|
|{n,} |至少n次|
|{m,n}|m-n次|
## 位置限定符
|字符|含义|
|^   |匹配行首|
|$   |匹配行末|
|\<  |匹配单词开头的|
|\>  |匹配单词结尾的|
|\b  |匹配单词开头或这结尾的|
|\B  |匹配单词在中间的|
## 特殊
|字符|含义|
|\   |普通转特殊，特殊转普通|
|()  |将单元扩起来使用数量限定符|
| |  |表示或的关系|
## 正则种类
1. Bash正则-基础正则(){}都需要\(\)\{\}
2. Extended-扩展这个则不需要
## example
```shell
egrep "^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$" regex
egrep "^([0-9]{1,3}\.){3}[0-9]{1,3}$" regex
```
## C语言中使用正则表达式
```c
#include <sys/types.h>
#include <regex.h>

int regcomp(regex_t *preg, const char *regex, int cflags);
int regexec(const regex_t *preg, const char *string, size_t nmatch,
            regmatch_t pmatch[], int eflags);
size_t regerror(int errcode, const regex_t *preg, char *errbuf,
                size_t errbuf_size);
void regfree(regex_t *preg);
```
```shell
./a.out "^([0-9]{1,3}\.){3}[0-9]{1,3}$" "192.168.0.1"
find ./ -maxdepth 1 -name "*.cpp" -print0 | xargs -0 ls -lh
grep -R "struct task_struct {" /usr/src/ -n
```
