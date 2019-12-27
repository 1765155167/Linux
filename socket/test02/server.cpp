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

struct clientST
{
    struct sockaddr_in addr;
    int fd;
};

void *dowork(void* arg)
{
    struct clientST* client = (struct clientST*) arg;
    char IP_BUF[BUFSIZ];
    inet_ntop(AF_INET, (void*)&client->addr.sin_addr.s_addr, IP_BUF, sizeof(struct sockaddr_in));
    std::cout << "client ip:" << IP_BUF << " port:" << ntohs(client->addr.sin_port) << std::endl;
    char* buf = new char[BUFSIZ];
    while(true) {
        bzero((void*)buf, sizeof(buf));
        int len = Readline(client->fd, buf);
        if(len == 0)
        {
            break;
        }
        if((strcmp(buf, "exit\n") == 0) || (strcmp(buf, "exit") == 0))
        {
            break;
        }
        for(int i = 0; i < len; i++)
        {
            buf[i] = toupper(buf[i]);
        }
        Write(client->fd, buf, len);
    }
    delete[] buf;
    std::cout << IP_BUF << "client 断开链接" << std::endl;
    close(client->fd);
    return NULL;
}

int main()
{
    int server_fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t server_len = sizeof(server_addr);
    socklen_t client_len = sizeof(client_addr);
    bzero(&server_addr, sizeof(server_addr));
    bzero(&client_addr, sizeof(client_addr));
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定本地有效的任意IP
    Bind(server_fd, (struct sockaddr*)&server_addr, server_len);

    Listen(server_fd, 128);//设置最大监听数

    while(true) {   
        int client_fd = Accept(server_fd,(struct sockaddr*)&client_addr, &client_len);
        pthread_t pthid = 0;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//设置线程分离。不需要回收子线程
        struct clientST client;
        client.addr = client_addr;
        client.fd = client_fd;
        pthread_create(&pthid, &attr, dowork, (void*)&client);
    }
    close(server_fd);
    return 0;
}
