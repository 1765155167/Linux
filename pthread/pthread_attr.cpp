#include <iostream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define u64 unsigned long int
#define addr_t u64

void* myfunc(void* arg)
{
    addr_t num = (addr_t)arg;
    std::cout << "num" << num << std::endl;
    int i = 0;
    while(true)
    {
        if(i > 10)
        {
            pthread_exit(NULL);
        }
        std::cout << "Hello World!" << std::endl;
        sleep(1);
        i++;
    }
    return NULL;
}

int main()
{
    pthread_t pthid;
    //创建线程时设置线程分离
    //初始化线程属性
    pthread_attr_t attr;//线程属性结构体
    pthread_attr_init(&attr);//初始化
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//线程分离
    int ret = pthread_create(&pthid, &attr, myfunc, (void*)(addr_t)5);
    if(ret != 0)
    {
        std::cout << "error num " << ret 
            << " error info " << strerror(ret) << std::endl;
    }
    pthread_attr_destroy(&attr);//释放线程属性与init对应
    while(1){}
    return 0;
}

