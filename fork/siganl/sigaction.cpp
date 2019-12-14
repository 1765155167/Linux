#include <iostream>
#include <signal.h>
#include <unistd.h>

void myfunc(int no)
{
    std::cout << "signal" << no << std::endl;
    sleep(3);
    std::cout << "wake up" << std::endl;
}

int main()
{
    struct sigaction act;
    act.sa_flags = 0;
    //设置需要临时屏蔽信号
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGQUIT);
    //回调函数
    act.sa_handler =myfunc;
    //设置信号捕捉
    sigaction(SIGINT, &act, NULL);
    while(true){}
    return 0;
}

