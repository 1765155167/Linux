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
- * - 缓冲区写满了:write 堵塞
- * - 缓冲区没有满:write 正常写
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
- * 有名管道 类型p
- * 伪文件，在磁盘大小永远为0
- * 在内核中有一个对应的缓冲区
2. 使用场景
- * 没有血缘关系的进程间通信
3. 创建方式
- * shell命令: mkfifo 管道名
- * 函数:mkfifo 
- * - int mkfifo(const char \*pathname, mode_t mode);
- * - 权限:mode&(~umask)
4. 使用方式
- * fifo文件可以使用IO函数进行操作
- * open/close
- * read/write
- * 不能执行lseek操作
5. 进程间通信
- * 有名管道文件可以想普通文件那样进行读写，区别是管道堵塞
- * a.cpp :读
- * - int fd = open("myfifo", O_RDONLY);
- * - read(fd, buf, sizeof(buf));
- * - close(fd);
- * b.cpp :写
- * - int fd = open("myfifo", O_WRONLY);
- * - write(fd, buf, sizeof(buf));
- * - close(fd);
