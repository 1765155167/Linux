#include <iostream>
 #include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include "hqf_log.h"
#include "wrap.h"

int main()
{
    struct epoll_event env;
    const int FILEMAX = 20;
    struct epoll_event envs[FILEMAX];
    int serv_fd = Socket(AF_INET, SOCK_STREAM, 0);
    int client_fd = 0;
    struct sockaddr_in serv_addr, client_addr;
    socklen_t serv_len = sizeof(serv_addr);
    socklen_t client_len = sizeof(client_addr);
    bzero(&serv_addr, serv_len);
    bzero(&client_addr, client_len);
    
    int opt = 1;
    setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    Bind(serv_fd, (struct sockaddr*)&serv_addr, serv_len);

    Listen(serv_fd, 128);
    int efd = epoll_create(FILEMAX);
    check("epoll_create", efd);
    env.events = EPOLLIN | EPOLLET;
    //env.events = EPOLLIN;
    env.data.fd = serv_fd;
    epoll_ctl(efd, EPOLL_CTL_ADD, serv_fd, &env);
    while(true) {
        int nready = epoll_wait(efd, envs, FILEMAX, -1);
        check("epoll_wait", nready);
        for(int i = 0; i < nready; i++) {
            if(!(envs[i].events&EPOLLIN)) {
                continue;
            }
            if(envs[i].data.fd == serv_fd) {
                client_fd = Accept(serv_fd, (struct sockaddr*)&client_addr, &client_len);
                char client_ip[50];
                std::cout << "client " << inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, client_len) << std::endl;
                env.data.fd = client_fd;
                //env.events = EPOLLIN | EPOLLET;
                //env.events = EPOLLIN;
                int res = epoll_ctl(efd, EPOLL_CTL_ADD, client_fd, &env);
                check("epoll_ctl", res);
            } else {
                int socketfd = envs[i].data.fd;
                int flag = fcntl(socketfd, F_GETFD);
                flag |= O_NONBLOCK;
                fcntl(socketfd, F_SETFD, flag);
                char buf[1024];
                int len = Read(socketfd, buf, sizeof(buf));
                //if(len == 0) {
                //    close(socketfd);
                //    epoll_ctl(efd, EPOLL_CTL_DEL, socketfd, NULL);
                //    continue;
                //}
                for(int j = 0; j < len; j++) {
                    buf[j] = toupper(buf[j]);
                }
                Write(socketfd, buf, len);
            }
        }
    } 
    close(serv_fd);
    close(efd);
    return 0;
}

