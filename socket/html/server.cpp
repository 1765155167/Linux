#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <strings.h>
#include <sys/epoll.h>

int s_efd = 0;

struct s_events {
    int fd;
    int events;
    void* arg;
    void (*call_back)(int fd, int events, void* arg);
};

void check(std::string str, int res)
{
    if(res < 0) {
        perror(str.c_str());
        exit(-1);
    }
}

void recvdata(int cfd, int events, void* arg);
void senddata(int cfd, int events, void* arg);

void senddata(int cfd, int events, void* arg)
{
    if(!(events & EPOLLOUT)) {
        return;
    }
    int fd = open("./index.html", O_RDWR);
    char buf[4096];
    int len = read(fd, buf, sizeof(buf));
    check("read", len);
    buf[len] ='\0';
    int res = send(cfd, buf, len, 0);
    if(res == 0) {
        epoll_ctl(s_efd, EPOLL_CTL_DEL, cfd, NULL);
        std::cout << "client unlink..." << std::endl;
        return ;
    }
    struct s_events *s_ev = (struct s_events*)arg;
    s_ev->call_back = recvdata;
    s_ev->fd = cfd;
    s_ev->events = EPOLLIN;
    s_ev->arg = s_ev;
    struct epoll_event ev;
    ev.data.ptr = s_ev;
    ev.events = s_ev->events;
    epoll_ctl(s_efd, EPOLL_CTL_MOD, cfd, &ev);
    return ;
}

void recvdata(int cfd, int events, void* arg)
{
    if(!(events & EPOLLIN)) {
        return ;
    }
    char buf[4096] = {0};
    int len = recv(cfd, buf, sizeof(buf), 0);
    if(len == 0)
    {
        epoll_ctl(s_efd, EPOLL_CTL_DEL, cfd, NULL);
        std::cout << "client unlink..." << std::endl;
        return ;
    }
    std::cout << "recv:\n" << buf;

    struct s_events *s_ev = (struct s_events*)arg;
    s_ev->call_back = senddata;
    s_ev->fd = cfd;
    s_ev->events = EPOLLOUT;
    s_ev->arg = s_ev;
    struct epoll_event ev;
    ev.data.ptr = s_ev;
    ev.events = s_ev->events;
    epoll_ctl(s_efd, EPOLL_CTL_MOD, cfd, &ev);
}

void Accept(int fd, int events, void *arg)
{
    struct s_events *ev_arg = (struct s_events*)arg;
    if((!(events & EPOLLIN)) && (!(ev_arg->events & EPOLLIN))) {
        return ;
    }
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    bzero(&client_addr, client_len);
    int client_fd = accept(fd, (struct sockaddr*)&client_addr, &client_len);
    check("accept", client_fd);
    char IPBUF[BUFSIZ];
    std::cout << "client limit " << inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, IPBUF, client_len) 
        << ":" << ntohs(client_addr.sin_port) << std::endl;
    struct s_events *s_ev = new struct s_events;
    s_ev->call_back = recvdata;
    s_ev->fd = client_fd;
    s_ev->events = EPOLLIN;
    s_ev->arg = s_ev;
    struct epoll_event ev;
    ev.data.ptr = s_ev;
    ev.events = s_ev->events;
    epoll_ctl(s_efd, EPOLL_CTL_ADD, client_fd, &ev);
    return ;
}

int main(int argc, char* argv[])
{
    int serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    check("socket", serv_fd);
    
    struct sockaddr_in serv_addr;
    socklen_t serv_len = sizeof(serv_addr);
    bzero(&serv_addr, serv_len);
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    if(argc == 1) {
        inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    }else if(argc == 2) {
        inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr);
    }
    int res = bind(serv_fd, (struct sockaddr*)&serv_addr, serv_len);
    check("bind", res);
    
    listen(serv_fd, 128);
    
    const int FILE_MAX = 50;
    s_efd = epoll_create(FILE_MAX);
    check("epoll_create", s_efd);
    struct epoll_event env;
    struct epoll_event envs[FILE_MAX];
    struct s_events *s_ev = new struct s_events;
    s_ev->call_back = Accept;
    s_ev->fd = serv_fd;
    s_ev->events = EPOLLIN;
    s_ev->arg = &s_ev;
    env.data.ptr = s_ev;
    env.events = s_ev->events;
    res = epoll_ctl(s_efd, EPOLL_CTL_ADD, serv_fd, &env);
    check("epoll_ctl", res); 
    while(true) {
        int nready = epoll_wait(s_efd, envs, FILE_MAX, -1);
        check("epoll_wait", nready);
        int i = 0;
        for(i = 0; i < nready; i++) {
            if(envs[i].data.ptr == NULL) {
                break;
            }
            struct s_events *c_ev = (struct s_events*)envs->data.ptr;
            c_ev->call_back(c_ev->fd, c_ev->events, c_ev);
        }
    }
    close(s_efd);
    close(serv_fd);
    return 0;
}

