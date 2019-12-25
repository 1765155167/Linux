#include <sys/types.h>
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
    int nready = 0, client[FD_SETSIZE];
    int maxfd = 0, server_fd = 0, client_fd = 0;
    char buf[BUFSIZ], str[INET_ADDRSTRLEN];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    fd_set rset, allset;

    server_fd = Socket(AF_INET, SOCK_STREAM, 0);

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

    maxfd = server_fd;
    maxi = -1;
    for(int i = 0; i < FD_SETSIZE; i++)
    {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(server_fd, &allset);

    while(true) {   
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        check("select", nready);

        if(FD_ISSET(server_fd, &rset)) {
            client_fd = Accept(server_fd,(struct sockaddr*)&client_addr, &client_len);
            std::cout << "client ip:" << inet_ntop(AF_INET, &client_addr.sin_addr, str, sizeof(str))
                << "port" << ntohs(client_addr.sin_port) << std::endl;
            int i = 0;
            for(i = 0; i < FD_SETSIZE; i++)
            {
                if(client[i] < 0)
                {
                    client[i] = client_fd;
                    break;
                }
            }

            if(i == FD_SETSIZE) {
                std::cerr << "too many client" << std::endl;
                exit(-1);
            }

            FD_SET(client_fd, &allset);
            if(client_fd > maxfd) {
                maxfd = client_fd;
            }
            if(i > maxi) {
                maxi = i;
            }
            if(--nready == 0) {
                continue;
            }
        }
        for(int i = 0; i <= maxi; i++) {
            if(client[i] < 0) {
                continue;
            }
            if(FD_ISSET(client[i], &rset)) {
                int len = Read(client[i], buf, sizeof(buf));
                if(len == 0) {
                    close(client[i]);
                    FD_CLR(client[i], &allset);
                    client[i] = -1;
                } else if(len > 0) {
                    for(int j = 0; j < len; j++)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    sleep(10);
                    Write(client[i], buf, len);
                }
                if(--nready == 0)
                    break;
            }
        }
    }
    close(server_fd);
    return 0;
}
