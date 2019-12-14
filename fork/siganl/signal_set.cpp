#include <iostream>
#include <signal.h>
#include <unistd.h>

int main()
{
    //手动屏蔽信号
    sigset_t myset;//自定义信号集合
    sigemptyset(&myset);//清空信号集
    sigaddset(&myset, SIGINT);//添加SIGINT(ctrl + c)信号到信号集中
    sigaddset(&myset, SIGQUIT);//[ctrl + \]
    sigaddset(&myset, SIGKILL);//
    //将自定义的信号集设置给堵塞信号集
    sigprocmask(SIG_BLOCK, &myset, NULL);
    //每隔一秒钟读一次内存的未决信号
    while(1)
    {
        sigset_t sigset;
        sigpending(&sigset);//读取未决信号集
        for(int i = 1; i < 32; i++)
        {
            //判断i信号是否在未决信号集中
            if(sigismember(&sigset, i))
            {
                std::cout << 1;
            }else {
                std::cout << 0;
            }
        }
        std::cout << std::endl;
        sleep(1);
    }
    return 0;
}

