#include <iostream>
#include <unistd.h>
#include "hqf_log.h"
int main()
{
    //创建一个会话
    //将子进程变成会长
    pid_t pid = fork();
    check("fork", pid);
    if(pid > 0)
    {
        exit(1);
#if 0
        kill(getpid(), SIGINT);
#endif
    } else {
        //变成会长
        //会长就是一个守护进程
        setsid();
        while(true){}//让子进程一直或者
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

