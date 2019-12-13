#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include "check.h"

int main()
{
    pid_t pid = fork();
    check("fork", pid);
    if(pid == 0)
    {
        std::cout << "I'am Child Process." << std::endl;
        std::cout << "ID:" << getpid() << " Father ID:" << getppid() << std::endl;
        sleep(20);
    }else 
    {

        std::cout << "I'am Father Process." << std::endl;
        std::cout << "ID:" << getpid() << " Father ID:" << getppid() << std::endl;
        int status = 0;
        //pid_t wpid = wait(NULL);//回收子进程
        pid_t wpid = wait(&status);//回收子进程
        std::cout << "wpid: " << wpid << std::endl;
        //判断子进程是否正常退出　返回子进程返回值
        if(WIFEXITED(status) != 0)
        {
            std::cout << "子进程正常退出" << std::endl;
        }
        //判断子进程是不是通过信号杀死
        if(WIFSIGNALED(status) != 0)
        {
            //获取信号编号
            int signal = WTERMSIG(status);
            std::cout << signal << "信号使子进程退出" << std::endl; 
        }
    }
    return 0;
}

