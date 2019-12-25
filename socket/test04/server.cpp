#include <sys/types.h>
#include <poll.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "hqf_log.h"
#include "wrap.h"


int main()
{
    int maxi = 0;
    int server_fd = 0, client_fd = 0;
    char buf[BUFSIZ], str[INET_ADDRSTRLEN];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_fd = Socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    socklen_t server_len = sizeof(server_addr);
    socklen_t client_len = sizeof(client_addr);
    bzero(&server_addr, sizeof(server_addr));
    bzero(&client_addr, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定本地有效的任意IP
    Bind(server_fd, (struct sockaddr*)&server_addr, server_len);

    Listen(server_fd, 128);//设置最大监听数
    
    const int OPENMAX = 65535;
    struct pollfd fds[OPENMAX];
    fds[0].fd = server_fd;
    fds[0].events= POLLIN;

    for(int i = 1; i < OPENMAX; i++)
    {
        fds[i].fd = -1;
    }
    maxi = 0;

    while(true) {
        int nreadly = poll(fds, maxi+1, -1);
        if(fds[0].revents & POLLIN) {
            client_fd = Accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
            std::cout << "client ip:" << inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str))
                << "port" << ntohs(client_addr.sin_port) << std::endl;
            std::string hello = "<html>\n";
            hello += "\t<title>\n";
            hello += "\t\tHello\n";
            hello += "\t</title>\n";
            hello += "\t<body>\n";
            hello += "\t\tWeclome\n";
            hello += "\t<body>\n";
            hello += "\t<html>\n";
            //Write(client_fd, hello.c_str(), hello.length());

            int i  = 0;
            for(i = 1; i < OPENMAX; i++ ) {
                if(fds[1].fd < 0) {
                    fds[i].fd = client_fd;
                    fds[i].events = POLLIN;
                    break;
                }
            }
            
            if(i == OPENMAX) {
                std::cerr << "too many client" << std::endl;
                exit(-1);
            }
            
            if(i > maxi) {
                maxi = i;
            }
            if(--nreadly == 0) {
                continue;
            }
        }
        for(int i = 1; i <= maxi; i++)
        {
            int sockfd = fds[i].fd;
            if(sockfd < 0)
            {
                continue;
            }
            if(fds[i].revents & POLLIN) {
                int len = Read(sockfd, buf, sizeof(buf));
                if(len == 0) {
                    fds[i].fd = -1;
                } else {
                    for(int j = 0; j < len; j++) {
                        buf[j] = toupper(buf[j]);
                    }
                    Write(sockfd, buf, len);
                }
                if(--nreadly <= 0) {
                    break;
                }
            }
        }
    }
    close(server_fd);
    return 0;
}
