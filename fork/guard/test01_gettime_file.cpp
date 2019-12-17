#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "hqf_log.h"


int fd = 0;
void myfunc(int no)
{
    if(no == SIGALRM) {
        //int fd = open("./time", O_WRONLY | O_APPEND | O_CREAT, 0644);
        //check("open", fd);
        time_t systime = time(NULL);
        char * timebuf = ctime(&systime);
        check("lseek", lseek(fd, SEEK_END, 0));
        check("write", write(fd, timebuf, strlen(timebuf) + 1));
        //close(fd);
    }
}

int main()
{
    pid_t pid = fork();
    check("fork", pid);
    if(pid > 0)
    {
        exit(1);
       //kill(getpid(), SIGINT); 
    } else if(pid == 0) {
        //创建新会话
        setsid();//变成会长，退出了控制终端，守护进程
        //改变工作目录
        chdir("/home/hqf");
        //重设文件掩码
        check("umask", umask(0));
        //关闭文件描述符
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        //核心程序
        //捕捉信号
        //signal(SIGALRM, myfunc);
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myfunc;
        sigemptyset(&act.sa_mask);
        sigaction(SIGALRM, &act, NULL);
        //创建定时器
        struct itimerval new_val;
        //第一次触发定时器时间
        new_val.it_value.tv_sec = 2;
        new_val.it_value.tv_usec = 0;
        //定时器周期
        new_val.it_interval.tv_sec = 2;
        new_val.it_interval.tv_usec = 0;
        setitimer(ITIMER_REAL, &new_val, NULL);
        if(access("./time", F_OK) == 0)
        {
            //文件存在
            fd = open("./time", O_RDWR | O_APPEND);
        } else {
            fd = open("./time", O_RDWR | O_APPEND | O_CREAT, 0644);
        }
        check("open", fd);
        //保证程序一直处于运行状态
        while(true){}
        close(fd);
    }
    return 0;
}

