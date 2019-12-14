#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include "hqf_log.h"

int main()
{
    pid_t pid = fork();
    check("fork", pid);
    if(pid > 0)
    {
        while(1)
        {
            std::cout << "I'am you father." << std::endl;
            sleep(1);
        }
    } else {
        sleep(5);
        int ret = kill(getppid(), SIGKILL);
        if(ret == -1)
        {
            std::cout << "信号没有发送出去" << std::endl;
        } else {
            std::cout << "I kill my father." << std::endl;
        }
    }
    return 0;
}
