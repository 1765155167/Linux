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
    size_t stacksize;
    void* stackaddr;
    int detachstate;
    //创建线程时设置线程分离
    //初始化线程属性
    pthread_attr_t attr;//线程属性结构体
    pthread_attr_init(&attr);//初始化
    
    //设置线程栈起始地址与大小
    stackaddr = malloc(0x10000);
    stacksize = 0x10000;
    pthread_attr_setstack(&attr, stackaddr, stacksize);
    pthread_attr_getstack(&attr, &stackaddr, &stacksize);//获取栈地址与容量
    std::cout << "stackaddr:" << stackaddr << " stacksize:" << stacksize << std::endl;
    
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);//设置线程分离
    pthread_attr_getdetachstate(&attr, &detachstate);//获取线程分离属性
    if (detachstate == PTHREAD_CREATE_DETACHED)
        std::cout << "thread detached" << std::endl;
    else if (detachstate == PTHREAD_CREATE_JOINABLE)
        std::cout << "thread join" << std::endl;
    else
        std::cout << "thread un known" << std::endl;

    int ret = pthread_create(&pthid, &attr, myfunc, (void*)(addr_t)5);
    if(ret != 0)
    {
        std::cout << "error num " << ret 
            << " error info " << strerror(ret) << std::endl;
    }
    std::cout << "线程创建成功" << std::endl;
    pthread_join(pthid, NULL);
    pthread_attr_destroy(&attr);//释放线程属性与init对应
    return 0;
}

