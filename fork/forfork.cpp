#include <iostream>
#include <unistd.h>
#include <vector>
#include "check.h"

int main()
{
    int i = 0;
    pid_t pid = 0;
    for(i = 0; i < 10; i++)
    {
        pid = fork();
        check("fork", pid);
        if(pid == 0)//如果是子进程退出避免子进程创建进程
        {
            break;
        }
    }
    //根据i值来判断第几个子进程和父进程
    switch(i)
    {
    case 0: 
        std::cout << "frist process pid:" << getpid() << std::endl;
        break;
    case 1:
        std::cout << "second process pid:" << getpid() << std::endl;
        break;
    case 2:
        std::cout << "thired process pid:" << getpid() << std::endl;
        break;
    case 3:
        std::cout << "fourth process pid:" << getpid() << std::endl;
        break;
    case 4:
        std::cout << "fifth process pid:" << getpid() << std::endl;
        break;
    case 5:
        std::cout << "sixth process pid:" << getpid() << std::endl;
        break;
    case 6:
        std::cout << "seventh process pid:" << getpid() << std::endl;
        break;
    case 7:
        std::cout << "eighth process pid:" << getpid() << std::endl;
        break;
    case 8:
        std::cout << "ninth process pid:" << getpid() << std::endl;
        break;
    case 9:
        std::cout << "tenth process pid:" << getpid() << std::endl;
        break;
    case 10:
        std::cout << "father process pid:" << getpid() << std::endl;
        break;
    }
    return 0;
}

