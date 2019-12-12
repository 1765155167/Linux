#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
#include "hqf_log.h"

int main()
{
    int fd[2];
    //创建匿名管道 fd[0]:r fd[1]:w
    int ret = pipe(fd);
    check("pipe", ret);
    std::cout << "fd[0]:" << fd[0] 
        << " fd[1]:" << fd[1] << std::endl;
    int i = 0;
    for(; i < 2; i++)
    {
        pid_t pid = fork();
        check("fork", pid);
        if(pid == 0)
        {
            break;
        }
    }

    if(i == 0) {//第一个子进程
        close(fd[0]);//关闭读
        check("dup2", dup2(fd[1], STDOUT_FILENO));
        check("execlp", execlp("ps", "ps", "aux", NULL));
    } else if(i == 1) {//第二个子进程
        close(fd[1]);//关闭写
        check("dup2", dup2(fd[0], STDIN_FILENO));
        check("execlp", execlp("grep", "grep", "bash", "--color=auto", NULL));
    } else if(i == 2) {//父进程
        close(fd[0]);
        close(fd[1]);
        while(true) {
            ret = waitpid(-1, NULL, WNOHANG);
            if(ret == -1) {
                break;
            }
            if(ret == 0)
            {
                sleep(1);
                continue;
            }
            std::cout << "child pid:" << ret << std::endl;
        }
    }
    close(fd[0]);
    close(fd[1]);
    return 0;
}
