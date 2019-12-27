#include <iostream>
#include "hqf_log.h"
#include "wrap.h"
#include <poll.h>
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
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr.s_addr);
    Bind(serv_fd, (struct sockaddr*)&serv_addr, serv_len);

    Listen(serv_fd, 128);

    const int POLLNUM = 1024;
    struct pollfd fds[POLLNUM];
    int maxi = 0;
    fds[0].fd = serv_fd;
    fds[0].events = POLLIN;
    for(int i = 1; i < POLLNUM; i++) {
        fds[i].fd = -1;
    }
    while(true) {
        int nready = poll(fds, 5, -1);
        check("poll", nready);
        if(fds[0].revents & POLLIN) {
            client_fd = Accept(serv_fd, (struct sockaddr*)&client_addr, &client_len);
            std::cout << "client IP: " 
                << inet_ntop(AF_INET, (void*)&client_addr.sin_addr.s_addr, ip_buf, sizeof(ip_buf))
                << "port:" << ntohs(client_addr.sin_port) << std::endl;

            Write(client_fd, welcome.c_str(), welcome.length());

            int i = 1;
            for(; i < POLLNUM; i++) {
                if(fds[i].fd < 0) {
                    fds[i].fd = client_fd;
                    fds[i].events = POLLIN;
                    break;
                }
            }
            if(maxi < i) {
                maxi = i;
            }
            if(--nready == 0) {
                continue;
            }
        }
        for(int i = 1; i <= maxi; i++)
        {
            if(fds[i].revents & POLLIN) {
                int sockfd = fds[i].fd;
                if(sockfd < 0) {
                    break;
                }
                size_t len = Read(sockfd, readBuf, 256);
                if(len == 0) {
                    fds[i].fd = -1;
                    close(sockfd);
                } else {
                    for(size_t j = 0; j < len; j++) {
                        readBuf[j] = toupper(readBuf[j]);
                    }
                    Write(sockfd, readBuf, len);
                }
                if(--nready <= 0) {
                    break;
                }
            }
        }
    }
    return 0;
}

