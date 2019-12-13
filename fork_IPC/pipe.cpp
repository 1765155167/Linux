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
    pid_t pid = fork();
    check("fork", pid);
    if(pid == 0)
    {
        close(fd[1]);//防止子进程写
        char* buf = new char[128];
        ret = read(fd[0], buf, 23);
        check("read", ret);
        std::cout << "read:" << buf;
        ret = read(fd[0], buf, 13);
        check("read", ret);
        std::cout << "read:" << buf;
    } else {
        close(fd[0]);//防止父进程读
        std::string  buf = "Hello I'am you father.\n";
        ret = write(fd[1], buf.c_str(), buf.length());
        check("write", ret);
        ret = write(fd[1], buf.c_str(), buf.length());
        check("write", ret);
        while(true)
        {
            int status = 0;
            ret = waitpid(pid, &status, WNOHANG);
            if(ret == -1){
                break;
            }
            if(ret > 0) {
                std::cout << "child id:" << ret << std::endl;
            }
        }
    }

    close(fd[0]);
    close(fd[1]);
    return 0;
}
