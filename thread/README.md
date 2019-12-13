# 多线程
- 头文件 thread,future 
- process(进程) linux:fork() windows:CreateProcess, signal) thread(POSIX threads)
- 多线程：mutilthread goto
- 面临问题：
- - 1.死锁
- - 2.乱序
- - 3.并发访问数据造成的问题
- - 4.低效率
# C++11给我们带来的新概念
- 1.高阶接口：(async, future)
- 2.低阶接口：(thread, mutex)
