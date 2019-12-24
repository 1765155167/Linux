#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <arpa/inet.h>
#include "hqf_log.h"
#include "wrap.h"

int main()
{
    int client_fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    Connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    char* buf = new char[1024];
    memset((void*)buf, 0, sizeof(buf));
    while(true) {
        fgets(buf, 1024, stdin);
        //std::cout << "inout:" << buf << std::endl;
        int n = Write(client_fd, buf, strlen(buf));
        if((strcmp(buf, "exit\n") == 0) || (strcmp(buf, "exit") == 0))
        {
            std::cout << "正常退出..." << std::endl;
            break;
        }
        memset((void*)buf, 0, sizeof(buf));
        n = Read(client_fd, buf, n);
        if(n == 0)
        {
            std::cout << "与服务器断开链接" << std::endl;
            break;
        }
        write(STDOUT_FILENO, buf, n);
        std::cout << "size:" << n << std::endl;
    }
    delete[] buf;
    close(client_fd);
    return 0;
}
