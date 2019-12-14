#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include "hqf_log.h"

void myfun(int no)
{
    std::cout << "捕捉到" << no << std::endl;
}

int main()
{
    //捕捉信号
    //注册捕捉函数
    signal(SIGINT, myfun);
    pid_t pid = fork();
    check("fork", pid);
    if(pid > 0)
    {
        //父进程回收子进程资源
        int status = 0;
        pid_t ret = wait(&status);
        if(WIFSIGNALED(status))
        {
            std::cout << "child pid " << ret 
                << " signal:" << WTERMSIG(status) 
                << std::endl;
        }
    } else {
        //向自己发送某信号
        raise(SIGINT);
        //向自己发送SIGABRT信号
        //abort();
    }
    return 0;
}
