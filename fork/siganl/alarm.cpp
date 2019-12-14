#include <iostream>
#include <signal.h>
#include <unistd.h>
#include "hqf_log.h"

bool flags = true;

void myfun(int no)
{
    std::cout << "捕捉到" << no << "信号!" << std::endl;
    flags = false;
}

int main()
{
    signal(SIGALRM, myfun);//捕捉信号
    int ret = alarm(5);
    std::cout << "ret:" << ret << std::endl;
    sleep(2);
    ret = alarm(6);
    std::cout << "ret:" << ret << std::endl;
    while(flags)
    {
        std::cout << "Hello I'am mr.hu" << std::endl;
        sleep(1);
    }
    return 0;
}

