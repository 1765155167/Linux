#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

struct node {
    int data;
    struct node* next;
};
struct node* Head = NULL;
pthread_mutex_t mutex_out;//cout 互斥锁
pthread_mutex_t mutex_node;//Head 互斥锁
sem_t sem;//信号量
//生产者模型
void* producer(void* arg)
{
    while(true)
    {
        //新建节点
        struct node* p = new struct node;
        if(p == NULL)
        {
            std::cout << "new struct node err" << std::endl;
            exit(1);
        }
        p->data = rand()%100;
        //节点插入链表 头插法
        pthread_mutex_lock(&mutex_node);
        p->next = Head;
        Head = p;
        pthread_mutex_unlock(&mutex_node);
        pthread_mutex_lock(&mutex_out);
        std::cout << "producer:" << pthread_self() << " data:" << p->data << std::endl;
        pthread_mutex_unlock(&mutex_out);
        sem_post(&sem);//sem++
        sleep(rand()%5);
    }
    return NULL;
}

//消费者模型
void* customer(void* arg)
{
    while(true)
    {
        sem_wait(&sem);//等待信号量
        pthread_mutex_lock(&mutex_node);
        //删掉一个节点
        struct node *pDel = Head;
        Head = Head->next;
        pthread_mutex_unlock(&mutex_node);

        pthread_mutex_lock(&mutex_out);
        std::cout << "customer: " << pthread_self() << " data: " << pDel->data << std::endl;
        pthread_mutex_unlock(&mutex_out);
        
        free(pDel);
        sleep(rand()%3);
    }
    return NULL;
}

int main()
{
    sem_init(&sem, 0, 0);
    pthread_mutex_init(&mutex_out, NULL);
    pthread_mutex_init(&mutex_node, NULL);
    pthread_t p1,p2;
    //创建生产者线程
    pthread_create(&p1, NULL, producer, NULL);
    //创建消费者线程
    pthread_create(&p2, NULL, customer, NULL);
    //回收线程资源
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_mutex_destroy(&mutex_node);
    pthread_mutex_destroy(&mutex_out);
    sem_destroy(&sem);
    return 0;
}

