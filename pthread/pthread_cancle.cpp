#include <iostream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define u64 unsigned long int
#define addr_t u64

void* myfunc(void* arg)
{
    addr_t num = (addr_t)arg;
    while(true)
    {
        int a = 0;
        pthread_testcancel();//设置一个取消点
        std::cout << "a = " << a << "arg " << num << std::endl;
    }
    return NULL;
}

int main()
{
    pthread_t pthid;
    //第四个参数传递的是地址
    //pthread_create(&pthid[i], NULL, myfunc, (void*)&i);
    //第四个参数传递的是数值
    int ret = pthread_create(&pthid, NULL, myfunc, (void*)(addr_t)5);
    if(ret != 0)
    {
        std::cout << "error num " << ret 
            << " error info " << strerror(ret) << std::endl;
    }
    std::cout << "parent thread id:" << pthread_self() << std::endl;
    pthread_cancel(pthid);//杀死子线程
    return 0;
}

