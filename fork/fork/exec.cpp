#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include "check.h"

int main()
{
    int ret = 0;
    for(int i = 0; i < 8; i++)
    {
        std::cout << "parent i = " << i << std::endl;
    }
    pid_t pid = fork();
    check("fork", pid);
    if(pid == 0)
    {
        //让子进程执行ls命令 ls使用子进程地址空间
        //ret = execl("/bin/ls" , "ls", "-l", "-a", "-h", "-i", NULL);
        ret = execlp("ls" , "ls", "-l", "-a", "-h", "-i", NULL);
        //只有exec函数执行出错才会执行到这，其实不需要判断返回值
        check("execlp", ret);
    }
    std::cout << "-----------------------------" << std::endl;
    for(int i = 0; i < 8; i++)
    {
        std::cout << "parent i = " << i << std::endl;
    }
    sleep(1);
    return 0;
}

