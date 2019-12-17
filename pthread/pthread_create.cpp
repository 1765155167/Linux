#include <iostream>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex;
void* pthread_function1(void* arg)
{
    while(true)
    {
        pthread_mutex_lock(&mutex);
        std::cout << "hello I'am fun one" << std::endl;
        pthread_mutex_unlock(&mutex);
        //sleep(1);
    }
    return NULL;
}

void* pthread_function2(void* arg)
{
    while(true)
    {
        pthread_mutex_lock(&mutex);
        std::cout << "hello I'am fun two" << std::endl;
        pthread_mutex_unlock(&mutex);
        //sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);//不分离
    //设置父子线程分离时不需要父线程回收子线程，join也没用，父线程死后子线程强制停止
    pthread_mutex_init(&mutex, NULL);
    pthread_t pthid1, pthid2;
    pthread_create(&pthid1, &attr, pthread_function1, NULL);
    pthread_create(&pthid2, &attr, pthread_function2, NULL);
    pthread_join(pthid1, NULL);
    pthread_join(pthid2, NULL);
    //注销
    pthread_mutex_destroy(&mutex);
    pthread_attr_destroy(&attr);
    return 0;
}

