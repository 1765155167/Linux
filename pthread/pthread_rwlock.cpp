#include <iostream>
#include <pthread.h>
#include <unistd.h>

int number = 0;
pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

void* writeFunction(void* arg)
{
    while(true)
    {
        //加写锁
        pthread_rwlock_wrlock(&rwlock);
        number++;
        pthread_mutex_lock(&mutex);
        std::cout << "write pthid:" << pthread_self()
            << "number: " << number <<  std::endl;
        pthread_mutex_unlock(&mutex);
        //解锁
        pthread_rwlock_unlock(&rwlock);
//        usleep(50);
    }
    return NULL;
}

void* readFunction(void* arg)
{
    while(true)
    {
        pthread_rwlock_rdlock(&rwlock);
        pthread_mutex_lock(&mutex);
        std::cout << "read pthid:" << pthread_self()
            << "number: " << number <<  std::endl;
        pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);
//        usleep(500);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_rwlock_init(&rwlock, NULL);
    pthread_t pthid[8];
    //创建三个写线程
    for(int i = 0; i < 3; ++i)
    {
        pthread_create(&pthid[i], NULL, writeFunction,NULL);
    }
    //创建5个读线程
    for(int i = 0; i < 3; ++i)
    {
        pthread_create(&pthid[i], NULL, readFunction,NULL);
    }
    //回收子线程资源
    for(int i = 0; i < 8; i++)
    {
        pthread_join(pthid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_rwlock_destroy(&rwlock);
    return 0;
}

