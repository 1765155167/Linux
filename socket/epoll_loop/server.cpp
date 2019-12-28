/*
 *epoll NBNOLOCK IO
 */
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "wrap.h"

#define MAX_EVENTS 1024
#define BUFLEN 4096
#define SERV_PORT 8080

void recvdata(int fd, int events, void* arg);
void senddata(int fd, int events, void* arg);

struct event_s {
    int fd;
    int events;
    void* arg;
    void (*call_back)(int fd, int events, void* arg);
    int status;
    char buf[BUFLEN];
    int len;
    long last_active;
};

int g_efd;
struct event_s g_events[MAX_EVENTS + 1];

void eventset(struct event_s* ev, int fd, void (*call_back)(int, int , void*), void* arg)
{
    ev->fd = fd;
    ev->events = 0;
    ev->call_back = call_back;
    ev->arg = arg;
    ev->status = 0;
    ev->last_active = time(NULL);
}

void eventadd(int efd, int events, struct event_s* ev)
{
    struct epoll_event epv = {0, {0}};
    int op;
    epv.data.ptr =ev;
    epv.events = ev->events = events;

    if(ev->status == 1)
    {
        op = EPOLL_CTL_MOD;
    } else {
        op = EPOLL_CTL_ADD;
        ev->status = 1;
    }

    if(epoll_ctl(efd, op, ev->fd, &epv) < 0) {
        std::cout << "event add faild [fd = " << ev->fd << "] events" << events << std::endl;  
    } else {
        std::cout << "event add ok [fd = " << ev->fd << "] events" << events << std::endl;  
    }

    return ;
}

void eventdel(int efd, struct event_s* ev)
{
    if(ev->status != 1) {
        return ;
    }
    struct epoll_event epv = {0, {0}};
    epv.data.ptr = ev;
    ev->status = 0;
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv);

    return ;
}

void acceptconn(int lfd, int events, void* arg)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = 0;

    client_fd = Accept(lfd, (struct sockaddr*)&client_addr, &client_len);
    do {
        int i = 0;
        for(;i < MAX_EVENTS; i++)
        {
            if(g_events[i].status == 0)
            {
                break;
            }
        }
        if(i == MAX_EVENTS) {
            std::cout << __func__ << "max Connect limit" << std::endl;
        }
        int flag = fcntl(client_fd, F_GETFL);
        if(flag < 0) {
            perror("fcntl");
            break;
        }
        flag |= O_NONBLOCK;
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
        if(flag < 0) {
            perror("fcntl");
            break;
        }

        eventset(&g_events[i], client_fd, recvdata, &g_events[i]);
        eventadd(g_efd, events, &g_events[i]);
    }while(0);
    char buf[256];
    std::cout << "new client connect ip:" << inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, buf, client_len)
        << ":" << ntohs(client_addr.sin_port) << std::endl;
    return ;
}

void recvdata(int fd, int events, void* arg)
{
    struct event_s *ev = (struct event_s*)arg;
    if(events != EPOLLIN) {
        return ;
    }
    int len = recv(fd, ev->buf, sizeof(ev->buf), 0);
    eventdel(g_efd, ev);
    if(len > 0) {
        ev->len = len;
        for(int i = 0; i < len; i++) {
            ev->buf[i] = toupper(ev->buf[i]);
        }
        ev->buf[len] = '\0';
        eventset(ev, fd, senddata, ev);
        eventadd(g_efd, EPOLLOUT, ev);
    } else if (len == 0) {
        close(ev->fd);
        std::cout << "[fd:" << fd << "] [pos:" << ev - g_events  << "] close "<< std::endl;
    } else {
        close(ev->fd);
        std::cout << "[fd:" << fd << "] [pos:" << ev - g_events  << "] close "<< std::endl;
    }
    return ;
}

void senddata(int fd, int events, void* arg)
{
    struct event_s* ev = (struct event_s*)arg;
    if(events != EPOLLOUT) {
        return ;
    }
    int len = send(fd, ev->buf, ev->len, 0);
    if(len > 0) {
        eventdel(g_efd, ev);
        eventset(ev, fd, recvdata, ev);
        eventadd(g_efd, EPOLLIN, ev);
    } else {
        close(ev->fd);
        eventdel(g_efd, ev);
        std::cout << "send[fd:" << ev->fd << "] errno :" << strerror(errno) << std::endl;
    }
    return ;
}

void initListenSocket(int efd, short port)
{
    int serv_fd = Socket(AF_INET, SOCK_STREAM, 0);
    fcntl(serv_fd, F_SETFL, O_NONBLOCK);

    eventset(&g_events[MAX_EVENTS], serv_fd, acceptconn, &g_events[MAX_EVENTS]);
    eventadd(efd, EPOLLIN, &g_events[MAX_EVENTS]);
    
    struct sockaddr_in serv_addr;
    socklen_t serv_len = sizeof(serv_addr); 
    bzero(&serv_addr, serv_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    Bind(serv_fd, (struct sockaddr*)&serv_addr, serv_len);
    Listen(serv_fd, 128);
    return ;
}


int main(int argc, char *argv[])
{
    unsigned short port = SERV_PORT;

    if (argc == 2)
        port = atoi(argv[1]);                           //使用用户指定端口.如未指定,用默认端口

    g_efd = epoll_create(MAX_EVENTS+1);                 //创建红黑树,返回给全局 g_efd 
    if (g_efd <= 0)
        printf("create efd in %s err %s\n", __func__, strerror(errno));

    initListenSocket(g_efd, port);                      //初始化监听socket

    struct epoll_event events[MAX_EVENTS+1];            //保存已经满足就绪事件的文件描述符数组 
	printf("server running:port[%d]\n", port);

    int checkpos = 0, i;
    while (1) {
        /* 超时验证，每次测试100个链接，不测试listenfd 当客户端60秒内没有和服务器通信，则关闭此客户端链接 */

        long now = time(NULL);                          //当前时间
        for (i = 0; i < 100; i++, checkpos++) {         //一次循环检测100个。 使用checkpos控制检测对象
            if (checkpos == MAX_EVENTS)
                checkpos = 0;
            if (g_events[checkpos].status != 1)         //不在红黑树 g_efd 上
                continue;

            long duration = now - g_events[checkpos].last_active;       //客户端不活跃的世间

            if (duration >= 60) {
                close(g_events[checkpos].fd);                           //关闭与该客户端链接
                printf("[fd=%d] timeout\n", g_events[checkpos].fd);
                eventdel(g_efd, &g_events[checkpos]);                   //将该客户端 从红黑树 g_efd移除
            }
        }

        /*监听红黑树g_efd, 将满足的事件的文件描述符加至events数组中, 1秒没有事件满足, 返回 0*/
        int nfd = epoll_wait(g_efd, events, MAX_EVENTS+1, 1000);
        if (nfd < 0) {
            printf("epoll_wait error, exit\n");
            break;
        }

        for (i = 0; i < nfd; i++) {
            /*使用自定义结构体myevent_s类型指针, 接收 联合体data的void *ptr成员*/
            struct event_s *ev = (struct event_s *)events[i].data.ptr;  

            if ((events[i].events & EPOLLIN) && (ev->events & EPOLLIN)) {           //读就绪事件
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
            if ((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT)) {         //写就绪事件
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }
    }

    /* 退出前释放所有资源 */
    return 0;
}

