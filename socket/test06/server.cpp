#include <iostream>
#include "hqf_log.h"
#include <sys/epoll.h>
#include "wrap.h"
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
int main()
{
    std::string welcome = "hello I'am Hqf";
    char readBuf[256];
    char ip_buf[256];
    int serv_fd, client_fd;
    struct sockaddr_in serv_addr, client_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    bzero(&client_fd, sizeof(client_addr));
    socklen_t serv_len = sizeof(serv_addr);
    socklen_t client_len = sizeof(client_addr);
    serv_fd = Socket(AF_INET, SOCK_STREAM, 0);
    
    int opt = 1;
    setsockopt(serv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //setsockopt();
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    Bind(serv_fd, (struct sockaddr*)&serv_addr, serv_len);

    Listen(serv_fd, 128);

    const int EPOLLNUM = 5000;
    struct epoll_event envs[EPOLLNUM];
    
    ssize_t efd = epoll_create(EPOLLNUM);
    check("epoll_create", efd);
    
    struct epoll_event env;
    env.events = EPOLLIN;
    env.data.fd = serv_fd;
    int res = epoll_ctl(efd, EPOLL_CTL_ADD, serv_fd, &env);
    check("epoll_ctl", res);

    while(true) {
        int nready = epoll_wait(efd, envs, EPOLLNUM, -1);
        check("epoll_wait", nready);
        for(int i = 0; i < nready; i ++) {
            if(!(envs[i].events & EPOLLIN)){
                continue;
            }
            if(envs[i].data.fd == serv_fd) {
                client_fd = Accept(serv_fd, (struct sockaddr*)&client_addr, &client_len);
                std::cout << "client IP: " 
                    << inet_ntop(AF_INET, (void*)&client_addr.sin_addr.s_addr, ip_buf, sizeof(ip_buf))
                    << "port:" << ntohs(client_addr.sin_port) << std::endl;

                //res = Write(client_fd, welcome.c_str(), welcome.length());
                env.events = EPOLLIN;
                env.data.fd = client_fd;
                res = epoll_ctl(efd, EPOLL_CTL_ADD, client_fd, &env);
                check("epoll_ctl", res);
            } else {
                int sockfd = envs[i].data.fd;
                int len = Read(sockfd, readBuf, sizeof(readBuf));
                if(len <= 0) {
                    close(sockfd);
                    epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
                } else {
                    for(int j = 0; j< len; j++) {
                        readBuf[j] = toupper(readBuf[j]);
                    }
                    res = Write(sockfd, readBuf, len);
                    if(res <= 0) {
                        close(sockfd);
                        epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
                    }
                }
            }
        }
    }
    close(serv_fd);
    close(efd);
    return 0;
}

