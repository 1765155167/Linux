#include <iostream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define u64 unsigned long int
#define addr_t u64
//创建一把互斥锁
pthread_mutex_t mutex;

void* myfunc(void* arg)
{
    addr_t num = (addr_t)arg;
    pthread_mutex_lock(&mutex);//加锁
    std::cout << num << " child pthread id:" << pthread_self() << std::endl;
    pthread_mutex_unlock(&mutex);//解锁
    return NULL;
}

int main()
{
    //初始化锁
    pthread_mutex_init(&mutex, NULL);
    pthread_t pthid[4];
    for(int i = 0; i < 4; i++)
    {
        //第四个参数传递的是数值
        int ret = pthread_create(&pthid[i], NULL, myfunc, (void*)((addr_t)i));
        if(ret != 0)
        {
            pthread_mutex_lock(&mutex);//枷锁
            std::cout << "error num " << ret 
                << " error info " << strerror(ret) << std::endl;
            pthread_mutex_unlock(&mutex);//解锁
        }
    }

    pthread_mutex_lock(&mutex);//枷锁
    std::cout << "parent thread id:" << pthread_self() << std::endl;
    pthread_mutex_unlock(&mutex);//解锁
    //等待子线程退出并且回收
    pthread_join(pthid[0], NULL);
    pthread_join(pthid[1], NULL);
    pthread_join(pthid[2], NULL);
    pthread_join(pthid[3], NULL);
    //释放锁
    pthread_mutex_destroy(&mutex);
    return 0;
}

