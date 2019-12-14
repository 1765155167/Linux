# SIGNAL 信号
## 特点:
- 简单
- 携带信息量少
- 使用在某个特定的场景中
## 信号的状态
- 产生:
- 未决:信号没有被处理
- 递达:信号被处理了(忽略、捕捉、执行默认动作)
## 产生信号方式
1. 键盘:ctrl + c
2. 命令:kill
3. 系统函数:kill
4. 软条件:定时器
5. 硬件:段错误、除0错误
## 信号发送过程
- 进程A-->内核(产生信号)-->进程B
## 相关函数
1. int kill(pid_t pid, int signal);
- * pid
- * - pid > 0:指定进程
- * - pid = 0:同一进程组进程
- * - pid < -1:|pid|发送给对应的进程组
- * - pid = -1:取|pid|所有有权限的进程组
2. int raise(int signal); //自己给自己发信号
3. void abort(void);//向自己发送SIGABRT终止信号
4. alarm--单次定时器(每个进程只有一个定时器)
- * unsigned int alarm(unsigned int seconds);
- * seconds:定时的秒数，当seconds=0时取消定时器
- * 时间到时发送SIGALRM
- * 返回值:上一个定时器还有多久到达定时时间
5. setitimer 周期性定时器
```c++
int setitimer(int which,//定时法则 
const struct itimerval *new_value,
struct itimerval *old_value);
struct itimerval {
    struct timeval it_interval; /* Interval for periodic timer 
                                 定时器循环周期*/
    struct timeval it_value;    /* Time until next expiration
                                 第一次触发定时器的时间*/
};

struct timeval {
    time_t      tv_sec;         /* seconds秒 */
    suseconds_t tv_usec;        /* microseconds微秒 */
};
```
## time ./a.out 测试程序执行时间
## 信号集
### 概念
- 未决信号集:没有被当前进程处理的信号
- 堵塞信号集:将某个信号放到堵塞
## 两者关系
信号产生，信号处于未决状态，进程收到信号后，信号被放入味觉信号集
放入未决信号集的信号等待处理，在处理之前需要判断堵塞信号集中标志位
如果堵塞信号集对应标志位为1不处理，为0处理该信号
- 现在自定义信号集中设置标志位，然后通过一系列函数来完成自定义信号集合
设置到堵塞信号集
## 自定义信号集函数
- int sigemptyset(sigset_t \* set);//设置set为空
- int sigfillset(sigset_t \* set);//将所有信号加入信号集
- int sigaddset(sigset_t \* set, int signo);//将信号加入信号集
- int sigdelset(sigset_t \* set, int signo);//从信号集中删除信号
- int sigismember(const sigset_t \* set, int signo);//判断信号是否存在
- int sigprocmask(int how, const sigset_t  \* set, sigset_t \* oldset);//将自定义信号集设置到堵塞信号集中
- int sigpenging(sigset_t \* set);//读取未决信号集
[参考文件](./signal_set.cpp)
### 阻塞信号集，未决信号集
- 位置:内核PCB
- 不能直接操作
- 堵塞信号集
- * 要屏蔽的信号
- 未决信号集
- * 没有被处理的信号的集合
## 信号的捕捉
- typedef void(\* sighander_t)(int);
- sighander_t signal(int signum, sighander_t hanler);
```c++
int sigaction(int signum,//捕捉的信号
const struct sigaction *act,//捕捉方式
struct sigaction* oldact//之前的捕捉方式
);
struct sigaction{
    void (*sa_handler)(int);//回调函数
    void (*sa_sigaction)(int,siginfo_t*,void*);
    sigset_t sa_mask;//在处理函数执行过程中，临时屏蔽制定信号
    int sa_flags;//使用第一个回调函数，改值赋值为0
}
```
