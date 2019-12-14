## fork
读时共享写时复制
父子进程变量地址(虚拟地址)是一样的
但在物理内存上不是同一块
## 常用函数
- getpid();
- getppid();
- execl("/home/hqf/a.out", "666", "arg", NULL);
- execlp("ls", "666", "arg", NULL);
## wait-堵塞函数
- pid_t wait(int\* status);
- 返回值
- * >0:回收子进程的pid
- * -1:没有子进程可以回收
- status
- * 获取退出时候的返回值
- * WIFEXITED(status) > 0 :正常退出
- * WIFSIGNALED(status) > 0 :返回信号编号
## waitpid 
- pid_t waitpid(pid_t pid, int\* status, int options);
- options:可选择堵塞或者非堵塞
- * options=0:阻塞函数
- * options=WNOHANG:非堵塞
- pid:可选择回收那个子进程
- * pid>0:某个子进程的pid
- * pid=-1:回收所有的子进程
- * pid=0:回收当前进程组的子进程
- * pid<0:pid(加减号)，用于释放不是同一个进程组的子进程
- 返回值
- * ret=-1:回收失败，没有可回收的子进程
- * ret>0:被回收的子进程的pid
- * ret=0:非堵塞时代表子进程还处于运行状态
