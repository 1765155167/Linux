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
    long size = fpathconf(fd[0], _PC_PIPE_BUF);
    std::cout << "pipe size:" << size << std::endl;
    close(fd[0]);
    close(fd[1]);
    return 0;
}
