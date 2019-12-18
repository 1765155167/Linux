#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <arpa/inet.h>
#include "hqf_log.h"

int main()
{
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    check("socket", client_fd);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6666);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    int ret = connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    check("connect", ret);
    const char* buf = "hello I'am hqf";
    while(true) {
        int n = write(client_fd, buf, strlen(buf)+1);
        check("write", n);
        char* rbuf = new char[1024];
        n = read(client_fd, rbuf, n);
        check("read", n);
        std::cout << "read: " << rbuf << std::endl;
        sleep(1);
    }
    write(client_fd, "exit", strlen("exit"));
    close(client_fd);
    return 0;
}