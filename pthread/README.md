# 线程
## 特点
- 主线程退出时，子线程会被强制结束
- 线程之间共享全局变量
## 函数
1. 创建一个线程
```c
int pthread_create(
    pthread_t *thread,//线程id(unsigned long int)
    const pthread_attr_t *attr,//线程属性
    void *(*start_routine) (void *),//线程处理函数
    void *arg//参数
);
```
- * thread:传出参数，线程创建后会被设置一个合适的值
- * attr:设置线程分离，默认NULL(父子进程不分离，需要手动去回收线程pcb) 
- * - int pthread_attr_init(pthread_attr\* attr);//初始化
- * - 设置线程分离属性
```c
int pthread_attr_setdetachstate(
    pthread_attr_t* attr,//线程属性
    int detachstate//PTHREAD_CREATE_DETACHED(分离) ..._JOINABLE(不分离)
)
```
- * start_routine:子线程处理函数
- * arg:传给回调函数的参数
- * 返回值：成功返回0，不成功返回错误号
2. 单个退出线程
- * void pthread_exit(void\* retval);
- * - retval:必须指向全局或者堆
3. 堵塞等待线程退出，获取线程退出状态
- * int pthread_join(pthread_t thread, void\*\* retval);
- * - thread:要回收的子线程的线程id
- * - retval:读取线程退出时携带的状态信息
```c
pthread_t pthid;
int ret = pthread_create(pthid, NULL, myfunc, NULL);
void* ptr;
pthread_join(pthid, &ptr);
```
4. 线程分离 
- * int pthread_detach(pthread_t thread);
- * 调用该函数之后不需要pthread_join
- * 子线程会自动回收自己的pcb
5. 杀死线程
- * int pthread_cancel(pthread_t thread);
- * 注意
- * - 再要杀死的子线程对应的处理函数的内部，必须做过一次系统调用
- * - write read printf ...
6. int pthread_equal(pthread_t t1, pthread_t t2);
- * 判断两个线程ID是否相等 预留函数
## 线程同步
### 互斥量（互斥锁）
- 类型：pthread_mutex_t mutex;
- 特点：
- * 多个线程串行访问共享数据
- * 效率低
- 使用步骤：
- 1. 创建一个互斥锁 pthread_mutex_t mutex;
- 2. 初始化 pthread_mutex_init(&mutex, NULL);
- 3. 加锁 pthread_mutex_lock(&mutex);
- 4. 解锁 pthread_mutex_unlock(&mutex);
- 相关函数介绍
```
//初始化锁
pthread_mutex_init(
    pthread_mutex_t* restrict mutex,//锁
    const pthread_mutexattr_t* restrict attr//锁的属性
);
//销毁锁（释放）
pthread_mutex_destory(pthread_mutex_t* mutex);
//加锁会堵 直到锁打开为止 
pthread_mutex_lock(pthread_mutex_t* mutex);
//尝试加锁不堵塞，成功返回0
pthread_mutex_trylock(pthread_mutex_t* mutex);
//解锁
pthread_mutex_unlock(pthread_mutex_t* mutex);
```
### 读写锁
- 类型：pthread_rwlock_t lock;
- 读锁：对内存做读操作
- 写锁：对内存做写操作
#### 读写锁的特性
- 线程A加读锁成功，又来三个变量做读操作，可以加锁成功
- 线程A加写锁成功，又来三个线程做读操作，三个线程堵塞
- 线程A加读锁成功，有来了线程B加写锁堵塞，又来了线程C加读锁堵塞
- 读共享、写独占、写优先级高、
#### 读写锁场景练习
1. 线程A加写锁成功，然后线程B请求读锁？失败
2. 线程A加读锁成功，然后线程B请求写锁？堵塞
3. 线程A加读锁成功，然后线程B请求读锁？成功
4. 线程A加读锁成功，然后线程B请求写锁？失败。然后线程C请求读锁？失败
5. 线程A加写锁成功，然后线程B请求读锁，然后线程C请求写锁，
- * BC堵塞
- * A解锁，C加写锁成功，B继续堵塞
- * C解锁，B加读锁成功。
注：程序中读操作较多是建议使用读写锁
#### 读写锁操作函数
```c
pthread_relock_t rwlock;//定义一把读写锁
//初始化读写锁
pthread_rwlock_init(
    pthread_relock_t* restrict rwlock,
    const pthread_ewlockattr* restrict attr
                   );
//销毁读写锁
pthread_rwlock_destroy(phread_rwlock_t* rwlock);
//加读锁
pthread_rwlock_rdlock(pthread_rwlock_t* rwlock);
//尝试加读锁 成功返回0 错误返回错误号
pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock);
//加写锁
pthread_rwlock_wrlock(pthread_rwlock_t* rwlock);
//尝试加写锁
pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock);
//解锁
pthread_rwlock_unlock(pthread_rwlock_t* rwlock);
```
练习：3个线程不定时写同一全局资源，5个线程不定时读同一全局资源
### 条件变量
- 读写锁不是什么时候都能堵塞线程
- 条件变量：在不满足条件时堵塞线程
```c
pthread_cond_t cond;
//初始化体格天剑变量
pthread_cond_init(
    pthread_cond_t* restrict cond,
    const pthread_condattr* restrict attr
                 );
//销毁条件变量
pthread_cond_destroy(pthread_cond_t* cond);
//堵塞等待一个条件变量
//堵塞线程,将已经上锁的mutex解锁
pthread_cond_wait(
    pthread_cond_t* cond,
    pthread_mutex_t* restrict mutex
                 );
//限时等待一个条件变量,将已经加锁的mutex解锁
struct timespec abstime;
time_t cur = time(NULL);
abstime.tv_sec = cur+1;
pthread_cond_timedwait(
    pthread_cond_t* cond,
    pthread_mutex_t* restrict mutex,
    const struct timespec* restrict abstime
                      );
//唤醒堵塞的进程并对互斥锁mutex加锁
pthread_cond_signal(pthread_cond_t* cond);//唤醒至少一个
pthread_cond_broadcast(pthread_cond_t* cond);//唤醒全部
```
#### 生产者消费者模型
```C
//消费者
while(true)
{
    //判断是不是有烧饼
    pthread_mutex_lock();
    if(head == NULL)
    {
        pthread_cond_wait(&cond, &mutex);//堵塞等待
    }
    //吃烧饼
    pthread_mutex_unlock();
    sleep(1);
}
//生产者
while(true)
{
    //创建一个节点（烧饼）
    pthread_mutex_lock(&mutex);
    //节点插入到链表
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(cond);//通知堵塞的消费者解除堵塞
}
```
### 信号量
- #include <semaphore.h>
- 类型：sem_t sem;//加强版的互斥锁
- 函数
```c
//初始化信号量
sem_init(
    sem_t* sem,
    int pshared,//1 进程同步， 0线程同步
    unsigned int value//信号量个数
        );
//销毁信号量
sem_destroy(sem_t* sem);
//加锁--
sem_wait(sem_t* sem);
//尝试枷锁 成功返回0 不成功返回错误号
sem_trywait(sem_t* sem);
//限时尝试枷锁
sem_timedwait(sem_t* sem, xxxx);
//解锁++
sem_post(sem_t* sem);
```
### 哲学家就餐问题
