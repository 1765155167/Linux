#include <iostream>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

void myfun(int no)
{
    std::cout << "hello World" << std::endl;
    std::cout << "捕捉到" << no << "信号" << std::endl;
    std::cout << "time" << time(NULL) << std::endl;
}

int main()
{
    //设置捕捉信号
    signal(SIGALRM, myfun);

    //设置定时器
    struct itimerval new_val;
    //第一次触发的时间 tv_sec + tv_usec
    new_val.it_value.tv_sec = 2;//s
    new_val.it_value.tv_usec = 0;//微秒
    //周期性定时
    new_val.it_interval.tv_sec = 1;
    new_val.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &new_val, NULL);

    while(1)
    {
        sleep(1);
    }
    return 0;
}

