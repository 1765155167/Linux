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
- * -1:回收失败，没有可回收的子进程
- * >0:被回收的子进程的pid
- * 0:非堵塞时代表子进程还处于运行状态
# 进程间通信IPC(InterProcess Communication)
## 进程间通信常用方式
1. 管道-简单，类似文件操作
2. 信号-系统开销小
3. 共享映射区-有无血缘关系进程间通信都可以
4. 本地套接字-稳定，类似网络通信套接字
## 管道
### 管道的概念
- 本质:
- * 内核缓冲区
- * 伪文件-不占用磁盘空间
- 特点:
- * 分为两部分:读端与写端，对应两个文件描述符
- * 写端流入，读端流出
- * 不需要释放管道空间，操作管道的进程被销毁后，管道自动释放
- * 管道默认是堵塞的
### 管道的原理
- 内部实现方式:队列
- * 环形队列，先进先出
- 缓冲区大小
- * 默认4K
- * 大小会适当的做调整但不会无限制的增加
### 管道的局限性
- 数据只能读一次因为使用队列方式实现的
- 采用半双工方式通信:不能同时读写
- 匿名管道:只能用于有血缘关系的进程间通信
### 管道的读写行为
- 读操作
- * 有数据:与read一样，返回读出的字节数
- * 无数据
- * - 写端全部关闭:read解除阻塞返回0
- * - 写端没有全部关闭:read会堵塞知道缓冲区有数据
- 写操作
- * 读端全部关闭
- * - 管道破裂，进程被终止:内核给当前进程发送SIGPIPE信号
- * 读端没有全部关闭
- * - 缓冲区写满了:waite 堵塞
- * - 缓冲区没有满:waite 正常写
### 如何设置非堵塞
* 默认读写两端都堵塞
* 设置读端为非堵塞
* - fcntl-变参函数
* - 1. 获取原来的flags: int flags = fcntl(fd[0], f_GETFL);
* - 2. 设置新的flags: fcntl(fd[0], F_SETFL, flags|O_NONBLOCK);
### 思考
- 单个进程能否使用管道完成读写:可以
- 父子进程间通信是否需要sleep:不需要，管道默认是堵塞的
### 查看缓冲区大小
- ulimit -a | grep pipe
- fpathconf()函数: man fpathconf
- long size = fpathconf(fd, _PC_PIPE_BUF);
### fifo 有名管道 
1. 特点
- 有名管道 类型p
- 伪文件，在磁盘大小永远为0
- 在内核中有一个对应的缓冲区
2. 使用场景
- 没有血缘关系的进程间通信
3. 创建方式
- shell命令: mkfifo 管道名
- 函数:mkfifo 
- * int mkfifo(const char \*pathname, mode_t mode);
- * - 权限:mode&(~umask)
4. 使用方式
- fifo文件可以使用IO函数进行操作
- open/close
- read/write
- 不能执行lseek操作
5. 进程间通信
- 有名管道文件可以想普通文件那样进行读写，区别是管道堵塞
- a.cpp :读
- * int fd = open("myfifo", O_RDONLY);
- * read(fd, buf, sizeof(buf));
- * close(fd);
- b.cpp :写
- * int fd = open("myfifo", O_WRONLY);
- * write(fd, buf, sizeof(buf));
- * close(fd);
## 内存间映射区
### mmap 创建内存映射区
```c
#include <sys/mman.h>
void* mmap(
    void* addr,//首地址 为NULL时系统自动分配地址
    size_t length,//大小,不能为0,一般为文件的大小
    int prot, //权限 PROT_EXEC PORT_READ(读，必须设置) PORT_WRITE(写) PORT_NONE
    int flags,//标志位参数 MAP_SHARED(共享的，数据会同步到磁盘) MAP_PRIVATE(私有的)
    int fd, //文件描述符 要映射的文件(open)
    off_t offset,//映射文件偏移量(必须是4k的整数倍),一般为0
    );
```
- 返回值:
- * 成功:映射区的首地址
- * 失败:MAP_FAILED:(void\* -1)
### munmap 释放内存映射区
```c
#include <sys/mman.h>
int mumap(void* addr, size_t length);
```
- 参考mmap.cpp代码
# 判断文件是否存在
- access("fifo", F_OK);
