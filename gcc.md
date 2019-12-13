# GCC工作流程
1. 预处理 gcc -E
2. 编译 gcc -S
3. 汇编 gcc -c
4. 链接 没参数
# GCC参数
1. -o：指定生成的文件的名字
2. -D：在编译的时候定义宏（控制log输出）
3. -I：指定头文件路径
4. -g：生成gdb调试程序
5. -On：编译优化 -O1 -O2 -O3
6. Wall：编译期间输出更多警告信息
# 制作静态库
1. 命名规则：lib+库名+.a
2. 制作步骤：
- 1. 生成.o：gcc -c \*.c
- 2. 打包：ar rcs 静态库名字 \*.o
3. 使用
- 1. gcc mian.c 库 -L(libPath) -o app -Iinclude
# 制作动态库
1. 命名规则：lib+库名+.so
2. 制作步骤
- 1. 生成与位置无关代码：gcc -fPIC -c \*.c -Iinclude
- 2. 将.o生成.so文件：gcc -shared -o 库名字 \*.o 
3. 使用：
- 1. gcc main.c -Llib -l(库名) -Iinclude -o app
- 2. 生成的应用程序不能执行，动态链接不到自己的库
- * 1. 临时设置库路径:export LD_LIBRARY_PATH=/home/hqf/lib
- * 2. 永久设置方法：
- * - 1. 在/etc/ld.so.conf文件中添加库目录
- * - 2. 更新：sudo ldconfg -v
# gdb调试
- start 开始执行程序 只执行一部
- n (next)下一步 
- s (step) 可以进入函数内部
- c (continue) 直接停在断点位置
- l (list)查看代码
- l main.cpp:main
- b 10 :在第10行设置断点
- b 10 if i==5 断点条件
- d 10 删除断点
- i b查看断点信息
- finish：从函数体内部跳出
- p：查看变量值
- ptype：查看变量类型
- set var 变脸名=值:设置变量的值
- display：设置追踪变量
- undispaly 编号：取消追踪变量
- info display：获取编号
- quit：退出
