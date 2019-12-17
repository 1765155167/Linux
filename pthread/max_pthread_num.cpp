#include <iostream>
#include <pthread.h>
//最大创建线程个数10337
void* hello(void* arg)
{
    long int num = (long int)arg;
    while(true);
    return NULL;
}

int main()
{
    int i = 0;
    while(true)
    {
        i++;
        pthread_t pthid;
        int ret = pthread_create(&pthid, NULL, hello, (void*)1);
        if(ret != 0)
        {
            std::cout << "pthread create err: " << i <<  std::endl;
            exit(-1);
        }
    }
    std::cout << "Hello world" << std::endl;
    return 0;
}

